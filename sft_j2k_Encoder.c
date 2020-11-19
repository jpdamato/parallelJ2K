
#include <math.h>

#include "sft_j2k_Encoder.h"
#include "opj_malloc.h"
#include "int.h"
#include "mct.h"
#include "pi.h"
#include "bio.h"

/* Define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif


#define WS(i) v->mem[(i)*2]
#define WD(i) v->mem[(1+(i)*2)]


#define S(i) a[(i)*2]
#define D(i) a[(1+(i)*2)]
#define S_(i) ((i)<0?S(0):((i)>=sn?S(sn-1):S(i)))
#define D_(i) ((i)<0?D(0):((i)>=dn?D(dn-1):D(i)))
/* new */
#define SS_(i) ((i)<0?S(0):((i)>=dn?S(dn-1):S(i)))
#define DD_(i) ((i)<0?D(0):((i)>=sn?D(sn-1):D(i)))

/* <summary>                                                              */
/* This table contains the norms of the 5-3 wavelets for different bands. */
/* </summary>                                                             */
static const double dwt_norms[4][10] = {
	{1.000, 1.500, 2.750, 5.375, 10.68, 21.34, 42.67, 85.33, 170.7, 341.3},
	{1.038, 1.592, 2.919, 5.703, 11.33, 22.64, 45.25, 90.48, 180.9},
	{1.038, 1.592, 2.919, 5.703, 11.33, 22.64, 45.25, 90.48, 180.9},
	{.7186, .9218, 1.586, 3.043, 6.019, 12.01, 24.00, 47.97, 95.93}
};


typedef union {
	float	f[4];
} v4;

typedef struct v4dwt_local {
	v4*	wavelet ;
	int		dn ;
	int		sn ;
	int		cas ;
} v4dwt_t ;

static const float dwt_alpha =  1.586134342f; /*  12994 */
static const float dwt_beta  =  0.052980118f; /*    434 */
static const float dwt_gamma = -0.882911075f; /*  -7233 */
static const float dwt_delta = -0.443506852f; /*  -3633 */

static const float K      = 1.230174105f; /*  10078 */
/* FIXME: What is this constant? */
static const float c13318 = 1.625732422f;


#pragma region REGION_TIER1

iopj_t1_t* it1_create(opj_common_ptr cinfo) 
{
	iopj_t1_t *t1 = (iopj_t1_t*) sft_malloc(sizeof(iopj_t1_t));
	if(!t1)
		return NULL;

	t1->cinfo = cinfo;
	/* create MQC and RAW handles */
//	t1->mqc = mqc_create();
	t1->mqc = (opj_mqc_t*)sft_malloc(sizeof(opj_mqc_t)) ;
//	t1->raw = raw_create();
	t1->raw = NULL;

	t1->data=NULL;
	t1->flags=NULL;
	t1->datasize=0;
	t1->flagssize=0;

	return t1;
}


opj_bool iallocate_buffers(iopj_t1_t *t1,	int w,	int h)
{
	int datasize=w * h;
	int flagssize;

	if(datasize > t1->datasize)
	{
		sft_free(t1->data);
		t1->data = (int*) sft_aligned_malloc(datasize * sizeof(int));
		if(!t1->data){
			return OPJ_FALSE;
		}
		t1->datasize=datasize;
	}
	sft_memSet(t1->data,0,datasize * sizeof(int));

	t1->flags_stride=w+2;
	flagssize=t1->flags_stride * (h+2);

	if(flagssize > t1->flagssize)
	{
		sft_free(t1->flags);
		t1->flags = (flag_t*) sft_aligned_malloc(flagssize * sizeof(flag_t));
		if(!t1->flags){
			return OPJ_FALSE;
		}
		t1->flagssize=flagssize;
	}
	sft_memSet(t1->flags,0,flagssize * sizeof(flag_t));

	t1->w=w;
	t1->h=h;

	return OPJ_TRUE;
}


/* <summary>                */
/* Get norm of 5-3 wavelet. */
/* </summary>               */
double idwt_getnorm(int level, int orient) 
{
	return dwt_norms[orient][level];
}



char it1_getctxno_zc(int f, int orient)
{
	return lut_ctxno_zc[(orient << 8) | (f & T1_SIG_OTH)];
}

char it1_getctxno_sc(int f)
{
	return lut_ctxno_sc[(f & (T1_SIG_PRIM | T1_SGN)) >> 4];
}

int it1_getctxno_mag(int f) 
{
	int tmp1 = (f & T1_SIG_OTH) ? T1_CTXNO_MAG + 1 : T1_CTXNO_MAG;
	int tmp2 = (f & T1_REFINE) ? T1_CTXNO_MAG + 2 : tmp1;
	return (tmp2);
}

short it1_getnmsedec_sig(int x, int bitpos) 
{
	if (bitpos > T1_NMSEDEC_FRACBITS) {
		return lut_nmsedec_sig[(x >> (bitpos - T1_NMSEDEC_FRACBITS)) & ((1 << T1_NMSEDEC_BITS) - 1)];
	}

	return lut_nmsedec_sig0[x & ((1 << T1_NMSEDEC_BITS) - 1)];
}

/** mod fixed_quality */
double it1_getwmsedec(	int nmsedec,	int compno,	int level,	int orient,	int bpno,
	int qmfbid,	double stepsize,	int numcomps,	int mct)
{
	double w1, w2, wmsedec;
	//if (qmfbid == 1) {
	w1 = (mct && numcomps==3) ? mct_getnorm(compno) : 1.0;
	w2 = idwt_getnorm(level, orient);
	//} 
	wmsedec = w1 * w2 * stepsize * (1 << bpno);
	wmsedec *= wmsedec * nmsedec / 8192.0;

	return wmsedec;
}

char it1_getspb(int f) 
{
	return lut_spb[(f & (T1_SIG_PRIM | T1_SGN)) >> 4];
}

short it1_getnmsedec_ref(int x, int bitpos) 
{
	if (bitpos > T1_NMSEDEC_FRACBITS)
	{
		return lut_nmsedec_ref[(x >> (bitpos - T1_NMSEDEC_FRACBITS)) & ((1 << T1_NMSEDEC_BITS) - 1)];
	}

	return lut_nmsedec_ref0[x & ((1 << T1_NMSEDEC_BITS) - 1)];
}



void it1_updateflags(flag_t *flagsp, int s, int stride) 
{
	flag_t *np = flagsp - stride;
	flag_t *sp = flagsp + stride;

	const flag_t mod[] = {
		T1_SIG_S, T1_SIG_S|T1_SGN_S,
		T1_SIG_E, T1_SIG_E|T1_SGN_E,
		T1_SIG_W, T1_SIG_W|T1_SGN_W,
		T1_SIG_N, T1_SIG_N|T1_SGN_N
	};

	np[-1] |= T1_SIG_SE;
	np[0]  |= mod[s];
	np[1]  |= T1_SIG_SW;

	flagsp[-1] |= mod[s+2];
	flagsp[0]  |= T1_SIG;
	flagsp[1]  |= mod[s+4];

	sp[-1] |= T1_SIG_NE;
	sp[0]  |= mod[s+6];
	sp[1]  |= T1_SIG_NW;
}


void it1_enc_sigpass_step(	iopj_t1_t *t1,	flag_t *flagsp,	int *datap,	int orient,	int bpno,	int one,
	int *nmsedec,	char type,	int vsc)
{
	int v, flag;

	opj_mqc_t *mqc = t1->mqc;	/* MQC component */

	if (vsc )
		flag = ((*flagsp) & (~(T1_SIG_S | T1_SIG_SE | T1_SIG_SW | T1_SGN_S))) ;
	else
	   flag =  (*flagsp);
	
	
	if ((flag & T1_SIG_OTH) && !(flag & (T1_SIG | T1_VISIT)))
	{
		v = int_abs(*datap) & one ? 1 : 0;
		mqc_setcurctx(mqc, it1_getctxno_zc(flag, orient));	/* ESSAI */
		if (type == T1_TYPE_RAW) {	/* BYPASS/LAZY MODE */
			mqc_bypass_enc(mqc, v);
		} else {
			mqc_encode(mqc, v);
		}
		if (v) 
		{
			if (*datap < 0 )
				v = 1;
			else
				v = 0;
			
			*nmsedec +=	it1_getnmsedec_sig(int_abs(*datap), bpno + T1_NMSEDEC_FRACBITS);
			mqc_setcurctx(mqc, it1_getctxno_sc(flag));	/* ESSAI */
			if (type == T1_TYPE_RAW) 
			{	/* BYPASS/LAZY MODE */
				mqc_bypass_enc(mqc, v);
			}
			else 
			{
				mqc_encode(mqc, v ^ it1_getspb(flag));
			}
			it1_updateflags(flagsp, v, t1->flags_stride);
		}
		*flagsp |= T1_VISIT;
	}
}


void it1_enc_clnpass_step(	iopj_t1_t *t1,	flag_t *flagsp,	int *datap,	int orient,	int bpno,	int one,
	int *nmsedec,	int partial,	int vsc)
{
	int v, flag;

	opj_mqc_t *mqc = t1->mqc;	/* MQC component */

	if (vsc)
       flag = ((*flagsp) & (~(T1_SIG_S | T1_SIG_SE | T1_SIG_SW | T1_SGN_S))) ;
	else
	   flag = (*flagsp);
	if (partial) 
	{
		//goto LABEL_PARTIAL;
		*nmsedec += it1_getnmsedec_sig(int_abs(*datap), bpno + T1_NMSEDEC_FRACBITS);
		mqc_setcurctx(mqc, it1_getctxno_sc(flag));
		v = *datap < 0 ? 1 : 0;
		mqc_encode(mqc, v ^ it1_getspb(flag));
		it1_updateflags(flagsp, v, t1->flags_stride);
	}
	else
	if (!(*flagsp & (T1_SIG | T1_VISIT))) 
	{
		mqc_setcurctx(mqc, it1_getctxno_zc(flag, orient));
		v = int_abs(*datap) & one ? 1 : 0;
		mqc_encode(mqc, v);
		if (v) 
		{
           // LABEL_PARTIAL:
			*nmsedec += it1_getnmsedec_sig(int_abs(*datap), bpno + T1_NMSEDEC_FRACBITS);
			mqc_setcurctx(mqc, it1_getctxno_sc(flag));
			v = *datap < 0 ? 1 : 0;
			mqc_encode(mqc, v ^ it1_getspb(flag));
			it1_updateflags(flagsp, v, t1->flags_stride);
		}
	}
	*flagsp &= ~T1_VISIT;
}


