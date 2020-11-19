
#include "sft_j2k_streamRW.h"
#include "sft_all_types.h"
#include "cio.h"
#include "int.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct ij2k_prog_order{
	OPJ_PROG_ORDER enum_prog;
	char str_prog[5];
}ij2k_prog_order_t;

ij2k_prog_order_t ij2k_prog_order_list[] = {
	{CPRL, "CPRL"},
	{LRCP, "LRCP"},
	{PCRL, "PCRL"},
	{RLCP, "RLCP"},
	{RPCL, "RPCL"},
	{(OPJ_PROG_ORDER)-1, ""}
};

void ij2k_write_soc(opj_j2k_t *j2k) 
{
	opj_cio_t *cio = j2k->cio;
	cio_write(cio, J2K_MS_SOC, 2);
}

void ij2k_write_siz(opj_j2k_t *j2k) 
{
	int i;
	int lenp, len;

	opj_cio_t *cio = j2k->cio;
	opj_image_t *image = j2k->image;
	opj_cp_t *cp = j2k->cp;

	cio_write(cio, J2K_MS_SIZ, 2);	/* SIZ */
	lenp = cio_tell(cio);
	cio_skip(cio, 2);
	cio_write(cio, cp->rsiz, 2);			/* Rsiz (capabilities) */
	cio_write(cio, image->x1, 4);	/* Xsiz */
	cio_write(cio, image->y1, 4);	/* Ysiz */
	cio_write(cio, image->x0, 4);	/* X0siz */
	cio_write(cio, image->y0, 4);	/* Y0siz */
	cio_write(cio, cp->tdx, 4);		/* XTsiz */
	cio_write(cio, cp->tdy, 4);		/* YTsiz */
	cio_write(cio, cp->tx0, 4);		/* XT0siz */
	cio_write(cio, cp->ty0, 4);		/* YT0siz */
	cio_write(cio, image->numcomps, 2);	/* Csiz */
	for (i = 0; i < image->numcomps; i++) 
	{
		cio_write(cio, image->comps[i].prec - 1 + (image->comps[i].sgnd << 7), 1);	/* Ssiz_i */
		cio_write(cio, image->comps[i].dx, 1);	/* XRsiz_i */
		cio_write(cio, image->comps[i].dy, 1);	/* YRsiz_i */
	}
	len = cio_tell(cio) - lenp;
	cio_seek(cio, lenp);
	cio_write(cio, len, 2);		/* Lsiz */
	cio_seek(cio, lenp + len);
}

void ij2k_write_tlm(opj_j2k_t *j2k)
{
	int lenp;
	opj_cio_t *cio = j2k->cio;
	j2k->tlm_start = cio_tell(cio);
	cio_write(cio, J2K_MS_TLM, 2);/* TLM */
	lenp = 4 + (5*j2k->totnum_tp);
	cio_write(cio,lenp,2);				/* Ltlm */
	cio_write(cio, 0,1);					/* Ztlm=0*/
	cio_write(cio,80,1);					/* Stlm ST=1(8bits-255 tiles max),SP=1(Ptlm=32bits) */
	cio_skip(cio,5*j2k->totnum_tp);
}
void ij2k_write_sod(opj_j2k_t *j2k, opj_tcd_t* tcd, int tileno, int tp_num , int cur_tp_num) 
{
	int l ,i;
	int totlen;	
	unsigned char* bpos = NULL;
		
	l = tcd->dataLength;
	
	// Store to CIO
	cio_write(j2k->cio, J2K_MS_SOD, 2);

	if (tileno == 0) 
	{
		j2k->sod_start = cio_tell(j2k->cio) + j2k->pos_correction;
	}


	bpos = j2k->cio->bp;
	for (i = 0 ; i<tcd->dataLength ; i++)	
	{
		*j2k->cio->bp = tcd->packetData[i];
		j2k->cio->bp++;
	}
	j2k->cio->bp = bpos;
	// Writing Psot in SOT marker 
	totlen = cio_tell(j2k->cio) + l - j2k->sot_start;
	cio_seek(j2k->cio, j2k->sot_start + 6);
	cio_write(j2k->cio, totlen, 4);
	cio_seek(j2k->cio, j2k->sot_start + totlen);
	// Writing Ttlm and Ptlm in TLM marker 
	//if(cp->cinema)
	cio_seek(j2k->cio, j2k->sot_start + totlen);
}

