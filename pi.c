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

#include "pi.h"
#include "int.h"
#include "opj_malloc.h"
#include <stdio.h>

/** @defgroup PI PI - Implementation of a packet iterator */
/*@{*/

/** @name Local static functions */
/*@{*/

/**
Get next packet in layer-resolution-component-precinct order.
@param pi packet iterator to modify
@return returns false if pi pointed to the last packet or else returns true 
*/
static opj_bool pi_next_lrcp(iopj_pi_iterator_t * pi);
/**
Get next packet in resolution-layer-component-precinct order.
@param pi packet iterator to modify
@return returns false if pi pointed to the last packet or else returns true 
*/
//static opj_bool pi_next_rlcp(iopj_pi_iterator_t * pi);
/**
Get next packet in resolution-precinct-component-layer order.
@param pi packet iterator to modify
@return returns false if pi pointed to the last packet or else returns true 
*/
//static opj_bool pi_next_rpcl(iopj_pi_iterator_t * pi);
/**
Get next packet in precinct-component-resolution-layer order.
@param pi packet iterator to modify
@return returns false if pi pointed to the last packet or else returns true 
*/
//static opj_bool pi_next_pcrl(iopj_pi_iterator_t * pi);
/**
Get next packet in component-precinct-resolution-layer order.
@param pi packet iterator to modify
@return returns false if pi pointed to the last packet or else returns true 
*/
//static opj_bool pi_next_cprl(iopj_pi_iterator_t * pi);

/*@}*/

/*@}*/

/* 
==========================================================
local functions
==========================================================
*/

static opj_bool pi_next_lrcp(iopj_pi_iterator_t * pi) 
{
	iopj_pi_comp_t *comp = NULL;
	iopj_pi_resolution_t *res = NULL;
	long index = 0;
	int LABEL_SKIP ;

	if (!pi->first) 
	{
		comp = &pi->comps[pi->compno];
		res = &comp->resolutions[pi->resno];
		//goto LABEL_SKIP;
		LABEL_SKIP = 1;
	} 
	else 
	{
		pi->first = 0;
		LABEL_SKIP = 0;
	}

	for (pi->layno = pi->poc.layno0; pi->layno < pi->poc.layno1; pi->layno++) 
	{
		for (pi->resno = pi->poc.resno0; pi->resno < pi->poc.resno1;	pi->resno++) 
		{
			for (pi->compno = pi->poc.compno0; pi->compno < pi->poc.compno1; pi->compno++) 
			{
				comp = &pi->comps[pi->compno];
				if (pi->resno >= comp->numresolutions) 
				{
					continue;
				}
				res = &comp->resolutions[pi->resno];
				if (!pi->tp_on)
				{
					pi->poc.precno1 = res->pw * res->ph;
				}
				for (pi->precno = pi->poc.precno0; pi->precno < pi->poc.precno1; pi->precno++) 
				{
				 if (!LABEL_SKIP)
				 {
					index = pi->layno * pi->step_l + pi->resno * pi->step_r + pi->compno * pi->step_c + pi->precno * pi->step_p;
					if (!pi->include[index]) 
					{
						pi->include[index] = 1;
						return OPJ_TRUE;
					}
				 }
//LABEL_SKIP:;	
				 LABEL_SKIP = 0;
				}
			}
		}
	}

	return OPJ_FALSE;
}


/* 
==========================================================
Packet iterator interface
==========================================================
*/