void it1_enc_clnpass(	iopj_t1_t *t1,	int bpno,	int orient,	int *nmsedec,	int cblksty)
{
	int i, j, k, one, agg, runlen, vsc;

	opj_mqc_t *mqc = t1->mqc;	/* MQC component */

	*nmsedec = 0;
	one = 1 << (bpno + T1_NMSEDEC_FRACBITS);
	for (k = 0; k < t1->h; k += 4) 
	{
		for (i = 0; i < t1->w; ++i) 
		{
			if (k + 3 < t1->h) 
			{
				if (cblksty & J2K_CCP_CBLKSTY_VSC) 
				{
					agg = !(iMACRO_t1_flags(1 + k,1 + i) & (T1_SIG | T1_VISIT | T1_SIG_OTH)
						|| iMACRO_t1_flags(1 + k + 1,1 + i) & (T1_SIG | T1_VISIT | T1_SIG_OTH)
						|| iMACRO_t1_flags(1 + k + 2,1 + i) & (T1_SIG | T1_VISIT | T1_SIG_OTH)
						|| (iMACRO_t1_flags(1 + k + 3,1 + i) 
						& (~(T1_SIG_S | T1_SIG_SE | T1_SIG_SW |	T1_SGN_S))) & (T1_SIG | T1_VISIT | T1_SIG_OTH));
				} 
				else 
				{
					agg = !(iMACRO_t1_flags(1 + k,1 + i) & (T1_SIG | T1_VISIT | T1_SIG_OTH)
						|| iMACRO_t1_flags(1 + k + 1,1 + i) & (T1_SIG | T1_VISIT | T1_SIG_OTH)
						|| iMACRO_t1_flags(1 + k + 2,1 + i) & (T1_SIG | T1_VISIT | T1_SIG_OTH)
						|| iMACRO_t1_flags(1 + k + 3,1 + i) & (T1_SIG | T1_VISIT | T1_SIG_OTH));
				}
			} 
			else 
			{
				agg = 0;
			}
			if (agg) 
			{
				for (runlen = 0; runlen < 4; ++runlen) 
				{
					if (int_abs(t1->data[((k + runlen)*t1->w) + i]) & one)
						break;
				}
				mqc_setcurctx(mqc, T1_CTXNO_AGG);
				mqc_encode(mqc, runlen != 4);
				if (runlen == 4) 
				{
					continue;
				}
				mqc_setcurctx(mqc, T1_CTXNO_UNI);
				mqc_encode(mqc, runlen >> 1);
				mqc_encode(mqc, runlen & 1);
			} 
			else 
			{
				runlen = 0;
			}
			for (j = k + runlen; j < k + 4 && j < t1->h; ++j) 
			{
				vsc = ((cblksty & J2K_CCP_CBLKSTY_VSC) && (j == k + 3 || j == t1->h - 1)) ? 1 : 0;
				it1_enc_clnpass_step(
					t1,
					&t1->flags[((j+1) * t1->flags_stride) + i + 1],
					&t1->data[(j * t1->w) + i],
					orient,
					bpno,
					one,
					nmsedec,
					agg && (j == k + runlen),
					vsc);
			}
		}
	}
}

void it1_enc_refpass_step(iopj_t1_t *t1,	flag_t *flagsp,	int *datap,	int bpno,	int one,	int *nmsedec,	char type,	int vsc)
{
	int v, flag;

	opj_mqc_t *mqc = t1->mqc;	/* MQC component */

	flag = vsc ? ((*flagsp) & (~(T1_SIG_S | T1_SIG_SE | T1_SIG_SW | T1_SGN_S))) : (*flagsp);
	if ((flag & (T1_SIG | T1_VISIT)) == T1_SIG) 
	{
		*nmsedec += it1_getnmsedec_ref(int_abs(*datap), bpno + T1_NMSEDEC_FRACBITS);
		v = int_abs(*datap) & one ? 1 : 0;
		mqc_setcurctx(mqc, it1_getctxno_mag(flag));	/* ESSAI */
		//JUAN if (type == T1_TYPE_RAW) 
		
		mqc_encode(mqc, v);
		
		*flagsp |= T1_REFINE;
	}
}




void it1_enc_refpass(iopj_t1_t *t1,	int bpno,	int *nmsedec,	char type,	int cblksty)
{
	int i, j, k, one, vsc;
	*nmsedec = 0;
	one = 1 << (bpno + T1_NMSEDEC_FRACBITS);
	for (k = 0; k < t1->h; k += 4)
	{
		for (i = 0; i < t1->w; ++i) 
		{
			for (j = k; j < k + 4 && j < t1->h; ++j)
			{
				vsc = ((cblksty & J2K_CCP_CBLKSTY_VSC) && (j == k + 3 || j == t1->h - 1)) ? 1 : 0;
				it1_enc_refpass_step(
					t1,
					&t1->flags[((j+1) * t1->flags_stride) + i + 1],
					&t1->data[(j * t1->w) + i],
					bpno,
					one,
					nmsedec,
					type,
					vsc);
			}
		}
	}
}


void it1_enc_sigpass(iopj_t1_t *t1,	int bpno,	int orient,	int *nmsedec,	char type,	int cblksty)
{
	int i, j, k, one, vsc;
	*nmsedec = 0;
	one = 1 << (bpno + T1_NMSEDEC_FRACBITS);
	for (k = 0; k < t1->h; k += 4) {
		for (i = 0; i < t1->w; ++i) {
			for (j = k; j < k + 4 && j < t1->h; ++j) {
				vsc = ((cblksty & J2K_CCP_CBLKSTY_VSC) && (j == k + 3 || j == t1->h - 1)) ? 1 : 0;
				it1_enc_sigpass_step(
					t1,
					&t1->flags[((j+1) * t1->flags_stride) + i + 1],
					&t1->data[(j * t1->w) + i],
					orient,
					bpno,
					one,
					nmsedec,
					type,
					vsc);
			}
		}
	}
}



/** mod fixed_quality */
void it1_encode_cblk(	iopj_t1_t *t1,	opj_tcd_cblk_enc_t* cblk,	int orient,	int compno,	int level,
	int qmfbid,	double stepsize,	int cblksty,	int numcomps,	int mct,	opj_tcd_tile_t * tile)
{
	double cumwmsedec = 0.0;

	opj_mqc_t *mqc = t1->mqc;	/* MQC component */

	int passno, bpno, passtype;
	int nmsedec = 0;
	int i, max;
	char type = T1_TYPE_MQ;
	double tempwmsedec;

	max = 0;
	for (i = 0; i < t1->w * t1->h; ++i) 
	{
		int tmp = abs(t1->data[i]);
		max = int_max(max, tmp);
	}

	if (max)
		cblk->numbps = (int_floorlog2(max) + 1) - T1_NMSEDEC_FRACBITS ;
	else
		cblk->numbps = 0;

	bpno = cblk->numbps - 1;
	passtype = 2;

	mqc_resetstates(mqc);
	mqc_setstate(mqc, T1_CTXNO_UNI, 0, 46);
	mqc_setstate(mqc, T1_CTXNO_AGG, 0, 3);
	mqc_setstate(mqc, T1_CTXNO_ZC, 0, 4);
	mqc_init_enc(mqc, cblk->data);

	for (passno = 0; bpno >= 0; ++passno) 
	{
		opj_tcd_pass_t *pass = &cblk->passes[passno];
		int correction = 3;
		type = ((bpno < (cblk->numbps - 4)) && (passtype < 2) && (cblksty & J2K_CCP_CBLKSTY_LAZY)) ? T1_TYPE_RAW : T1_TYPE_MQ;

		switch (passtype) {
		case 0:
			it1_enc_sigpass(t1, bpno, orient, &nmsedec, type, cblksty);
			break;
		case 1:
			it1_enc_refpass(t1, bpno, &nmsedec, type, cblksty);
			break;
		case 2:
			it1_enc_clnpass(t1, bpno, orient, &nmsedec, cblksty);
			/* code switch SEGMARK (i.e. SEGSYM) */
			if (cblksty & J2K_CCP_CBLKSTY_SEGSYM)
				mqc_segmark_enc(mqc);
			break;
		}

		/* fixed_quality */
		tempwmsedec = it1_getwmsedec(nmsedec, compno, level, orient, bpno, qmfbid, stepsize, numcomps, mct);
		cumwmsedec += tempwmsedec;
		tile->distotile += tempwmsedec;

		/* Code switch "RESTART" (i.e. TERMALL) */
		pass->term = 0;
		//JUAN if ((cblksty & J2K_CCP_CBLKSTY_TERMALL)	&& !((passtype == 2) && (bpno - 1 < 0))) 
		
		if (++passtype == 3)
		{
			passtype = 0;
			bpno--;
		}

		//if (pass->term && bpno > 0) 		
		pass->distortiondec = cumwmsedec;
		pass->rate = mqc_numbytes(mqc) + correction;	// FIXME 

		// Code-switch "RESET" 
		if (cblksty & J2K_CCP_CBLKSTY_RESET)
			mqc_reset_enc(mqc);
	}

	if (!(cblksty & J2K_CCP_CBLKSTY_LAZY))
		mqc_flush(mqc);

	cblk->totalpasses = passno;

	for (passno = 0; passno<cblk->totalpasses; passno++) 
	{
		opj_tcd_pass_t *pass = &cblk->passes[passno];
		if (pass->rate > mqc_numbytes(mqc))
			pass->rate = mqc_numbytes(mqc);
		/*Preventing generation of FF as last data byte of a pass*/
		if((pass->rate>1) && (cblk->data[pass->rate - 1] == 0xFF))
		{
			pass->rate--;
		}

		if (passno == 0)
			pass->len = pass->rate - 0;
		else 
			pass->len = pass->rate - cblk->passes[passno - 1].rate;		
	}
}

void it1_encode_cblks(	iopj_t1_t *t1,	opj_tcd_tile_t *tile,	opj_tcp_t *tcp)
{
	int compno, resno, bandno, precno, cblkno;
	tile->distotile = 0;		/* fixed_quality */	
	for (compno = 0; compno < tile->numcomps; ++compno) 
	{
		opj_tcd_tilecomp_t* tilec = &tile->comps[compno];
		opj_tccp_t* tccp = &tcp->tccps[compno];
		int tile_w = tilec->x1 - tilec->x0;

		for (resno = 0; resno < tilec->numresolutions; ++resno) 
		{
			opj_tcd_resolution_t *res = &tilec->resolutions[resno];

			for (bandno = 0; bandno < res->numbands; ++bandno) 
			{
				opj_tcd_band_t* restrict band = &res->bands[bandno];
				int bandconst = 8192 * 8192 / ((int) floor(band->stepsize * 8192));

				for (precno = 0; precno < res->pw * res->ph; ++precno) 
				{
					opj_tcd_precinct_t *prc = &band->precincts[precno];

					for (cblkno = 0; cblkno < prc->cw * prc->ch; ++cblkno) 
					{
						opj_tcd_cblk_enc_t* cblk = &prc->cblks.enc[cblkno];
						int* restrict datap;
						cpixelType* restrict tiledp;
						int cblk_w;
						int cblk_h;
						int i, j;

						int x = cblk->x0 - band->x0;
						int y = cblk->y0 - band->y0;
						if (band->bandno & 1) 
						{
							opj_tcd_resolution_t *pres = &tilec->resolutions[resno - 1];
							x += pres->x1 - pres->x0;
						}
						if (band->bandno & 2) 
						{
							opj_tcd_resolution_t *pres = &tilec->resolutions[resno - 1];
							y += pres->y1 - pres->y0;
						}

						if(!iallocate_buffers(t1,cblk->x1 - cblk->x0,cblk->y1 - cblk->y0))
						{
							return;
						}

						datap=t1->data;
						cblk_w = t1->w;
						cblk_h = t1->h;

						tiledp=&tilec->data[(y * tile_w) + x];
						if (tccp->qmfbid == 1) 
						{
							for (j = 0; j < cblk_h; ++j) 
							{
								for (i = 0; i < cblk_w; ++i) 
								{
									int tmp = tiledp[(j * tile_w) + i];
									datap[(j * cblk_w) + i] = tmp << T1_NMSEDEC_FRACBITS;
								}
							}
						}
						/* if (tccp->qmfbid == 0) */						

						it1_encode_cblk(
							t1,
							cblk,
							band->bandno,
							compno,
							tilec->numresolutions - 1 - resno,
							tccp->qmfbid,
							band->stepsize,
							tccp->cblksty,
							tile->numcomps,
							tcp->mct,
							tile);

					} /* cblkno */
				} /* precno */
			} /* bandno */
		} /* resno  */
	} /* compno  */
}