void ij2k_write_cod(opj_j2k_t *j2k,int curtileno) 
{
	opj_cp_t *cp = NULL;
	opj_tcp_t *tcp = NULL;
	int lenp, len;
	int i;

	opj_cio_t *cio = j2k->cio;
	opj_tccp_t *tccp = NULL;

	cio_write(cio, J2K_MS_COD, 2);	/* COD */

	lenp = cio_tell(cio);
	cio_skip(cio, 2);

	cp = j2k->cp;
	tcp = &cp->tcps[curtileno];
	tccp = &tcp->tccps[0];

	cio_write(cio, tcp->csty, 1);		/* Scod */
	cio_write(cio, tcp->prg, 1);		/* SGcod (A) */
	cio_write(cio, tcp->numlayers, 2);	/* SGcod (B) */
	cio_write(cio, tcp->mct, 1);		/* SGcod (C) */

	//ij2k_write_cox(j2k, 0);
	cio_write(cio, tccp->numresolutions - 1, 1);	/* SPcox (D) */
	cio_write(cio, tccp->cblkw - 2, 1);				/* SPcox (E) */
	cio_write(cio, tccp->cblkh - 2, 1);				/* SPcox (F) */
	cio_write(cio, tccp->cblksty, 1);				/* SPcox (G) */
	cio_write(cio, tccp->qmfbid, 1);				/* SPcox (H) */

	if (tccp->csty & J2K_CCP_CSTY_PRT) 
	{
		for (i = 0; i < tccp->numresolutions; i++) {
			cio_write(cio, tccp->prcw[i] + (tccp->prch[i] << 4), 1);	/* SPcox (I_i) */
		}
	}
	len = cio_tell(cio) - lenp;
	cio_seek(cio, lenp);
	cio_write(cio, len, 2);		/* Lcod */
	cio_seek(cio, lenp + len);

}

void ij2k_write_qcd(opj_j2k_t *j2k, int curtileno) 
{
	int lenp, len;
	int bandno, numbands;
	int expn, mant;
	int compno = 0;
	opj_cio_t *cio = j2k->cio;	
	opj_cp_t *cp = j2k->cp;
	opj_tcp_t *tcp = &cp->tcps[curtileno];
	opj_tccp_t *tccp = &tcp->tccps[compno];
	
	cio_write(cio, J2K_MS_QCD, 2);	/* QCD */
	lenp = cio_tell(cio);
	cio_skip(cio, 2);
	//ij2k_write_qcx(j2k, 0);
	cio_write(cio, tccp->qntsty + (tccp->numgbits << 5), 1);	/* Sqcx */
	if (tccp->qntsty == J2K_CCP_QNTSTY_SIQNT)
		numbands =  1;
	else
	   numbands =   tccp->numresolutions * 3 - 2;

	for (bandno = 0; bandno < numbands; bandno++) 
	{
		expn = tccp->stepsizes[bandno].expn;
		mant = tccp->stepsizes[bandno].mant;

		if (tccp->qntsty == J2K_CCP_QNTSTY_NOQNT) 
		{
			cio_write(cio, expn << 3, 1);	/* SPqcx_i */
		} 
		else 
		{
			cio_write(cio, (expn << 11) + mant, 2);	/* SPqcx_i */
		}
	}
	len = cio_tell(cio) - lenp;
	cio_seek(cio, lenp);
	cio_write(cio, len, 2);			/* Lqcd */
	cio_seek(cio, lenp + len);		
}

void ij2k_write_com(opj_j2k_t *j2k) 
{
	unsigned int i;
	int lenp, len;

	if(j2k->cp->comment) 
	{
		opj_cio_t *cio = j2k->cio;
		char *comment = j2k->cp->comment;

		cio_write(cio, J2K_MS_COM, 2);
		lenp = cio_tell(cio);
		cio_skip(cio, 2);
		cio_write(cio, 1, 2);		/* General use (IS 8859-15:1999 (Latin) values) */
		for (i = 0; i < strlen(comment); i++) 
		{
			cio_write(cio, comment[i], 1);
		}
		len = cio_tell(cio) - lenp;
		cio_seek(cio, lenp);
		cio_write(cio, len, 2);
		cio_seek(cio, lenp + len);
	}
}

void ij2k_write_sot(opj_cio_t *cio , opj_j2k_t *j2k, int curtileno, int cur_tp_num) 
{
	int lenp, len;

	j2k->sot_start = cio_tell(cio);
	cio_write(cio, J2K_MS_SOT, 2);		/* SOT */
	lenp = cio_tell(cio);
	cio_skip(cio, 2);					/* Lsot (further) */
	cio_write(cio, curtileno, 2);	/* Isot */
	cio_skip(cio, 4);					/* Psot (further in j2k_write_sod) */
	cio_write(cio, cur_tp_num , 1);	/* TPsot */
	cio_write(cio, j2k->cur_totnum_tp[curtileno], 1);		/* TNsot */
	len = cio_tell(cio) - lenp;
	cio_seek(cio, lenp);
	cio_write(cio, len, 2);				/* Lsot */
	cio_seek(cio, lenp + len);
}

