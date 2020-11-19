/*
* Copyright (c) 2002-2007, Communications and Remote Sensing Laboratory, Universite catholique de Louvain (UCL), Belgium
* Copyright (c) 2002-2007, Professor Benoit Macq
* Copyright (c) 2001-2003, David Janssens
* Copyright (c) 2002-2003, Yannick Verschueren
* Copyright (c) 2003-2007, Francois-Olivier Devaux and Antonin Descampe
* Copyright (c) 2005, Herve Drolon, FreeImage Team
* Copyright (c) 2006-2007, Parvatha Elangovan
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS `AS IS'
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#include "opj_includes.h"

#include <omp.h>

/* ----------------------------------------------------------------------- */

/**
Create a new TCD handle
*/
opj_tcd_t* tcd_create(opj_common_ptr cinfo) 
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
void tcd_destroy(opj_tcd_t *tcd) {
	if(tcd) 
	{
		sft_free(tcd->tcd_image);
		sft_free(tcd);
	}
}

void tcd_malloc_encode(opj_tcd_t *tcd,const  opj_image_t * image, opj_cp_t * cp, int curtileno) 
{
	cp->cinema = cp->cinema *2;


}

void tcd_free_encode(opj_tcd_t *tcd)
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
							tgt_destroy(prc->incltree);
							prc->incltree = NULL;
						}
						if (prc->imsbtree != NULL) 
						{
							tgt_destroy(prc->imsbtree);	
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

void tcd_init_encode(opj_tcd_t *tcd, const opj_image_t * image, opj_cp_t * cp, int curtileno) 
{
}

void tcd_malloc_decode(opj_tcd_t *tcd, opj_image_t * image, opj_cp_t * cp) 
{
}

void tcd_malloc_decode_tile(opj_tcd_t *tcd, opj_image_t * image, opj_cp_t * cp, int tileno, opj_codestream_info_t *cstr_info) {

}

void tcd_makelayer_fixed(opj_tcd_t *tcd, int layno, int final) 
{
	return ;
}

void tcd_rateallocate_fixed(opj_tcd_t *tcd) 
{
	int layno;
	for (layno = 0; layno < tcd->tcp->numlayers; layno++) 
	{
		tcd_makelayer_fixed(tcd, layno, 1);
	}
}

void tcd_makelayer(opj_tcd_tile_t *tcd_tile, int layno, double thresh, int final) 
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

opj_bool tcd_rateallocate(opj_tcd_tile_t *tcd_tile, const opj_tcd_t *tcd,int len, opj_codestream_info_t *cstr_info) 
{
	int compno, resno, bandno, precno, cblkno, passno, layno;
	double min, max;
	double cumdisto[100];	/* fixed_quality */
	const double K = 1;		/* 1.1; fixed_quality */
	double maxSE = 0;

	opj_cp_t *cp = tcd->cp;	 
	opj_tcp_t *tcd_tcp = tcd->tcp;

	min = DBL_MAX;
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
		tcd_makelayer(tcd->tcd_tile, layno, goodthresh, 1);

		/* fixed_quality */
		if (layno == 0) 
			cumdisto[layno] = tcd_tile->distolayer[0] ;
		else
			cumdisto[layno] = (cumdisto[layno - 1] + tcd_tile->distolayer[layno]);	
	}

	return OPJ_TRUE;
}

int tcd_encode_tile(const opj_j2k_t *j2k ,  const opj_tcd_t *tcd,const int tileno,unsigned char *dest,const int len, opj_codestream_info_t *cstr_info) 
{
	int compno;
	int l,  numpacks = 0;
	opj_tcd_tile_t *tile = NULL;
	opj_tcp_t *tcd_tcp = NULL;
	opj_cp_t *cp = NULL;

	opj_tcp_t *tcp = &tcd->cp->tcps[0];
	opj_tccp_t *tccp = &tcp->tccps[0];
	opj_image_t *image = tcd->image;

	opj_t1_t *t1 = NULL;		/* T1 component */
	opj_t2_t *t2 = NULL;		/* T2 component */

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
					int *tile_data = &tilec->data[(y - tilec->y0) * tw];
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
			int* restrict c0 = tile->comps[0].data ; 
			int* restrict c1 = tile->comps[1].data ; 
			int* restrict c2 = tile->comps[2].data ; 
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
			dwt_encode(tilec);			
		}

		/*------------------TIER1-----------------*/
		/*JUAN
		t1 = t1_create(tcd->cinfo);
		t1_encode_cblks(t1, tile, tcd_tcp);
		t1_destroy(t1);
		*/

		/*-----------RATE-ALLOCATE------------------*/
		/* INDEX */
		/* Normal Rate/distortion allocation */
		tcd_rateallocate(tcd->tcd_tile , tcd,  len, cstr_info);
	}
	/*--------------TIER2------------------*/
	/* INDEX */
	/*JUAN
	t2 = t2_create(tcd->cinfo, image, cp);
	l = t2_encode_packets(t2,tileno, tile, tcd_tcp->numlayers, dest, len, cstr_info,
							tcd->tp_num,tcd->tp_pos,tcd->cur_pino,FINAL_PASS,
							j2k->cur_totnum_tp[tileno]);
	t2_destroy(t2);
	*/

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

/////////////////////////////////////////////
/// DECODING
/////////////////////////////////////////////

opj_bool tcd_decode_tile(opj_tcd_t *tcd, unsigned char *src, int len, int tileno, opj_codestream_info_t *cstr_info) 
{

	return OPJ_TRUE;
}

void tcd_free_decode(opj_tcd_t *tcd) 
{
}

void tcd_free_decode_tile(opj_tcd_t *tcd, int tileno) 
{
}