#pragma endregion REGION_TIER1

void itgt_setvalue(iopj_tgt_tree_t *tree, int leafno, int value) 
{
	iopj_tgt_node_t *node;
	node = &tree->nodes[leafno];
	while (node && node->value > value) {
		node->value = value;
		node = node->parent;
	}
}

void itgt_encode(opj_bio_t *bio, opj_tgt_tree_t *tree, int leafno, int threshold) 
{
	opj_tgt_node_t *stk[31];
	opj_tgt_node_t **stkptr;
	opj_tgt_node_t *node;
	int low;

	stkptr = stk;
	node = &tree->nodes[leafno];
	while (node->parent) 
	{
		*stkptr++ = node;
		node = node->parent;
	}

	low = 0;
	for (;;) 
	{
		if (low > node->low) 
		{
			node->low = low;
		} 
		else 
		{
			low = node->low;
		}

		while (low < threshold) 
		{
			if (low >= node->value) 
			{
				if (!node->known) 
{
					bio_write(bio, 1, 1);
					node->known = 1;
				}
				break;
			}
			bio_write(bio, 0, 1);
			++low;
		}

		node->low = low;
		if (stkptr == stk)
			break;
		node = *--stkptr;
	}
}


#pragma region REGION_DWT
/* <summary>			                 */
/* Forward lazy transform (horizontal).  */
/* </summary>                            */ 
void idwt_deinterleave_h(int *a, int *b, int dn, int sn, int cas) 
{
	int i;
	for (i=0; i<sn; i++) b[i]=a[2*i+cas];
	for (i=0; i<dn; i++) b[sn+i]=a[(2*i+1-cas)];
}

/* <summary>                             */  
/* Forward lazy transform (vertical).    */
/* </summary>                            */ 
void idwt_deinterleave_v(int *a, int *b, int dn, int sn, int x, int cas) 
{
	int i;
	for (i=0; i<sn; i++) b[i*x]=a[2*i+cas];
	for (i=0; i<dn; i++) b[(sn+i)*x]=a[(2*i+1-cas)];
}

/* <summary>                             */
/* Inverse lazy transform (horizontal).  */
/* </summary>                            */
void idwt_interleave_h(idwt_t* h, int *a) 
{
	int *ai = a;
	int *bi = h->mem + h->cas;
	int  i	= h->sn;
	while( i-- ) 
	{
		*bi = *(ai++);
		bi += 2;
	}
	ai	= a + h->sn;
	bi	= h->mem + 1 - h->cas;
	i	= h->dn ;
	while( i-- ) 
	{
		*bi = *(ai++);
		bi += 2;
	}
}

/* <summary>                             */  
/* Inverse lazy transform (vertical).    */
/* </summary>                            */ 
void idwt_interleave_v(idwt_t* v, int *a, int x) 
{
	int *ai = a;
	int *bi = v->mem + v->cas;
	int  i = v->sn;
	while( i-- ) {
		*bi = *ai;
		bi += 2;
		ai += x;
	}
	ai = a + (v->sn * x);
	bi = v->mem + 1 - v->cas;
	i = v->dn ;
	while( i-- ) {
		*bi = *ai;
		bi += 2;  
		ai += x;
	}
}

/* <summary>                            */
/* Forward 5-3 wavelet transform in 1-D. */
/* </summary>                           */
void idwt_encode_1(int *a, int dn, int sn, int cas) 
{
	int i;

	if (!cas) {
		if ((dn > 0) || (sn > 1)) {	/* NEW :  CASE ONE ELEMENT */
			for (i = 0; i < dn; i++) D(i) -= (S_(i) + S_(i + 1)) >> 1;
			for (i = 0; i < sn; i++) S(i) += (D_(i - 1) + D_(i) + 2) >> 2;
		}
	} else {
		if (!sn && dn == 1)		    /* NEW :  CASE ONE ELEMENT */
			S(0) *= 2;
		else {
			for (i = 0; i < dn; i++) S(i) -= (DD_(i) + DD_(i - 1)) >> 1;
			for (i = 0; i < sn; i++) D(i) += (SS_(i) + SS_(i + 1) + 2) >> 2;
		}
	}
}

/* <summary>                            */
/* Forward 5-3 wavelet transform in 2-D. */
/* </summary>                           */
void idwt_encode(opj_tcd_tilecomp_t * tilec) 
{
	int i, j, k;
	cpixelType *a = NULL;
	int *aj = NULL;
	int *bj = NULL;
	int w, l;

	w = tilec->x1-tilec->x0;
	l = tilec->numresolutions-1;
	a = tilec->data;

	for (i = 0; i < l; i++) 
	{
		int rw;			/* width of the resolution level computed                                                           */
		int rh;			/* height of the resolution level computed                                                          */
		int rw1;		/* width of the resolution level once lower than computed one                                       */
		int rh1;		/* height of the resolution level once lower than computed one                                      */
		int cas_col;	/* 0 = non inversion on horizontal filtering 1 = inversion between low-pass and high-pass filtering */
		int cas_row;	/* 0 = non inversion on vertical filtering 1 = inversion between low-pass and high-pass filtering   */
		int dn, sn;

		rw = tilec->resolutions[l - i].x1 - tilec->resolutions[l - i].x0;
		rh = tilec->resolutions[l - i].y1 - tilec->resolutions[l - i].y0;
		rw1= tilec->resolutions[l - i - 1].x1 - tilec->resolutions[l - i - 1].x0;
		rh1= tilec->resolutions[l - i - 1].y1 - tilec->resolutions[l - i - 1].y0;

		cas_row = tilec->resolutions[l - i].x0 % 2;
		cas_col = tilec->resolutions[l - i].y0 % 2;

		sn = rh1;
		dn = rh - rh1;
		bj = (int*)sft_malloc(rh * sizeof(int));
		for (j = 0; j < rw; j++) 
		{
			aj = a + j;
			for (k = 0; k < rh; k++)  bj[k] = aj[k*w];
			idwt_encode_1(bj, dn, sn, cas_col);
			idwt_deinterleave_v(bj, aj, dn, sn, w, cas_col);
		}
		sft_free(bj);

		sn = rw1;
		dn = rw - rw1;
		bj = (int*)sft_malloc(rw * sizeof(int));
		for (j = 0; j < rh; j++) 
		{
			aj = a + j * w;
			for (k = 0; k < rw; k++)  bj[k] = aj[k];
			idwt_encode_1(bj, dn, sn, cas_row);
			idwt_deinterleave_h(bj, aj, dn, sn, cas_row);
		}
		sft_free(bj);
	}
}

#pragma endregion REGION_DWT


#pragma region REGION_TIER2
iopj_t2_t* it2_create(opj_common_ptr cinfo, opj_image_t *image, opj_cp_t *cp)
{
	/* create the tcd structure */
	iopj_t2_t *t2 = (iopj_t2_t*)sft_malloc(sizeof(iopj_t2_t));
	if(!t2) return NULL;
	t2->cinfo = cinfo;
	t2->image = image;
	t2->cp = cp;

	return t2;
}

void it2_destroy(iopj_t2_t *t2) 
{
	if(t2) {
		sft_free(t2);
	}
}


void it2_putcommacode(opj_bio_t *bio, int n) 
{
	while (--n >= 0) {
		bio_write(bio, 1, 1);
	}
	bio_write(bio, 0, 1);
}

void it2_putnumpasses(opj_bio_t *bio, int n) {
	if (n == 1) {
		bio_write(bio, 0, 1);
	} else if (n == 2) {
		bio_write(bio, 2, 2);
	} else if (n <= 5) {
		bio_write(bio, 0xc | (n - 3), 4);
	} else if (n <= 36) {
		bio_write(bio, 0x1e0 | (n - 6), 9);
	} else if (n <= 164) {
		bio_write(bio, 0xff80 | (n - 37), 16);
	}
}