void ij2k_write_coc(opj_j2k_t *j2k, int compno, int curtileno) 
{
	int lenp, len,i;

	opj_cp_t *cp = j2k->cp;
	opj_tcp_t *tcp = &cp->tcps[curtileno];
	opj_image_t *image = j2k->image;
	opj_cio_t *cio = j2k->cio;
	opj_tccp_t *tccp = &tcp->tccps[compno];
	
	cio_write(cio, J2K_MS_COC, 2);	/* COC */
	lenp = cio_tell(cio);
	cio_skip(cio, 2);
	if (image->numcomps <=256)
		cio_write(cio, compno, 1);	/* Ccoc */
	else
	   cio_write(cio, compno,  2);	/* Ccoc */
	cio_write(cio, tcp->tccps[compno].csty, 1);	/* Scoc */
	
	//ij2k_write_cox(j2k, compno);
	cio_write(cio, tccp->numresolutions - 1, 1);	/* SPcox (D) */
	cio_write(cio, tccp->cblkw - 2, 1);				/* SPcox (E) */
	cio_write(cio, tccp->cblkh - 2, 1);				/* SPcox (F) */
	cio_write(cio, tccp->cblksty, 1);				/* SPcox (G) */
	cio_write(cio, tccp->qmfbid, 1);				/* SPcox (H) */

	if (tccp->csty & J2K_CCP_CSTY_PRT) 
	{
		for (i = 0; i < tccp->numresolutions; i++) 
		{
			cio_write(cio, tccp->prcw[i] + (tccp->prch[i] << 4), 1);	/* SPcox (I_i) */
		}
	}

	len = cio_tell(cio) - lenp;
	cio_seek(cio, lenp);
	cio_write(cio, len, 2);			/* Lcoc */
	cio_seek(cio, lenp + len);
}

void ij2k_write_qcc(opj_j2k_t *j2k, int compno, int curtileno) 
{
	int lenp, len;
	int bandno, numbands;
	int expn, mant;

	opj_cio_t *cio = j2k->cio;
	opj_cp_t *cp = j2k->cp;
	opj_tcp_t *tcp = &cp->tcps[curtileno];
	opj_tccp_t *tccp = &tcp->tccps[compno];	
	
	cio_write(cio, J2K_MS_QCC, 2);	/* QCC */
	lenp = cio_tell(cio);
	cio_skip(cio, 2);
	if (j2k->image->numcomps <= 256)
	  cio_write(cio, compno,  1 );	/* Cqcc */	
	else
	  cio_write(cio, compno, 2);	/* Cqcc */	
	
	//ij2k_write_qcx(j2k, compno);	
	cio_write(cio, tccp->qntsty + (tccp->numgbits << 5), 1);	/* Sqcx */

	if (tccp->qntsty == J2K_CCP_QNTSTY_SIQNT)
		numbands = 1;
	else
	   numbands =  tccp->numresolutions * 3 - 2;

	for (bandno = 0; bandno < numbands; bandno++) 
	{
		expn = tccp->stepsizes[bandno].expn;
		mant = tccp->stepsizes[bandno].mant;

		if (tccp->qntsty == J2K_CCP_QNTSTY_NOQNT) 
		{
			cio_write(cio, expn << 3, 1);	/* SPqcx_i */
		} 
		else 
		{
			cio_write(cio, (expn << 11) + mant, 2);	/* SPqcx_i */
		}
	}
	len = cio_tell(cio) - lenp;
	cio_seek(cio, lenp);
	cio_write(cio, len, 2);			/* Lqcc */
	cio_seek(cio, lenp + len);
}

void ij2k_write_poc(opj_j2k_t *j2k, int curtileno) 
{
	int len, numpchgs, i;

	int numcomps = j2k->image->numcomps;
	
	opj_cp_t *cp = j2k->cp;
	opj_tcp_t *tcp = &cp->tcps[curtileno];
	opj_tccp_t *tccp = &tcp->tccps[0];
	opj_cio_t *cio = j2k->cio;

	numpchgs = 1 + tcp->numpocs;
	cio_write(cio, J2K_MS_POC, 2);	/* POC  */
	if (numcomps <= 256)
	   len = 2 + (5 + 2 * (1 )) * numpchgs;
	else
		len = 2 + (5 + 2 * ( 2)) * numpchgs;
	
	cio_write(cio, len, 2);		/* Lpoc */
	for (i = 0; i < numpchgs; i++) 
	{
		opj_poc_t *poc = &tcp->pocs[i];
		cio_write(cio, poc->resno0, 1);	/* RSpoc_i */
		cio_write(cio, poc->compno0, (numcomps <= 256 ? 1 : 2));	/* CSpoc_i */
		cio_write(cio, poc->layno1, 2);	/* LYEpoc_i */
		poc->layno1 = int_min(poc->layno1, tcp->numlayers);
		cio_write(cio, poc->resno1, 1);	/* REpoc_i */
		poc->resno1 = int_min(poc->resno1, tccp->numresolutions);
		if (numcomps <= 256 )
		  cio_write(cio, poc->compno1, ( 1 ));	/* CEpoc_i */
		else
			cio_write(cio, poc->compno1, ( 2));	/* CEpoc_i */
		poc->compno1 = int_min(poc->compno1, numcomps);
		cio_write(cio, poc->prg, 1);	/* Ppoc_i */
	}
}