iopj_pi_iterator_t *pi_create_decode(opj_image_t *image, opj_cp_t *cp, int tileno) 
{
	int p, q;
	int compno, resno, pino;
	iopj_pi_iterator_t *pi = NULL;
	opj_tcp_t *tcp = NULL;
	opj_tccp_t *tccp = NULL;

	tcp = &cp->tcps[tileno];

	pi = (iopj_pi_iterator_t*) sft_calloc((tcp->numpocs + 1), sizeof(iopj_pi_iterator_t));
	if(!pi) 
	{
		/* TODO: throw an error */
		return NULL;
	}

	for (pino = 0; pino < tcp->numpocs + 1; pino++) {	/* change */
		int maxres = 0;
		int maxprec = 0;
		p = tileno % cp->tw;
		q = tileno / cp->tw;

		pi[pino].tx0 = int_max(cp->tx0 + p * cp->tdx, image->x0);
		pi[pino].ty0 = int_max(cp->ty0 + q * cp->tdy, image->y0);
		pi[pino].tx1 = int_min(cp->tx0 + (p + 1) * cp->tdx, image->x1);
		pi[pino].ty1 = int_min(cp->ty0 + (q + 1) * cp->tdy, image->y1);
		pi[pino].numcomps = image->numcomps;

		pi[pino].comps = (iopj_pi_comp_t*) sft_calloc(image->numcomps, sizeof(iopj_pi_comp_t));
		if(!pi[pino].comps) {
			/* TODO: throw an error */
			pi_destroy(pi, cp, tileno);
			return NULL;
		}

		for (compno = 0; compno < pi->numcomps; compno++) {
			int tcx0, tcy0, tcx1, tcy1;
			iopj_pi_comp_t *comp = &pi[pino].comps[compno];
			tccp = &tcp->tccps[compno];
			comp->dx = image->comps[compno].dx;
			comp->dy = image->comps[compno].dy;
			comp->numresolutions = tccp->numresolutions;

			comp->resolutions = (iopj_pi_resolution_t*) sft_calloc(comp->numresolutions, sizeof(iopj_pi_resolution_t));
			if(!comp->resolutions) {
				/* TODO: throw an error */
				pi_destroy(pi, cp, tileno);
				return NULL;
			}

			tcx0 = int_ceildiv(pi->tx0, comp->dx);
			tcy0 = int_ceildiv(pi->ty0, comp->dy);
			tcx1 = int_ceildiv(pi->tx1, comp->dx);
			tcy1 = int_ceildiv(pi->ty1, comp->dy);
			if (comp->numresolutions > maxres) {
				maxres = comp->numresolutions;
			}

			for (resno = 0; resno < comp->numresolutions; resno++) {
				int levelno;
				int rx0, ry0, rx1, ry1;
				int px0, py0, px1, py1;
				iopj_pi_resolution_t *res = &comp->resolutions[resno];
				if (tccp->csty & J2K_CCP_CSTY_PRT) {
					res->pdx = tccp->prcw[resno];
					res->pdy = tccp->prch[resno];
				} else {
					res->pdx = 15;
					res->pdy = 15;
				}
				levelno = comp->numresolutions - 1 - resno;
				rx0 = int_ceildivpow2(tcx0, levelno);
				ry0 = int_ceildivpow2(tcy0, levelno);
				rx1 = int_ceildivpow2(tcx1, levelno);
				ry1 = int_ceildivpow2(tcy1, levelno);
				px0 = int_floordivpow2(rx0, res->pdx) << res->pdx;
				py0 = int_floordivpow2(ry0, res->pdy) << res->pdy;
				px1 = int_ceildivpow2(rx1, res->pdx) << res->pdx;
				py1 = int_ceildivpow2(ry1, res->pdy) << res->pdy;
				res->pw = (rx0==rx1)?0:((px1 - px0) >> res->pdx);
				res->ph = (ry0==ry1)?0:((py1 - py0) >> res->pdy);

				if (res->pw*res->ph > maxprec) {
					maxprec = res->pw*res->ph;
				}

			}
		}

		tccp = &tcp->tccps[0];
		pi[pino].step_p = 1;
		pi[pino].step_c = maxprec * pi[pino].step_p;
		pi[pino].step_r = image->numcomps * pi[pino].step_c;
		pi[pino].step_l = maxres * pi[pino].step_r;

		if (pino == 0) {
			pi[pino].include = (short int*) sft_calloc(image->numcomps * maxres * tcp->numlayers * maxprec, sizeof(short int));
			if(!pi[pino].include) {
				/* TODO: throw an error */
				pi_destroy(pi, cp, tileno);
				return NULL;
			}
		}
		else {
			pi[pino].include = pi[pino - 1].include;
		}

		if (tcp->POC == 0) {
			pi[pino].first = 1;
			pi[pino].poc.resno0 = 0;
			pi[pino].poc.compno0 = 0;
			pi[pino].poc.layno1 = tcp->numlayers;
			pi[pino].poc.resno1 = maxres;
			pi[pino].poc.compno1 = image->numcomps;
			pi[pino].poc.prg = tcp->prg;
		} else {
			pi[pino].first = 1;
			pi[pino].poc.resno0 = tcp->pocs[pino].resno0;
			pi[pino].poc.compno0 = tcp->pocs[pino].compno0;
			pi[pino].poc.layno1 = tcp->pocs[pino].layno1;
			pi[pino].poc.resno1 = tcp->pocs[pino].resno1;
			pi[pino].poc.compno1 = tcp->pocs[pino].compno1;
			pi[pino].poc.prg = tcp->pocs[pino].prg;
		}
		pi[pino].poc.layno0  = 0;
		pi[pino].poc.precno0 = 0; 
		pi[pino].poc.precno1 = maxprec;

	}

	return pi;
}