int it2_encode_packet(opj_tcd_tile_t * tile, opj_tcp_t * tcp, iopj_pi_iterator_t *pi, unsigned char *dest, int length, opj_codestream_info_t *cstr_info, int tileno) 
{
	int bandno, cblkno;
	unsigned char *c = dest;

	int compno = pi->compno;	/* component value */
	int resno  = pi->resno;		/* resolution level value */
	int precno = pi->precno;	/* precinct value */
	int layno  = pi->layno;		/* quality layer value */

	opj_tcd_tilecomp_t *tilec = &tile->comps[compno];
	opj_tcd_resolution_t *res = &tilec->resolutions[resno];

	opj_bio_t *bio = NULL;	/* BIO component */

	/* <SOP 0xff91> */
	if (tcp->csty & J2K_CP_CSTY_SOP) 
	{
		c[0] = 255;
		c[1] = 145;
		c[2] = 0;
		c[3] = 4;
		c[4] = (unsigned char)((tile->packno % 65536) / 256);
		c[5] = (unsigned char)((tile->packno % 65536) % 256);
		c += 6;
	}
	/* </SOP> */

	if (!layno) 
	{
		for (bandno = 0; bandno < res->numbands; bandno++) 
		{
			opj_tcd_band_t *band = &res->bands[bandno];
			opj_tcd_precinct_t *prc = &band->precincts[precno];
			itgt_reset(prc->incltree);
			itgt_reset(prc->imsbtree);
			for (cblkno = 0; cblkno < prc->cw * prc->ch; cblkno++) 
			{
				opj_tcd_cblk_enc_t* cblk = &prc->cblks.enc[cblkno];
				cblk->numpasses = 0;
				itgt_setvalue((iopj_tgt_tree_t*)(prc->imsbtree), cblkno, band->numbps - cblk->numbps);
			}
		}
	}

	bio = bio_create();
	bio_init_enc(bio, c, length);
	bio_write(bio, 1, 1);		/* Empty header bit */

	/* Writing Packet header */
	for (bandno = 0; bandno < res->numbands; bandno++) 
	{
		opj_tcd_band_t *band = &res->bands[bandno];
		opj_tcd_precinct_t *prc = &band->precincts[precno];
		for (cblkno = 0; cblkno < prc->cw * prc->ch; cblkno++) 
		{
			opj_tcd_cblk_enc_t* cblk = &prc->cblks.enc[cblkno];
			opj_tcd_layer_t *layer = &cblk->layers[layno];
			if (!cblk->numpasses && layer->numpasses) 
			{
				itgt_setvalue((iopj_tgt_tree_t*)(prc->incltree), cblkno, layno);
			}
		}
		for (cblkno = 0; cblkno < prc->cw * prc->ch; cblkno++) 
		{
			opj_tcd_cblk_enc_t* cblk = &prc->cblks.enc[cblkno];
			opj_tcd_layer_t *layer = &cblk->layers[layno];
			int increment = 0;
			int nump = 0;
			int len = 0, passno;
			/* cblk inclusion bits */
			if (!cblk->numpasses) 
			{
				itgt_encode(bio, prc->incltree, cblkno, layno + 1);
			} 
			else 
			{
				bio_write(bio, layer->numpasses != 0, 1);
			}
			/* if cblk not included, go to the next cblk  */
			if (!layer->numpasses) 
			{
				continue;
			}
			/* if first instance of cblk --> zero bit-planes information */
			if (!cblk->numpasses) 
			{
				cblk->numlenbits = 3;
				itgt_encode(bio, prc->imsbtree, cblkno, 999);
			}
			/* number of coding passes included */
			it2_putnumpasses(bio, layer->numpasses);

			/* computation of the increase of the length indicator and insertion in the header     */
			for (passno = cblk->numpasses; passno < cblk->numpasses + layer->numpasses; passno++) 
			{
				opj_tcd_pass_t *pass = &cblk->passes[passno];
				nump++;
				len += pass->len;
				if (pass->term || passno == (cblk->numpasses + layer->numpasses) - 1) 
				{
					increment = int_max(increment, int_floorlog2(len) + 1 - (cblk->numlenbits + int_floorlog2(nump)));
					len = 0;
					nump = 0;
				}
			}
			it2_putcommacode(bio, increment);

			/* computation of the new Length indicator */
			cblk->numlenbits += increment;

			/* insertion of the codeword segment length */
			for (passno = cblk->numpasses; passno < cblk->numpasses + layer->numpasses; passno++)
			{
				opj_tcd_pass_t *pass = &cblk->passes[passno];
				nump++;
				len += pass->len;
				if (pass->term || passno == (cblk->numpasses + layer->numpasses) - 1) 
				{
					bio_write(bio, len, cblk->numlenbits + int_floorlog2(nump));
					len = 0;
					nump = 0;
				}
			}
		}
	}

	if (bio_flush(bio)) 
	{
		bio_destroy(bio);
		return -999;		/* modified to eliminate longjmp !! */
	}

	c += bio_numbytes(bio);
	bio_destroy(bio);

	/* <EPH 0xff92> */
	if (tcp->csty & J2K_CP_CSTY_EPH) 
	{
		c[0] = 255;
		c[1] = 146;
		c += 2;
	}
	/* </EPH> */

	/* << INDEX */
	/* End of packet header position. Currently only represents the distance to start of packet
	// Will be updated later by incrementing with packet start value */
	if(cstr_info && cstr_info->index_write) 
	{
		opj_packet_info_t *info_PK = &cstr_info->tile[tileno].packet[cstr_info->packno];
		info_PK->end_ph_pos = (int)(c - dest);
	}
	/* INDEX >> */

	/* Writing the packet body */

	for (bandno = 0; bandno < res->numbands; bandno++) 
	{
		opj_tcd_band_t *band = &res->bands[bandno];
		opj_tcd_precinct_t *prc = &band->precincts[precno];
		for (cblkno = 0; cblkno < prc->cw * prc->ch; cblkno++) 
		{
			opj_tcd_cblk_enc_t* cblk = &prc->cblks.enc[cblkno];
			opj_tcd_layer_t *layer = &cblk->layers[layno];
			if (!layer->numpasses) 
			{
				continue;
			}
			if (c + layer->len > dest + length) 
			{
				return -999;
			}
			
			sft_memCopy(c, layer->data, layer->len);
			cblk->numpasses += layer->numpasses;
			c += layer->len;
			/* << INDEX */ 			
			/* INDEX >> */
		}
	}

	return (c - dest);
}

int it2_encode_packets(iopj_t2_t* t2,int tileno, opj_tcd_tile_t *tile, int maxlayers, unsigned char *dest, int len, opj_codestream_info_t *cstr_info,int tpnum, int tppos,int pino, J2K_T2_MODE t2_mode, int cur_totnum_tp)
{
	unsigned char *c = dest;
	int e = 0;

	iopj_pi_iterator_t *pi = NULL;

	opj_image_t *image = t2->image;
	opj_cp_t *cp = t2->cp;
	opj_tcp_t *tcp = &cp->tcps[tileno];
	int pocno = cp->cinema == CINEMA4K_24? 2: 1;
	int maxcomp = cp->max_comp_size > 0 ? image->numcomps : 1;

	pi = pi_initialise_encode(image, cp, tileno, t2_mode);
	if(!pi) {
		/* TODO: throw an error */
		return -999;
	}

	if(t2_mode == THRESH_CALC )
	{ /* Calculating threshold */		
	}
	else{  /* t2_mode == FINAL_PASS  */
		pi_create_encode(pi, cp,tileno,pino,tpnum,tppos,t2_mode,cur_totnum_tp);
		// JUAN : Only LRCP encoding
		while (pi_next(&pi[pino])) 		
		{
			if (pi[pino].layno < maxlayers) 
			{
				e = it2_encode_packet(tile, &cp->tcps[tileno], &pi[pino], c, dest + len - c, cstr_info, tileno);
				if (e == -999) {
					break;
				} else {
					c += e;
				}
				/* INDEX >> */
				if(cstr_info) {					
				}
				/* << INDEX */
				tile->packno++;
			}
		}
	}

	pi_destroy(pi, cp, tileno);

	if (e == -999) {
		return e;
	}

	return (c - dest);
}

#pragma endregion REGION_TIER2

void itcd_makelayer_fixed(opj_tcd_t *tcd, int layno, int final) 
{
	return ;
}

void itcd_rateallocate_fixed(opj_tcd_t *tcd) 
{
	int layno;
	for (layno = 0; layno < tcd->tcp->numlayers; layno++) 
	{
		itcd_makelayer_fixed(tcd, layno, 1);
	}
}

void itcd_makelayer(opj_tcd_tile_t *tcd_tile, int layno, double thresh, int final) 
{
	int compno, resno, bandno, precno, cblkno, passno;
	tcd_tile->distolayer[layno] = 0;	/* fixed_quality */

	for (compno = 0; compno < tcd_tile->numcomps; compno++) 
	{
		opj_tcd_tilecomp_t *tilec = &tcd_tile->comps[compno];
		for (resno = 0; resno < tilec->numresolutions; resno++) 
		{
			opj_tcd_resolution_t *res = &tilec->resolutions[resno];
			for (bandno = 0; bandno < res->numbands; bandno++) 
			{
				opj_tcd_band_t *band = &res->bands[bandno];
				for (precno = 0; precno < res->pw * res->ph; precno++) 
				{
					opj_tcd_precinct_t *prc = &band->precincts[precno];
					for (cblkno = 0; cblkno < prc->cw * prc->ch; cblkno++) 
					{
						opj_tcd_cblk_enc_t *cblk = &prc->cblks.enc[cblkno];
						opj_tcd_layer_t *layer = &cblk->layers[layno];

						int n;
						if (layno == 0) 
						{
							cblk->numpassesinlayers = 0;
						}
						n = cblk->numpassesinlayers;
						for (passno = cblk->numpassesinlayers; passno < cblk->totalpasses; passno++) 
						{
							int dr;
							double dd;
							opj_tcd_pass_t *pass = &cblk->passes[passno];
							if (n == 0) 
							{
								dr = pass->rate;
								dd = pass->distortiondec;
							} 
							else 
							{
								dr = pass->rate - cblk->passes[n - 1].rate;
								dd = pass->distortiondec - cblk->passes[n - 1].distortiondec;
							}
							if (!dr) 
							{
								if (dd != 0)
									n = passno + 1;
								continue;
							}
							if (dd / dr >= thresh)
								n = passno + 1;
						}
						layer->numpasses = n - cblk->numpassesinlayers;

						if (!layer->numpasses) 
						{
							layer->disto = 0;
							continue;
						}
						if (cblk->numpassesinlayers == 0) 
						{
							layer->len = cblk->passes[n - 1].rate;
							layer->data = cblk->data;
							layer->disto = cblk->passes[n - 1].distortiondec;
						} 
						else 
						{
							layer->len = cblk->passes[n - 1].rate -	cblk->passes[cblk->numpassesinlayers - 1].rate;
							layer->data = cblk->data + cblk->passes[cblk->numpassesinlayers - 1].rate;
							layer->disto = cblk->passes[n - 1].distortiondec - cblk->passes[cblk->numpassesinlayers - 1].distortiondec;
						}

						tcd_tile->distolayer[layno] += layer->disto;	/* fixed_quality */

						if (final)
							cblk->numpassesinlayers = n;
					}
				}
			}
		}
	}
}