void ij2k_write_rgn(opj_j2k_t *j2k, int compno, int tileno) 
{
	opj_cp_t *cp = j2k->cp;
	opj_tcp_t *tcp = &cp->tcps[tileno];
	opj_cio_t *cio = j2k->cio;
	int numcomps = j2k->image->numcomps;
	
	cio_write(cio, J2K_MS_RGN, 2);						/* RGN  */
	if (numcomps <= 256)
		cio_write(cio, 5 , 2);			/* Lrgn */
	else
		cio_write(cio, 6, 2);			/* Lrgn */
	
	if (numcomps <= 256)
	  cio_write(cio, compno, 1);	/* Crgn */
	else
		cio_write(cio, compno, 2);	/* Crgn */
	cio_write(cio, 0, 1);								/* Srgn */
	cio_write(cio, tcp->tccps[compno].roishift, 1);		/* SPrgn */
}

void ij2k_write_eoc(opj_j2k_t *j2k) 
{
	opj_cio_t *cio = j2k->cio;
	/* opj_event_msg(j2k->cinfo, "%.8x: EOC\n", cio_tell(cio) + j2k->pos_correction); */
	cio_write(cio, J2K_MS_EOC, 2);
	/* update markers struct */
}

char *ij2k_convert_progression_order(OPJ_PROG_ORDER prg_order)
{
	ij2k_prog_order_t *po;
	for(po = ij2k_prog_order_list; po->enum_prog != -1; po++ ){
		if(po->enum_prog == prg_order){
			break;
		}
	}
	return po->str_prog;
}


int ij2k_get_num_tp(opj_cp_t *cp,int pino,int tileno)
{
	char *prog;
	int i;
	int tpnum=1,tpend=0;
	opj_tcp_t *tcp = &cp->tcps[tileno];
	prog = ij2k_convert_progression_order(tcp->prg);
	
	if(cp->tp_on == 1){
		for(i=0;i<4;i++){
			if(tpend!=1){
				if( cp->tp_flag == prog[i] ){
					tpend=1;cp->tp_pos=i;
				}
				switch(prog[i]){
				case 'C':
					tpnum= tpnum * tcp->pocs[pino].compE;
					break;
				case 'R':
					tpnum= tpnum * tcp->pocs[pino].resE;
					break;
				case 'P':
					tpnum= tpnum * tcp->pocs[pino].prcE;
					break;
				case 'L':
					tpnum= tpnum * tcp->pocs[pino].layE;
					break;
				}
			}
		}
	}else{
		tpnum=1;
	}
	return tpnum;
}


void ij2k_writeToCio(int tileno , opj_j2k_t *j2k , opj_tcd_t *tcd,const opj_image_t *image )
{	
	opj_cp_t *cp = j2k->cp;		
	int pino;
	int tilepartno=0;
	// UniPG>> 
	int acc_pack_num = 0;
	int compno = 0;
	int curtileno = tileno;
	int cur_tp_num = 0;
	
	// <<UniPG 
	opj_tcp_t *tcp = &cp->tcps[tileno];
    curtileno = tileno;

	cur_tp_num = 0;
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
			//Encoding
			tcd->tp_num = tilepartno ;
			tcd->cur_tp_num = cur_tp_num;
			tcd->tcd_tileno = tileno;
			tcd->tcd_tile = tcd->tcd_image->tiles;
			tcd->tcp = &tcd->cp->tcps[tileno];
			// INDEX >> 
			ij2k_write_sot(j2k->cio , j2k ,tileno , cur_tp_num);

			if(cur_tp_num == 0 && cp->cinema == 0)
			{
				for (compno = 1; compno < image->numcomps; compno++) 
				{					
					ij2k_write_coc(j2k, compno,tileno);
					ij2k_write_qcc(j2k, compno,tileno);
				}
				if (cp->tcps[tileno].numpocs) 
				{					
					ij2k_write_poc(j2k, tileno);
				}
			}

			// INDEX >> 
			ij2k_write_sod(j2k, tcd , tileno, tilepartno , cur_tp_num);
			// INDEX >> 
			cur_tp_num++;
		}			
}
	}