iopj_pi_iterator_t *pi_initialise_encode(opj_image_t *image, opj_cp_t *cp, int tileno, J2K_T2_MODE t2_mode)
{ 
	int p, q, pino;
	int compno, resno;
	int maxres = 0;
	int maxprec = 0;
	iopj_pi_iterator_t *pi = NULL;
	opj_tcp_t *tcp = NULL;
	opj_tccp_t *tccp = NULL;

	tcp = &cp->tcps[tileno];

	pi = (iopj_pi_iterator_t*) sft_calloc((tcp->numpocs + 1), sizeof(iopj_pi_iterator_t));
	if(!pi) {	return NULL;}
	pi->tp_on = cp->tp_on;

	for(pino = 0;pino < tcp->numpocs+1 ; pino ++)
	{
		p = tileno % cp->tw;
		q = tileno / cp->tw;

		pi[pino].tx0 = int_max(cp->tx0 + p * cp->tdx, image->x0);
		pi[pino].ty0 = int_max(cp->ty0 + q * cp->tdy, image->y0);
		pi[pino].tx1 = int_min(cp->tx0 + (p + 1) * cp->tdx, image->x1);
		pi[pino].ty1 = int_min(cp->ty0 + (q + 1) * cp->tdy, image->y1);
		pi[pino].numcomps = image->numcomps;

		pi[pino].comps = (iopj_pi_comp_t*) sft_calloc(image->numcomps, sizeof(iopj_pi_comp_t));
		if(!pi[pino].comps) {
			pi_destroy(pi, cp, tileno);
			return NULL;
		}

		for (compno = 0; compno < pi[pino].numcomps; compno++) 
		{
			int tcx0, tcy0, tcx1, tcy1;
			iopj_pi_comp_t *comp = &pi[pino].comps[compno];
			tccp = &tcp->tccps[compno];
			comp->dx = image->comps[compno].dx;
			comp->dy = image->comps[compno].dy;
			comp->numresolutions = tccp->numresolutions;

			comp->resolutions = (iopj_pi_resolution_t*) sft_malloc(comp->numresolutions * sizeof(iopj_pi_resolution_t));
			if(!comp->resolutions) 
			{
				pi_destroy(pi, cp, tileno);
				return NULL;
			}

			tcx0 = int_ceildiv(pi[pino].tx0, comp->dx);
			tcy0 = int_ceildiv(pi[pino].ty0, comp->dy);
			tcx1 = int_ceildiv(pi[pino].tx1, comp->dx);
			tcy1 = int_ceildiv(pi[pino].ty1, comp->dy);
			if (comp->numresolutions > maxres) 
			{
				maxres = comp->numresolutions;
			}

			for (resno = 0; resno < comp->numresolutions; resno++) 
			{
				int levelno;
				int rx0, ry0, rx1, ry1;
				int px0, py0, px1, py1;
				iopj_pi_resolution_t *res = &comp->resolutions[resno];
				if (tccp->csty & J2K_CCP_CSTY_PRT) 
				{
					res->pdx = tccp->prcw[resno];
					res->pdy = tccp->prch[resno];
				} 
				else 
				{
					res->pdx = 15;
					res->pdy = 15;
				}
				levelno = comp->numresolutions - 1 - resno;
				rx0 = int_ceildivpow2(tcx0, levelno);
				ry0 = int_ceildivpow2(tcy0, levelno);
				rx1 = int_ceildivpow2(tcx1, levelno);
				ry1 = int_ceildivpow2(tcy1, levelno);
				px0 = int_floordivpow2(rx0, res->pdx) << res->pdx;
				py0 = int_floordivpow2(ry0, res->pdy) << res->pdy;
				px1 = int_ceildivpow2(rx1, res->pdx) << res->pdx;
				py1 = int_ceildivpow2(ry1, res->pdy) << res->pdy;
				res->pw = (rx0==rx1)?0:((px1 - px0) >> res->pdx);
				res->ph = (ry0==ry1)?0:((py1 - py0) >> res->pdy);

				if (res->pw*res->ph > maxprec) 
				{
					maxprec = res->pw * res->ph;
				}
			}
		}

		tccp = &tcp->tccps[0];
		pi[pino].step_p = 1;
		pi[pino].step_c = maxprec * pi[pino].step_p;
		pi[pino].step_r = image->numcomps * pi[pino].step_c;
		pi[pino].step_l = maxres * pi[pino].step_r;

		for (compno = 0; compno < pi->numcomps; compno++) 
		{
			iopj_pi_comp_t *comp = &pi->comps[compno];
			for (resno = 0; resno < comp->numresolutions; resno++) 
			{
				int dx, dy;
				iopj_pi_resolution_t *res = &comp->resolutions[resno];
				dx = comp->dx * (1 << (res->pdx + comp->numresolutions - 1 - resno));
				dy = comp->dy * (1 << (res->pdy + comp->numresolutions - 1 - resno));
				pi[pino].dx = !pi->dx ? dx : int_min(pi->dx, dx);
				pi[pino].dy = !pi->dy ? dy : int_min(pi->dy, dy);
			}
		}

		if (pino == 0) 
		{
			pi[pino].include = (short int*) sft_calloc(tcp->numlayers * pi[pino].step_l, sizeof(short int));
			if(!pi[pino].include) {
				pi_destroy(pi, cp, tileno);
				return NULL;
			}
		}
		else 
		{
			pi[pino].include = pi[pino - 1].include;
		}

		/* Generation of boundaries for each prog flag*/
		if(tcp->POC && ( cp->cinema || ((!cp->cinema) && (t2_mode == FINAL_PASS))))
		{
			tcp->pocs[pino].compS= tcp->pocs[pino].compno0;
			tcp->pocs[pino].compE= tcp->pocs[pino].compno1;
			tcp->pocs[pino].resS = tcp->pocs[pino].resno0;
			tcp->pocs[pino].resE = tcp->pocs[pino].resno1;
			tcp->pocs[pino].layE = tcp->pocs[pino].layno1;
			tcp->pocs[pino].prg  = tcp->pocs[pino].prg1;
			if (pino > 0)
				tcp->pocs[pino].layS = (tcp->pocs[pino].layE > tcp->pocs[pino - 1].layE) ? tcp->pocs[pino - 1].layE : 0;
		}else {
			tcp->pocs[pino].compS= 0;
			tcp->pocs[pino].compE= image->numcomps;
			tcp->pocs[pino].resS = 0;
			tcp->pocs[pino].resE = maxres;
			tcp->pocs[pino].layS = 0;
			tcp->pocs[pino].layE = tcp->numlayers;
			tcp->pocs[pino].prg  = tcp->prg;
		}
		tcp->pocs[pino].prcS = 0;
		tcp->pocs[pino].prcE = maxprec;;
		tcp->pocs[pino].txS = pi[pino].tx0;
		tcp->pocs[pino].txE = pi[pino].tx1;
		tcp->pocs[pino].tyS = pi[pino].ty0;
		tcp->pocs[pino].tyE = pi[pino].ty1;
		tcp->pocs[pino].dx = pi[pino].dx;
		tcp->pocs[pino].dy = pi[pino].dy;
	}
	return pi;
}