opj_bool itcd_rateallocate(opj_tcd_tile_t *tcd_tile, const opj_tcd_t *tcd,int len, opj_codestream_info_t *cstr_info) 
{
	int compno, resno, bandno, precno, cblkno, passno, layno;
	double min, max;
	double cumdisto[100];	/* fixed_quality */
	const double K = 1;		/* 1.1; fixed_quality */
	double maxSE = 0;

	opj_cp_t *cp = tcd->cp;	 
	opj_tcp_t *tcd_tcp = tcd->tcp;

	min = 5000000000000;
	max = 0;

	tcd_tile->numpix = 0;		/* fixed_quality */

	for (compno = 0; compno < tcd_tile->numcomps; compno++) 
	{
		opj_tcd_tilecomp_t *tilec = &tcd_tile->comps[compno];
		tilec->numpix = 0;

		for (resno = 0; resno < tilec->numresolutions; resno++) 
		{
			opj_tcd_resolution_t *res = &tilec->resolutions[resno];

			for (bandno = 0; bandno < res->numbands; bandno++) 
			{
				opj_tcd_band_t *band = &res->bands[bandno];

				for (precno = 0; precno < res->pw * res->ph; precno++) 
				{
					opj_tcd_precinct_t *prc = &band->precincts[precno];

					for (cblkno = 0; cblkno < prc->cw * prc->ch; cblkno++) 
					{
						opj_tcd_cblk_enc_t *cblk = &prc->cblks.enc[cblkno];

						for (passno = 0; passno < cblk->totalpasses; passno++) 
						{
							opj_tcd_pass_t *pass = &cblk->passes[passno];
							int dr;
							double dd, rdslope;
							if (passno == 0) 
							{
								dr = pass->rate;
								dd = pass->distortiondec;
							} 
							else 
							{
								dr = pass->rate - cblk->passes[passno - 1].rate;
								dd = pass->distortiondec - cblk->passes[passno - 1].distortiondec;
							}
							if (dr == 0) 
							{
								continue;
							}
							rdslope = dd / dr;
							if (rdslope < min) 
							{
								min = rdslope;
							}
							if (rdslope > max) 
							{
								max = rdslope;
							}
						} /* passno */

						/* fixed_quality */
						tcd_tile->numpix += ((cblk->x1 - cblk->x0) * (cblk->y1 - cblk->y0));
						tilec->numpix += ((cblk->x1 - cblk->x0) * (cblk->y1 - cblk->y0));
					} /* cbklno */
				} /* precno */
			} /* bandno */
		} /* resno */

		maxSE += (((double)(1 << tcd->image->comps[compno].prec) - 1.0) 
			* ((double)(1 << tcd->image->comps[compno].prec) -1.0)) 
			* ((double)(tilec->numpix));
	} /* compno */

	/* index file */	
	for (layno = 0; layno < tcd_tcp->numlayers; layno++) 
	{
		double lo = min;
		double hi = max;
		int success = 0;
		int maxlen = tcd_tcp->rates[layno] ? int_min(((int) ceil(tcd_tcp->rates[layno])), len) : len;
		double goodthresh = 0;
		double stable_thresh = 0;
		
		double distotarget;		/* fixed_quality */

		/* fixed_quality */
		distotarget = tcd_tile->distotile - ((K * maxSE) / pow((float)10, tcd_tcp->distoratio[layno] / 10));

		/* Don't try to find an optimal threshold but rather take everything not included yet, if
		-r xx,yy,zz,0   (disto_alloc == 1 and rates == 0)
		-q xx,yy,zz,0	  (fixed_quality == 1 and distoratio == 0)
		==> possible to have some lossy layers and the last layer for sure lossless */
		
		// Omitted
		//if ( ((cp->disto_alloc==1) && (tcd_tcp->rates[layno]>0)) || ((cp->fixed_quality==1) && (tcd_tcp->distoratio[layno]>0))) 
		success = 1;
		goodthresh = min;

		/* Threshold for Marcela Index */		
		itcd_makelayer(tcd->tcd_tile, layno, goodthresh, 1);

		/* fixed_quality */
		if (layno == 0) 
			cumdisto[layno] = tcd_tile->distolayer[0] ;
		else
			cumdisto[layno] = (cumdisto[layno - 1] + tcd_tile->distolayer[layno]);	
	}

	return OPJ_TRUE;
}


int itcd_encode_tile(const opj_j2k_t *j2k ,  const opj_tcd_t *tcd,const int tileno,unsigned char *dest,const int len, opj_codestream_info_t *cstr_info) 
{
	int compno;
	int l,  numpacks = 0;
	opj_tcd_tile_t *tile = NULL;
	opj_tcp_t *tcd_tcp = NULL;
	opj_cp_t *cp = NULL;

	opj_tcp_t *tcp = &tcd->cp->tcps[0];
	opj_tccp_t *tccp = &tcp->tccps[0];
	opj_image_t *image = tcd->image;

	iopj_t1_t *t1 = NULL;		/* T1 component */
	iopj_t2_t *t2 = NULL;		/* T2 component */

	tile = tcd->tcd_image->tiles;
	tcd_tcp = &tcd->cp->tcps[tileno];
	cp = tcd->cp;

	if(tcd->cur_tp_num == 0)
	{
		/* INDEX >> "Precinct_nb_X et Precinct_nb_Y" */
		/* << INDEX */
		/*---------------TILE-------------------*/		
		for (compno = 0; compno < tile->numcomps; compno++) 
		{
			int x, y;
			int adjust , offset_x , offset_y, tw , w;
			opj_tcd_tilecomp_t *tilec;

			if ( image->comps[compno].sgnd  )
				adjust = 0;
			else
				adjust = 1 << (image->comps[compno].prec - 1);

			offset_x = int_ceildiv(image->x0, image->comps[compno].dx);
			offset_y = int_ceildiv(image->y0, image->comps[compno].dy);

			tilec = &tile->comps[compno];
			tw = tilec->x1 - tilec->x0;
			w = int_ceildiv(image->x1 - image->x0, image->comps[compno].dx);		
			/* extract tile data */			
			if (tcd_tcp->tccps[compno].qmfbid == 1) 
			{
				for (y = tilec->y0; y < tilec->y1; y++) 
				{
					/* start of the src tile scanline */
					int *data = &image->comps[compno].data[(tilec->x0 - offset_x) + (y - offset_y) * w];
					/* start of the dst tile scanline */
					cpixelType *tile_data = &tilec->data[(y - tilec->y0) * tw];
					for (x = tilec->x0; x < tilec->x1; x++) 
					{
						*tile_data++ = *data++ - adjust;
					}
				}
			}// else if (tcd_tcp->tccps[compno].qmfbid == 0) 			
		}

		/*----------------MCT-------------------*/
		if (tcd_tcp->mct) 
		{
			int samples = (tile->comps[0].x1 - tile->comps[0].x0) * (tile->comps[0].y1 - tile->comps[0].y0);			
			//mct_encode(tile->comps[0].data, tile->comps[1].data, tile->comps[2].data, samples);
			/* <summary> */
			/* Foward reversible MCT. */
			/* </summary> */
			int* c0 = tile->comps[0].data ; 
			int* c1 = tile->comps[1].data ; 
			int* c2 = tile->comps[2].data ; 
			int n = samples ;
			int i;				
			for(i = 0; i < n; ++i) 
			{
				int r = c0[i];
				int g = c1[i];
				int b = c2[i];
				int y = (r + (g * 2) + b) >> 2;
				int u = b - g;
				int v = r - g;
				c0[i] = y;
				c1[i] = u;
				c2[i] = v;
			}

		}

		/*----------------DWT---------------------*/
		for (compno = 0; compno < tile->numcomps; compno++) 
		{
			opj_tcd_tilecomp_t *tilec = &tile->comps[compno];
			//Compresion sin perdida
			//if (tcd_tcp->tccps[compno].qmfbid == 1) {
			idwt_encode(tilec);			
		}

		/*------------------TIER1-----------------*/
		t1 = it1_create(tcd->cinfo);
		it1_encode_cblks(t1, tile, tcd_tcp);
		//t1_destroy(t1);

		/*-----------RATE-ALLOCATE------------------*/
		/* INDEX */
		/* Normal Rate/distortion allocation */
		itcd_rateallocate(tcd->tcd_tile , tcd,  len, cstr_info);
	}
	/*--------------TIER2------------------*/
	/* INDEX */
	t2 = it2_create(tcd->cinfo, image, cp);
	l = it2_encode_packets(t2,tileno, tile, tcd_tcp->numlayers, dest, len, cstr_info,
		tcd->tp_num,tcd->tp_pos,tcd->cur_pino,FINAL_PASS,
		j2k->cur_totnum_tp[tileno]);
	it2_destroy(t2);

	/*---------------CLEAN-------------------*/	
	if(tcd->cur_tp_num == j2k->cur_totnum_tp[tileno] - 1)
	{
		/* cleaning memory */
		for (compno = 0; compno < tile->numcomps; compno++) 
		{
			opj_tcd_tilecomp_t *tilec = &tile->comps[compno];
			sft_free(tilec->data);
		}
	}
	return l;
}

int ij2k_encode_Tile(opj_cp_t *cp, opj_j2k_t *j2k, void *tile_coder, int tileno, int tp_num , int cur_tp_num)
{

	opj_tcp_t *tcp = NULL;
	opj_codestream_info_t *cstr_info = NULL;
	int curtileno = tileno;
	int l;
	int i = 0;
	int layno = 0;

	opj_tcd_t *tcd = (opj_tcd_t*)tile_coder;	/* cast is needed because of conflicts in header inclusions */
	//opj_cp_t *cp = j2k->cp;

	/* INDEX >> */	
	/* << INDEX */	
	tcp = &cp->tcps[tileno];
	for (layno = 0; layno < tcp->numlayers; layno++) 
	{
		if (tcp->rates[layno]>(j2k->sod_start / (cp->th * cp->tw))) 
		{
			tcp->rates[layno]-=(j2k->sod_start / (cp->th * cp->tw));
		} 
		else if (tcp->rates[layno]) 
		{
			tcp->rates[layno]=1;
		}
	}

	if(cur_tp_num == 0)
	{
		tcd->tcd_image->tiles->packno = 0;		
	}


	tcd->packetData = (unsigned char*)sft_malloc(28000* sizeof(unsigned char));
	l = itcd_encode_tile(j2k, tcd, tileno, tcd->packetData, 28000, cstr_info);
	tcd->dataLength = l;

	totalMemPerTile += l;

	return l;
}




/**
Create a new TCD handle
*/
opj_tcd_t* itcd_create(opj_common_ptr cinfo) 
{
	/* create the tcd structure */
	opj_tcd_t *tcd = (opj_tcd_t*)sft_malloc(sizeof(opj_tcd_t));
	if(!tcd) return NULL;
	tcd->cinfo = cinfo;
	tcd->tcd_image = (opj_tcd_image_t*)sft_malloc(sizeof(opj_tcd_image_t));
	if(!tcd->tcd_image) 
	{
		sft_free(tcd);
		return NULL;
	}

	return tcd;
}

/**
Destroy a previously created TCD handle
*/
void itcd_destroy(opj_tcd_t *tcd)
{
	if(tcd) 
	{
		sft_free(tcd->tcd_image);
		sft_free(tcd);
	}
}

void itcd_free_encode(opj_tcd_t *tcd)
{
	int tileno, compno, resno, bandno, precno, cblkno;

	for (tileno = 0; tileno < 1; tileno++) {
		opj_tcd_tile_t *tile = tcd->tcd_image->tiles;

		for (compno = 0; compno < tile->numcomps; compno++) 
		{
			opj_tcd_tilecomp_t *tilec = &tile->comps[compno];

			for (resno = 0; resno < tilec->numresolutions; resno++) 
			{
				opj_tcd_resolution_t *res = &tilec->resolutions[resno];

				for (bandno = 0; bandno < res->numbands; bandno++) {
					opj_tcd_band_t *band = &res->bands[bandno];

					for (precno = 0; precno < res->pw * res->ph; precno++) 
					{
						opj_tcd_precinct_t *prc = &band->precincts[precno];

						if (prc->incltree != NULL) 
						{
							itgt_destroy(prc->incltree);
							prc->incltree = NULL;
						}
						if (prc->imsbtree != NULL) 
						{
							itgt_destroy(prc->imsbtree);	
							prc->imsbtree = NULL;
						}
						for (cblkno = 0; cblkno < prc->cw * prc->ch; cblkno++) 
						{
							sft_free(prc->cblks.enc[cblkno].data - 2);
							sft_free(prc->cblks.enc[cblkno].layers);
							sft_free(prc->cblks.enc[cblkno].passes);
						}
						sft_free(prc->cblks.enc);
					} /* for (precno */
					sft_free(band->precincts);
					band->precincts = NULL;
				} /* for (bandno */
			} /* for (resno */
			sft_free(tilec->resolutions);
			tilec->resolutions = NULL;
		} /* for (compno */
		sft_free(tile->comps);
		tile->comps = NULL;
	} /* for (tileno */
	sft_free(tcd->tcd_image->tiles);
	tcd->tcd_image->tiles = NULL;
}

/* <summary>                          */
/* Get gain of 5-3 wavelet transform. */
/* </summary>                         */
int idwt_getgain(int orient) 
{
	if (orient == 0)
		return 0;
	if (orient == 1 || orient == 2)
		return 1;
	return 2;
}


/* 
==========================================================
Tag-tree coder interface
==========================================================
*/
opj_tgt_tree_t *itgt_create(int numleafsh, int numleafsv)
{
	int nplh[32];
	int nplv[32];
	opj_tgt_node_t *node = NULL;
	opj_tgt_node_t *parentnode = NULL;
	opj_tgt_node_t *parentnode0 = NULL;
	opj_tgt_tree_t *tree = NULL;
	int i, j, k;
	int numlvls;
	int n;

	tree = (opj_tgt_tree_t *) sft_malloc(sizeof(opj_tgt_tree_t));
	if(!tree) return NULL;
	tree->numleafsh = numleafsh;
	tree->numleafsv = numleafsv;

	numlvls = 0;
	nplh[0] = numleafsh;
	nplv[0] = numleafsv;
	tree->numnodes = 0;
	do {
		n = nplh[numlvls] * nplv[numlvls];
		nplh[numlvls + 1] = (nplh[numlvls] + 1) / 2;
		nplv[numlvls + 1] = (nplv[numlvls] + 1) / 2;
		tree->numnodes += n;
		++numlvls;
	} while (n > 1);

	/* ADD */
	if (tree->numnodes == 0) 
	{
		sft_free(tree);
		return NULL;
	}

	tree->nodes = (opj_tgt_node_t*) sft_calloc(tree->numnodes, sizeof(opj_tgt_node_t));
	if(!tree->nodes) 
	{
		sft_free(tree);
		return NULL;
	}

	node = tree->nodes;
	parentnode = &tree->nodes[tree->numleafsh * tree->numleafsv];
	parentnode0 = parentnode;

	for (i = 0; i < numlvls - 1; ++i) 
	{
		for (j = 0; j < nplv[i]; ++j) 
		{
			k = nplh[i];
			while (--k >= 0) 
			{
				node->parent = parentnode;
				++node;
				if (--k >= 0) 
				{
					node->parent = parentnode;
					++node;
				}
				++parentnode;
			}
			if ((j & 1) || j == nplv[i] - 1) 
			{
				parentnode0 = parentnode;
			} 
			else 
			{
				parentnode = parentnode0;
				parentnode0 += nplh[i];
			}
		}
	}
	node->parent = 0;

	itgt_reset(tree);

	return tree;
}

void itgt_destroy(opj_tgt_tree_t *tree) 
{
	sft_free(tree->nodes);
	sft_free(tree);
}

/* 
==========================================================
Tag-tree coder interface
==========================================================
*/
void itgt_reset(opj_tgt_tree_t *tree) 
{
	int i;

	if (NULL == tree)
		return;

	for (i = 0; i < tree->numnodes; i++) 
	{
		tree->nodes[i].value = 999;
		tree->nodes[i].low = 0;
		tree->nodes[i].known = 0;
	}
}

void ij2k_convertToYUV(const opj_j2k_t *j2k ,  const opj_tcd_t *tcd,unsigned char *dest,const int len, opj_codestream_info_t *cstr_info) 
{
	int  tileno;
	int  numpacks = 0;
	opj_tcd_tile_t *tile = NULL;
	opj_tcp_t *tcd_tcp = NULL;
	opj_cp_t *cp = NULL;

	opj_tcp_t *tcp = &tcd->cp->tcps[0];
	opj_tccp_t *tccp = &tcp->tccps[0];
	opj_image_t *image = tcd->image;

	iopj_t1_t *t1 = NULL;		/* T1 component */
	iopj_t2_t *t2 = NULL;		/* T2 component */

	for (tileno = 0 ; tileno< tcd->tcd_image->th *  tcd->tcd_image->tw ; tileno++)
	{
		tile = tcd->tcd_image->tiles;
		tcd_tcp = &tcd->cp->tcps[tileno];
		cp = tcd->cp;

		tcd_tcp->YUV = 1;


		/* INDEX >> "Precinct_nb_X et Precinct_nb_Y" */
		/* << INDEX */
		/*----------------MCT-------------------*/
		if (tcd_tcp->mct) 
		{
			int samples = (tile->comps[0].x1 - tile->comps[0].x0) * (tile->comps[0].y1 - tile->comps[0].y0);			
			//mct_encode(tile->comps[0].data, tile->comps[1].data, tile->comps[2].data, samples);
			/* <summary> */
			/* Foward reversible MCT. */
			/* </summary> */
			int* c0 = tile->comps[0].data ; 
			int* c1 = tile->comps[1].data ; 
			int* c2 = tile->comps[2].data ; 
			int n = samples ;
			int i;				
			for(i = 0; i < n; ++i) 
			{
				int r = c0[i];
				int g = c1[i];
				int b = c2[i];
				int y = (r + (g * 2) + b) >> 2;
				int u = b - g;
				int v = r - g;
				c0[i] = y;
				c1[i] = u;
				c2[i] = v;
			}

		}
	}

}

/* ----------------------------------------------------------------------- */