void pi_destroy(iopj_pi_iterator_t *pi, opj_cp_t *cp, int tileno)
{
	int compno, pino;
	opj_tcp_t *tcp = &cp->tcps[tileno];
	if(pi) {
		for (pino = 0; pino < tcp->numpocs + 1; pino++) {	
			if(pi[pino].comps) {
				for (compno = 0; compno < pi->numcomps; compno++) {
					iopj_pi_comp_t *comp = &pi[pino].comps[compno];
					if(comp->resolutions) {
						sft_free(comp->resolutions);
					}
				}
				sft_free(pi[pino].comps);
			}
		}
		if(pi->include) 
		{
			sft_free(pi->include);
		}
		sft_free(pi);
	}
}

opj_bool pi_next(iopj_pi_iterator_t * pi)
{
	return pi_next_lrcp(pi);
	/*
	switch (pi->poc.prg) {
	case LRCP:
	return pi_next_lrcp(pi);
	case RLCP:
	return pi_next_rlcp(pi);
	case RPCL:
	return pi_next_rpcl(pi);
	case PCRL:
	return pi_next_pcrl(pi);
	case CPRL:
	return pi_next_cprl(pi);
	case PROG_UNKNOWN:
	return OPJ_FALSE;
	}

	return OPJ_FALSE;*/
}

opj_bool pi_create_encode( iopj_pi_iterator_t *pi, opj_cp_t *cp,int tileno, int pino,int tpnum, int tppos, J2K_T2_MODE t2_mode,int cur_totnum_tp)
{
	//char prog[4];
	int i;
	int incr_top=1,resetX=0;
	opj_tcp_t *tcps =&cp->tcps[tileno];
	opj_poc_t *tcp= &tcps->pocs[pino];
	// JUAN : Only LRCP encoding
	pi[pino].first = 1;
	pi[pino].poc.prg = tcp->prg;
	//strncpy(prog, "LRCP",4);

	if(!(cp->tp_on && ((!cp->cinema && (t2_mode == FINAL_PASS)) || cp->cinema)))
	{
		pi[pino].poc.resno0 = tcp->resS;
		pi[pino].poc.resno1 = tcp->resE;
		pi[pino].poc.compno0 = tcp->compS;
		pi[pino].poc.compno1 = tcp->compE;
		pi[pino].poc.layno0 = tcp->layS;
		pi[pino].poc.layno1 = tcp->layE;
		pi[pino].poc.precno0 = tcp->prcS;
		pi[pino].poc.precno1 = tcp->prcE;
		pi[pino].poc.tx0 = tcp->txS;
		pi[pino].poc.ty0 = tcp->tyS;
		pi[pino].poc.tx1 = tcp->txE;
		pi[pino].poc.ty1 = tcp->tyE;
	}
	/*
	JUAN
	else 
	{

	}	
	*/
	return OPJ_FALSE;
}