void itcd_malloc_encode(opj_j2k_t *j2k, opj_tcd_t *tcd,const opj_image_t * image, opj_cp_t * cp, int curtileno)
{
	int tileno, compno, resno, bandno, precno, cblkno;

	opj_tcp_t *tcp = &cp->tcps[curtileno];
	int j;
	opj_tcd_tile_t *tile ;
	/* cfr p59 ISO/IEC FDIS15444-1 : 2000 (18 august 2000) */
	int p = curtileno % cp->tw;	/* si numerotation matricielle .. */
	int q = curtileno / cp->tw;	/* .. coordonnees de la tile (q,p) q pour ligne et p pour colonne */

	/* opj_tcd_tile_t *tile=&tcd->tcd_image->tiles[tileno]; */


	tcd->tcd_image->tiles = (opj_tcd_tile_t *) sft_malloc(sizeof(opj_tcd_tile_t));
	tile = tcd->tcd_image->tiles;
	//for (tileno = 0; tileno < 1; tileno++) 
	tileno = 0;
	//tile->distolayer = (float *) sft_malloc(sizeof(float));

	/* 4 borders of the tile rescale on the image if necessary */
	tile->x0 = int_max(cp->tx0 + p * cp->tdx, image->x0);
	tile->y0 = int_max(cp->ty0 + q * cp->tdy, image->y0);
	tile->x1 = int_min(cp->tx0 + (p + 1) * cp->tdx, image->x1);
	tile->y1 = int_min(cp->ty0 + (q + 1) * cp->tdy, image->y1);
	tile->numcomps = image->numcomps;
	/* tile->PPT=image->PPT;  */
	tile->comps = (opj_tcd_tilecomp_t *) sft_malloc(image->numcomps * sizeof(opj_tcd_tilecomp_t));

	/* Modification of the RATE >> */
	for (j = 0; j < tcp->numlayers; j++) 
	{
		if (tcp->rates[j]) 
		{
			if (cp->tp_on)
				tcp->rates[j] =	(((float) (tile->numcomps* (tile->x1 - tile->x0) * (tile->y1 - tile->y0)* image->comps[0].prec))/(tcp->rates[j] * 8 * image->comps[0].dx * image->comps[0].dy)) - (((/*tcd->cur_totnum_tp*/j2k->cur_totnum_tp[tileno]  - 1) * 14 )/ tcp->numlayers);
			else
				tcp->rates[j] = ((float) (tile->numcomps * (tile->x1 - tile->x0) * (tile->y1 - tile->y0)* image->comps[0].prec))/ (tcp->rates[j] * 8 * image->comps[0].dx * image->comps[0].dy);
		}
		else
			tcp->rates[j] = 0.0f;

		if (tcp->rates[j]) 
		{
			if (j && tcp->rates[j] < tcp->rates[j - 1] + 10) 
			{
				tcp->rates[j] = tcp->rates[j - 1] + 20;
			} 
			else 
			{
				if (!j && tcp->rates[j] < 30)
					tcp->rates[j] = 30;
			}

			if(j == (tcp->numlayers-1))
			{
				tcp->rates[j] = tcp->rates[j]- 2;
			}
		}
	}
	/* << Modification of the RATE */				

	//Asignando memoria
	for (compno = 0; compno < tile->numcomps; compno++) 
	{
		opj_tccp_t *tccp = &tcp->tccps[compno];
		opj_tcd_tilecomp_t *tilec = &tile->comps[compno];

		/* border of each tile component (global) */
		tilec->x0 = int_ceildiv(tile->x0, image->comps[compno].dx);
		tilec->y0 = int_ceildiv(tile->y0, image->comps[compno].dy);
		tilec->x1 = int_ceildiv(tile->x1, image->comps[compno].dx);
		tilec->y1 = int_ceildiv(tile->y1, image->comps[compno].dy);

		tilec->data = (cpixelType *) sft_aligned_malloc((tilec->x1 - tilec->x0) * (tilec->y1 - tilec->y0) * sizeof(cpixelType));
		tilec->numresolutions = tccp->numresolutions;
		tilec->resolutions = (opj_tcd_resolution_t *) sft_malloc(tilec->numresolutions * sizeof(opj_tcd_resolution_t));
	}

	for (compno = 0; compno < tile->numcomps; compno++) 
	{
		opj_tccp_t *tccp = &tcp->tccps[compno];
		opj_tcd_tilecomp_t *tilec = &tile->comps[compno];

		for (resno = 0; resno < tilec->numresolutions; resno++) 
		{
			int pdx, pdy;
			int levelno = tilec->numresolutions - 1 - resno;
			int tlprcxstart, tlprcystart, brprcxend, brprcyend;
			int tlcbgxstart, tlcbgystart, brcbgxend, brcbgyend;
			int cbgwidthexpn, cbgheightexpn;
			int cblkwidthexpn, cblkheightexpn;

			opj_tcd_resolution_t *res = &tilec->resolutions[resno];

			/* border for each resolution level (global) */
			res->x0 = int_ceildivpow2(tilec->x0, levelno);
			res->y0 = int_ceildivpow2(tilec->y0, levelno);
			res->x1 = int_ceildivpow2(tilec->x1, levelno);
			res->y1 = int_ceildivpow2(tilec->y1, levelno);

			if (resno == 0 )
				res->numbands = 1;
			else
				res->numbands = 3;

			/* p. 35, table A-23, ISO/IEC FDIS154444-1 : 2000 (18 august 2000) */
			if (tccp->csty & J2K_CCP_CSTY_PRT) 
			{
				pdx = tccp->prcw[resno];
				pdy = tccp->prch[resno];
			} else 
			{
				pdx = 15;
				pdy = 15;
			}
			/* p. 64, B.6, ISO/IEC FDIS15444-1 : 2000 (18 august 2000)  */
			tlprcxstart = int_floordivpow2(res->x0, pdx) << pdx;
			tlprcystart = int_floordivpow2(res->y0, pdy) << pdy;

			brprcxend = int_ceildivpow2(res->x1, pdx) << pdx;
			brprcyend = int_ceildivpow2(res->y1, pdy) << pdy;

			res->pw = (brprcxend - tlprcxstart) >> pdx;
			res->ph = (brprcyend - tlprcystart) >> pdy;

			if (resno == 0) 
			{
				tlcbgxstart = tlprcxstart;
				tlcbgystart = tlprcystart;
				brcbgxend = brprcxend;
				brcbgyend = brprcyend;
				cbgwidthexpn = pdx;
				cbgheightexpn = pdy;
			} 
			else 
			{
				tlcbgxstart = int_ceildivpow2(tlprcxstart, 1);
				tlcbgystart = int_ceildivpow2(tlprcystart, 1);
				brcbgxend = int_ceildivpow2(brprcxend, 1);
				brcbgyend = int_ceildivpow2(brprcyend, 1);
				cbgwidthexpn = pdx - 1;
				cbgheightexpn = pdy - 1;
			}

			cblkwidthexpn = int_min(tccp->cblkw, cbgwidthexpn);
			cblkheightexpn = int_min(tccp->cblkh, cbgheightexpn);

			for (bandno = 0; bandno < res->numbands; bandno++) 
			{
				int x0b, y0b, i;
				int gain, numbps;
				opj_stepsize_t *ss = NULL;

				opj_tcd_band_t *band = &res->bands[bandno];

				if (resno == 0)
					band->bandno = 0;
				else
					band->bandno = bandno + 1;
				//band->bandno = resno == 0 ? 0 : bandno + 1;
				if ((band->bandno == 1) || (band->bandno == 3) )
					x0b = 1;
				else
					x0b = 0;

				if ((band->bandno == 2) || (band->bandno == 3) )
					y0b = 1;
				else
					y0b = 0;


				if (band->bandno == 0) 
				{
					/* band border (global) */
					band->x0 = int_ceildivpow2(tilec->x0, levelno);
					band->y0 = int_ceildivpow2(tilec->y0, levelno);
					band->x1 = int_ceildivpow2(tilec->x1, levelno);
					band->y1 = int_ceildivpow2(tilec->y1, levelno);
				} 
				else 
				{
					/* band border (global) */
					band->x0 = int_ceildivpow2(tilec->x0 - (1 << levelno) * x0b, levelno + 1);
					band->y0 = int_ceildivpow2(tilec->y0 - (1 << levelno) * y0b, levelno + 1);
					band->x1 = int_ceildivpow2(tilec->x1 - (1 << levelno) * x0b, levelno + 1);
					band->y1 = int_ceildivpow2(tilec->y1 - (1 << levelno) * y0b, levelno + 1);
				}

				if ( resno == 0 )
					ss = &tccp->stepsizes[0];
				else
					ss = &tccp->stepsizes[ 3 * (resno - 1) + bandno + 1];
				//gain = tccp->qmfbid == 0 ? dwt_getgain_real(band->bandno) : dwt_getgain(band->bandno);					
				gain = idwt_getgain(band->bandno);
				numbps = image->comps[compno].prec + gain;


				band->stepsize = (float)((1.0 + ss->mant / 2048.0) * pow(2.0, numbps - ss->expn));
				band->numbps = ss->expn + tccp->numgbits - 1;	/* WHY -1 ? */

				band->precincts = (opj_tcd_precinct_t *) sft_malloc(3 * res->pw * res->ph * sizeof(opj_tcd_precinct_t));

				for (i = 0; i < res->pw * res->ph * 3; i++) 
				{
					band->precincts[i].imsbtree = NULL;
					band->precincts[i].incltree = NULL;
					band->precincts[i].cblks.enc = NULL;
				}

				for (precno = 0; precno < res->pw * res->ph; precno++) 
				{
					int tlcblkxstart, tlcblkystart, brcblkxend, brcblkyend;

					int cbgxstart = tlcbgxstart + (precno % res->pw) * (1 << cbgwidthexpn);
					int cbgystart = tlcbgystart + (precno / res->pw) * (1 << cbgheightexpn);
					int cbgxend = cbgxstart + (1 << cbgwidthexpn);
					int cbgyend = cbgystart + (1 << cbgheightexpn);

					opj_tcd_precinct_t *prc = &band->precincts[precno];

					/* precinct size (global) */
					prc->x0 = int_max(cbgxstart, band->x0);
					prc->y0 = int_max(cbgystart, band->y0);
					prc->x1 = int_min(cbgxend, band->x1);
					prc->y1 = int_min(cbgyend, band->y1);

					tlcblkxstart = int_floordivpow2(prc->x0, cblkwidthexpn) << cblkwidthexpn;
					tlcblkystart = int_floordivpow2(prc->y0, cblkheightexpn) << cblkheightexpn;
					brcblkxend = int_ceildivpow2(prc->x1, cblkwidthexpn) << cblkwidthexpn;
					brcblkyend = int_ceildivpow2(prc->y1, cblkheightexpn) << cblkheightexpn;
					prc->cw = (brcblkxend - tlcblkxstart) >> cblkwidthexpn;
					prc->ch = (brcblkyend - tlcblkystart) >> cblkheightexpn;

					prc->cblks.enc = (opj_tcd_cblk_enc_t*) sft_calloc((prc->cw * prc->ch), sizeof(opj_tcd_cblk_enc_t));
					prc->incltree = itgt_create(prc->cw, prc->ch);
					prc->imsbtree = itgt_create(prc->cw, prc->ch);

					for (cblkno = 0; cblkno < prc->cw * prc->ch; cblkno++) 
					{
						int cblkxstart = tlcblkxstart + (cblkno % prc->cw) * (1 << cblkwidthexpn);
						int cblkystart = tlcblkystart + (cblkno / prc->cw) * (1 << cblkheightexpn);
						int cblkxend = cblkxstart + (1 << cblkwidthexpn);
						int cblkyend = cblkystart + (1 << cblkheightexpn);

						opj_tcd_cblk_enc_t* cblk = &prc->cblks.enc[cblkno];

						/* code-block size (global) */
						cblk->x0 = int_max(cblkxstart, prc->x0);
						cblk->y0 = int_max(cblkystart, prc->y0);
						cblk->x1 = int_min(cblkxend, prc->x1);
						cblk->y1 = int_min(cblkyend, prc->y1);
						cblk->data = (unsigned char*) sft_calloc(8192+2, sizeof(unsigned char));
						/* FIXME: mqc_init_enc and mqc_byteout underrun the buffer if we don't do this. Why? */
						cblk->data += 2;
						cblk->layers = (opj_tcd_layer_t*) sft_calloc(100, sizeof(opj_tcd_layer_t));
						cblk->passes = (opj_tcd_pass_t*) sft_calloc(100, sizeof(opj_tcd_pass_t));
					}
				}
			}
		}
	}


	/* tcd_dump(stdout, tcd, &tcd->tcd_image); */
}

void itcd_init_encode( const opj_tcd_t *tcd,const opj_image_t * image,const opj_cp_t * cp, int curtileno) 
{
	int tileno, compno, resno, bandno, precno, cblkno;

	for (tileno = 0; tileno < 1; tileno++) 
	{
		opj_tcp_t *tcp = &cp->tcps[curtileno];
		int j;
		/* cfr p59 ISO/IEC FDIS15444-1 : 2000 (18 august 2000) */
		int p = curtileno % cp->tw;
		int q = curtileno / cp->tw;

		opj_tcd_tile_t *tile = tcd->tcd_image->tiles;

		/* 4 borders of the tile rescale on the image if necessary */
		tile->x0 = int_max(cp->tx0 + p * cp->tdx, image->x0);
		tile->y0 = int_max(cp->ty0 + q * cp->tdy, image->y0);
		tile->x1 = int_min(cp->tx0 + (p + 1) * cp->tdx, image->x1);
		tile->y1 = int_min(cp->ty0 + (q + 1) * cp->tdy, image->y1);

		tile->numcomps = image->numcomps;
		/* tile->PPT=image->PPT; */

		/* Modification of the RATE >> */
		for (j = 0; j < tcp->numlayers; j++) 
		{
			tcp->rates[j] = 0 ;
		
		}
		/* << Modification of the RATE */

		/* tile->comps=(opj_tcd_tilecomp_t*)opj_realloc(tile->comps,image->numcomps*sizeof(opj_tcd_tilecomp_t)); */
		for (compno = 0; compno < tile->numcomps; compno++) 
		{
			opj_tccp_t *tccp = &tcp->tccps[compno];

			opj_tcd_tilecomp_t *tilec = &tile->comps[compno];

			/* border of each tile component (global) */
			tilec->x0 = int_ceildiv(tile->x0, image->comps[compno].dx);
			tilec->y0 = int_ceildiv(tile->y0, image->comps[compno].dy);
			tilec->x1 = int_ceildiv(tile->x1, image->comps[compno].dx);
			tilec->y1 = int_ceildiv(tile->y1, image->comps[compno].dy);

			tilec->data = (cpixelType *) sft_aligned_malloc((tilec->x1 - tilec->x0) * (tilec->y1 - tilec->y0) * sizeof(cpixelType));
			tilec->numresolutions = tccp->numresolutions;
			/* tilec->resolutions=(opj_tcd_resolution_t*)opj_realloc(tilec->resolutions,tilec->numresolutions*sizeof(opj_tcd_resolution_t)); */
			for (resno = 0; resno < tilec->numresolutions; resno++) 
			{
				int pdx, pdy;

				int levelno = tilec->numresolutions - 1 - resno;
				int tlprcxstart, tlprcystart, brprcxend, brprcyend;
				int tlcbgxstart, tlcbgystart, brcbgxend, brcbgyend;
				int cbgwidthexpn, cbgheightexpn;
				int cblkwidthexpn, cblkheightexpn;

				opj_tcd_resolution_t *res = &tilec->resolutions[resno];

				/* border for each resolution level (global) */
				res->x0 = int_ceildivpow2(tilec->x0, levelno);
				res->y0 = int_ceildivpow2(tilec->y0, levelno);
				res->x1 = int_ceildivpow2(tilec->x1, levelno);
				res->y1 = int_ceildivpow2(tilec->y1, levelno);	
				if (resno == 0)
					res->numbands = 1 ;
				else
					res->numbands = 3;

				/* p. 35, table A-23, ISO/IEC FDIS154444-1 : 2000 (18 august 2000) */
				if (tccp->csty & J2K_CCP_CSTY_PRT) 
				{
					pdx = tccp->prcw[resno];
					pdy = tccp->prch[resno];
				} 
				else 
				{
					pdx = 15;
					pdy = 15;
				}
				/* p. 64, B.6, ISO/IEC FDIS15444-1 : 2000 (18 august 2000)  */
				tlprcxstart = int_floordivpow2(res->x0, pdx) << pdx;
				tlprcystart = int_floordivpow2(res->y0, pdy) << pdy;
				brprcxend = int_ceildivpow2(res->x1, pdx) << pdx;
				brprcyend = int_ceildivpow2(res->y1, pdy) << pdy;

				res->pw = (brprcxend - tlprcxstart) >> pdx;
				res->ph = (brprcyend - tlprcystart) >> pdy;

				if (resno == 0) 
				{
					tlcbgxstart = tlprcxstart;
					tlcbgystart = tlprcystart;
					brcbgxend = brprcxend;
					brcbgyend = brprcyend;
					cbgwidthexpn = pdx;
					cbgheightexpn = pdy;
				} 
				else 
				{
					tlcbgxstart = int_ceildivpow2(tlprcxstart, 1);
					tlcbgystart = int_ceildivpow2(tlprcystart, 1);
					brcbgxend = int_ceildivpow2(brprcxend, 1);
					brcbgyend = int_ceildivpow2(brprcyend, 1);
					cbgwidthexpn = pdx - 1;
					cbgheightexpn = pdy - 1;
				}

				cblkwidthexpn = int_min(tccp->cblkw, cbgwidthexpn);
				cblkheightexpn = int_min(tccp->cblkh, cbgheightexpn);

				for (bandno = 0; bandno < res->numbands; bandno++) 
				{
					int x0b, y0b;
					int gain, numbps;
					opj_stepsize_t *ss = NULL;

					opj_tcd_band_t *band = &res->bands[bandno];

					if (resno == 0)
						band->bandno = 0;
					else
						band->bandno = bandno + 1;

					if ((band->bandno == 1) || (band->bandno == 3) )
						x0b = 1;
					else 
						x0b = 0;

					if ((band->bandno == 2) || (band->bandno == 3) )
						y0b = 1;
					else 
						y0b = 0;

					if (band->bandno == 0) 
					{
						/* band border */
						band->x0 = int_ceildivpow2(tilec->x0, levelno);
						band->y0 = int_ceildivpow2(tilec->y0, levelno);
						band->x1 = int_ceildivpow2(tilec->x1, levelno);
						band->y1 = int_ceildivpow2(tilec->y1, levelno);
					} 
					else 
					{
						band->x0 = int_ceildivpow2(tilec->x0 - (1 << levelno) * x0b, levelno + 1);
						band->y0 = int_ceildivpow2(tilec->y0 - (1 << levelno) * y0b, levelno + 1);
						band->x1 = int_ceildivpow2(tilec->x1 - (1 << levelno) * x0b, levelno + 1);
						band->y1 = int_ceildivpow2(tilec->y1 - (1 << levelno) * y0b, levelno + 1);
					}

					if (resno == 0)
						ss = &tccp->stepsizes[ 0 ];
					else
						ss = &tccp->stepsizes[ 3 * (resno - 1) + bandno + 1];

					gain =  idwt_getgain(band->bandno);

					numbps = image->comps[compno].prec + gain;
					band->stepsize = (float)((1.0 + ss->mant / 2048.0) * pow(2.0, numbps - ss->expn));
					band->numbps = ss->expn + tccp->numgbits - 1;	/* WHY -1 ? */

					for (precno = 0; precno < res->pw * res->ph; precno++) 
					{
						int tlcblkxstart, tlcblkystart, brcblkxend, brcblkyend;

						int cbgxstart = tlcbgxstart + (precno % res->pw) * (1 << cbgwidthexpn);
						int cbgystart = tlcbgystart + (precno / res->pw) * (1 << cbgheightexpn);
						int cbgxend = cbgxstart + (1 << cbgwidthexpn);
						int cbgyend = cbgystart + (1 << cbgheightexpn);

						opj_tcd_precinct_t *prc = &band->precincts[precno];

						/* precinct size (global) */
						prc->x0 = int_max(cbgxstart, band->x0);
						prc->y0 = int_max(cbgystart, band->y0);
						prc->x1 = int_min(cbgxend, band->x1);
						prc->y1 = int_min(cbgyend, band->y1);

						tlcblkxstart = int_floordivpow2(prc->x0, cblkwidthexpn) << cblkwidthexpn;
						tlcblkystart = int_floordivpow2(prc->y0, cblkheightexpn) << cblkheightexpn;
						brcblkxend = int_ceildivpow2(prc->x1, cblkwidthexpn) << cblkwidthexpn;
						brcblkyend = int_ceildivpow2(prc->y1, cblkheightexpn) << cblkheightexpn;
						prc->cw = (brcblkxend - tlcblkxstart) >> cblkwidthexpn;
						prc->ch = (brcblkyend - tlcblkystart) >> cblkheightexpn;

						sft_free(prc->cblks.enc);
						prc->cblks.enc = (opj_tcd_cblk_enc_t*) sft_calloc(prc->cw * prc->ch, sizeof(opj_tcd_cblk_enc_t));

						if (prc->incltree != NULL) 
						{
							itgt_destroy(prc->incltree);
						}
						if (prc->imsbtree != NULL) 
						{
							itgt_destroy(prc->imsbtree);
						}

						prc->incltree = itgt_create(prc->cw, prc->ch);
						prc->imsbtree = itgt_create(prc->cw, prc->ch);

						for (cblkno = 0; cblkno < prc->cw * prc->ch; cblkno++) 
						{
							int cblkxstart = tlcblkxstart + (cblkno % prc->cw) * (1 << cblkwidthexpn);
							int cblkystart = tlcblkystart + (cblkno / prc->cw) * (1 << cblkheightexpn);
							int cblkxend = cblkxstart + (1 << cblkwidthexpn);
							int cblkyend = cblkystart + (1 << cblkheightexpn);

							opj_tcd_cblk_enc_t* cblk = &prc->cblks.enc[cblkno];

							/* code-block size (global) */
							cblk->x0 = int_max(cblkxstart, prc->x0);
							cblk->y0 = int_max(cblkystart, prc->y0);
							cblk->x1 = int_min(cblkxend, prc->x1);
							cblk->y1 = int_min(cblkyend, prc->y1);
							cblk->data = (unsigned char*) sft_calloc(8192+2, sizeof(unsigned char));
							/* FIXME: mqc_init_enc and mqc_byteout underrun the buffer if we don't do this. Why? */
							cblk->data += 2;
							cblk->layers = (opj_tcd_layer_t*) sft_calloc(100, sizeof(opj_tcd_layer_t));
							cblk->passes = (opj_tcd_pass_t*) sft_calloc(100, sizeof(opj_tcd_pass_t));
						}
					} /* precno */
				} /* bandno */
			} /* resno */
		} /* compno */
	} /* tileno */

	/* tcd_dump(stdout, tcd, &tcd->tcd_image); */
}


/*
typedef struct opj_tcd 
{	
int tp_pos; // Position of the tilepart flag in Progression order	
int tp_num; // Tile part number
int cur_tp_num;// Current tile part number
int cur_totnum_tp;// Total number of tileparts of the current tile
int cur_pino;// Current Packet iterator number 
opj_common_ptr cinfo;// codec context 
opj_tcd_image_t *tcd_image;	// info on each image tile 
opj_image_t *image;// image 
opj_cp_t *cp;// coding parameters 
opj_tcd_tile_t *tcd_tile;	// pointer to the current encoded/decoded tile 
opj_tcp_t *tcp;// coding/decoding parameters common to all tiles 
int tcd_tileno; // current encoded/decoded tile 	
double encoding_time;// Time taken to encode a tile
} opj_tcd_t;
*/

opj_tcd_t* iclone_TCD_tile(opj_tcd_t *tcd)
{
	opj_tcd_t *clone = (opj_tcd_t*)sft_malloc(sizeof(opj_tcd_t));
	clone->cinfo = tcd->cinfo;
	clone->cp = tcd->cp;
	clone->cur_pino = tcd->cur_pino;
	//	clone->cur_totnum_tp = tcd->cur_totnum_tp;
	clone->cur_tp_num = tcd->cur_tp_num;
	clone->encoding_time = tcd->encoding_time;
	clone->image = tcd->image;
	clone->tcd_image = tcd->tcd_image;
	clone->tcd_tile = tcd->tcd_tile;
	clone->tcd_tileno = tcd->tcd_tileno;
	clone->tcp = tcd->tcp;
	clone->tp_num = tcd->tp_num;
	clone->tp_pos = tcd->tp_pos;

	return clone;
}


void idwt_encode_stepsize(int stepsize, int numbps, opj_stepsize_t *bandno_stepsize) 
{
	int p, n;
	p = int_floorlog2(stepsize) - 13;
	n = 11 - int_floorlog2(stepsize);
	bandno_stepsize->mant = (n < 0 ? stepsize >> -n : stepsize << n) & 0x7ff;
	bandno_stepsize->expn = numbps - p;
}

void idwt_calc_explicit_stepsizes(opj_tccp_t * tccp, int prec) 
{
	int numbands, bandno;
	numbands = 3 * tccp->numresolutions - 2;
	for (bandno = 0; bandno < numbands; bandno++) 
	{
		double stepsize;
		int resno, level, orient, gain;

		resno = (bandno == 0) ? 0 : ((bandno - 1) / 3 + 1);
		orient = (bandno == 0) ? 0 : ((bandno - 1) % 3 + 1);
		level = tccp->numresolutions - 1 - resno;
		gain = (tccp->qmfbid == 0) ? 0 : ((orient == 0) ? 0 : (((orient == 1) || (orient == 2)) ? 1 : 2));
		if (tccp->qntsty == J2K_CCP_QNTSTY_NOQNT) 
		{
			stepsize = 1.0;
		} 
		else 
		{
			double norm = dwt_norms[orient][level];
			stepsize = (1 << (gain)) / norm;
		}
		idwt_encode_stepsize((int) floor(stepsize * 8192.0), prec + gain, &tccp->stepsizes[bandno]);
	}
}

void ij2k_encodeTile(const opj_cp_t *cp  ,int tileno , opj_j2k_t *j2k , opj_tcd_t *gtcd,const opj_image_t *image )
{
	//opj_cp_t *cp = j2k->cp;		
	int pino;
	int tilepartno=0;
	// UniPG>> 
	int acc_pack_num = 0;
	int compno = 0;
	int curtileno = tileno;
	int cur_tp_num = 0;
	opj_tcd_t* tcd =NULL;

	// <<UniPG 
	opj_tcp_t *tcp = &cp->tcps[tileno];
	curtileno = tileno;

	// initialisation before tile encoding  	
	//tcd = iclone_TCD_tile(gtcd);
	tcd = gtcd;
	//Deprecated :tcd->cur_totnum_tp = j2k->cur_totnum_tp[tileno];
	itcd_init_encode(tcd, image, cp, tileno);


	// INDEX >> 
	for(pino = 0; pino <= tcp->numpocs; pino++) 
	{
		int tot_num_tp;
		tcd->cur_pino=pino;
		//Get number of tile parts
		tot_num_tp = ij2k_get_num_tp(cp,pino,tileno);
		tcd->tp_pos = cp->tp_pos;

		for(tilepartno = 0; tilepartno < tot_num_tp ; tilepartno++)
		{
			int tp_num = tilepartno;			
			unsigned char* bpos = NULL;
			//Encoding
			tcd->tp_num = tp_num ;
			tcd->cur_tp_num = cur_tp_num;
			tcd->tcd_tileno = tileno;
			tcd->tcd_tile = tcd->tcd_image->tiles;
			tcd->tcp = &tcd->cp->tcps[tileno];
			ij2k_encode_Tile(j2k->cp, j2k,tcd,tileno, tp_num,cur_tp_num);
			cur_tp_num++;
		}			
	}

}

