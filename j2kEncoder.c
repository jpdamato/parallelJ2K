
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <math.h>
#include "openjpeg.h"
#include "opj_malloc.h"
#include "sft_utils.h"

#include <time.h>
#include <omp.h>
//#include "u_ProcessTime.h"

#include "j2k.h"
#include "pi.h"
#include "int.h"
#include "sft_j2k_streamRW.h"


#ifdef _WIN32
#include "windirent.h"
#else
#include <dirent.h>
#endif /* _WIN32 */

#ifdef _WIN32
#include <windows.h>
#else
#include <strings.h>
#define _stricmp strcasecmp
#define _strnicmp strncasecmp
#endif /* _WIN32 */
#include "sft_j2k_Encoder.h"
#include "opj_config.h"
#include "openjpeg.h"
#include "opj_getopt.h"

#include "format_defs.h"
#include <omp.h>

#include "j2k.h"
#include "cio.h"
#include "tcd.h"
#include "sft_bitmapLoader.h"
#include "sft_j2k_streamRW.h"

#define CINEMA_24_CS 1302083	/*Codestream length for 24fps*/
#define CINEMA_48_CS 651041		/*Codestream length for 48fps*/
#define COMP_24_CS 1041666		/*Maximum size per color component for 2K & 4K @ 24fps*/
#define COMP_48_CS 520833		/*Maximum size per color component for 2K @ 48fps*/


int numConcurrentTasks = 1;

/**
sample error callback expecting a FILE* client object
*/
void perror_callback(const char *msg, void *client_data) 
{
	FILE *stream = (FILE*)client_data;
	fprintf(stream, "[ERROR] %s", msg);
}
/**
sample warning callback expecting a FILE* client object
*/
void pwarning_callback(const char *msg, void *client_data) {
	FILE *stream = (FILE*)client_data;
	fprintf(stream, "[WARNING] %s", msg);
}
/**
sample debug callback expecting a FILE* client object
*/
void pinfo_callback(const char *msg, void *client_data) {
	FILE *stream = (FILE*)client_data;
	fprintf(stream, "[INFO] %s", msg);
}

iopj_pi_iterator_t *ipi_initialise_encode(opj_image_t *image, opj_cp_t *cp, int tileno, J2K_T2_MODE t2_mode)
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
			if(!comp->resolutions) {
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


	/**	mem allocation for TLM marker*/
int ij2k_calculate_tp(opj_cp_t *cp,int img_numcomp,opj_image_t *image,opj_j2k_t *j2k )
{
	int pino,tileno,totnum_tp=0;

	OPJ_ARG_NOT_USED(img_numcomp);

	j2k->cur_totnum_tp = (int *) sft_malloc(cp->tw * cp->th * sizeof(int));
	for (tileno = 0; tileno < cp->tw * cp->th; tileno++) 
	{
		int cur_totnum_tp = 0;
		opj_tcp_t *tcp = &cp->tcps[tileno];
		for(pino = 0; pino <= tcp->numpocs; pino++) 
		{
			int tp_num=0;
			iopj_pi_iterator_t *pi = ipi_initialise_encode(image, cp, tileno,FINAL_PASS);
			if(!pi) { return -1;}
			tp_num = ij2k_get_num_tp(cp,pino,tileno);
			totnum_tp = totnum_tp + tp_num;
			cur_totnum_tp = cur_totnum_tp + tp_num;
			pi_destroy(pi, cp, tileno);
		}
		j2k->cur_totnum_tp[tileno] = cur_totnum_tp;

		/* << INDEX */
	}
	return totnum_tp;
}



opj_bool ij2k_encode(opj_j2k_t *j2k, opj_cio_t *cio, opj_image_t *image, opj_codestream_info_t *cstr_info) 
{
	int tileno, compno , tasks ;
	double clkstart ,clkend , acumEncodingTime, acumWritingTime;
	opj_cp_t *cp = NULL;
	opj_tcd_t* tcd[250];	// TCD component 
	
	char *numstr = NULL;
	double startTime = sft_clock() , endTime;
	j2k->cio = cio;	
	j2k->image = image;

	cp = j2k->cp;

	cio_enterSection("Preparing Heading..",12);
	
	//Preparing Headers
	// INDEX >> 
	ij2k_write_soc(j2k);
	ij2k_write_siz(j2k);
	ij2k_write_cod(j2k,0);
	ij2k_write_qcd(j2k,0);
	
	for (compno = 0; compno < image->numcomps; compno++) 
	{
		opj_tcp_t *tcp = &cp->tcps[0];
		if (tcp->tccps[compno].roishift)
			ij2k_write_rgn(j2k, compno, 0);
	}

	if (cp->comment != NULL) 
	{

		ij2k_write_com(j2k);
	}

	j2k->totnum_tp = ij2k_calculate_tp(cp,image->numcomps,image,j2k);

	// create the tile encoder 	
	
	omp_set_num_threads(numConcurrentTasks);
	numConcurrentTasks = omp_get_max_threads();
	
	endTime = sft_clock() - startTime;
	printf("J2K writing time %f \n" , endTime);

	// encode each tile 
	// Aca viene lo bueno!!!
	
	cio_leaveSection();
	numstr= (char*)(malloc(21));
	startTime = sft_clock() ;

	// Preparing data!!
	for (tasks = 0 ; tasks < numConcurrentTasks ; tasks ++)
	{
		tcd[tasks] = itcd_create(j2k->cinfo);
		tcd[tasks]->image = image;
		tcd[tasks]->cp = cp;
		tcd[tasks]->tcd_image->tw = cp->tw;
		tcd[tasks]->tcd_image->th = cp->th;
		//Deprecated :tcd->cur_totnum_tp = j2k->cur_totnum_tp[tileno];
		itcd_malloc_encode(j2k , tcd[tasks], image, cp, tasks);
	}

	endTime = sft_clock() - startTime;
	printf("J2K allocating Tiles time %f \n" , endTime);
			
	acumEncodingTime  =0.0;
	acumWritingTime = 0.0;
	for (tileno = 0; tileno < cp->tw * cp->th; tileno  += numConcurrentTasks ) 
	{	 
		//sprintf(numstr, "Tile no%d", tileno);
		//cio_enterSection(numstr,sizeof(numstr));
		//sft_enterSection("Tile");
		clkstart = sft_clock();
		#pragma omp parallel for 
		for ( tasks = 0 ; tasks<numConcurrentTasks ; tasks ++)
         if (tileno + tasks <cp->tw * cp->th ) 
 		   ij2k_encodeTile(j2k->cp ,tileno + tasks, j2k,tcd[tasks], image);		
		
		clkend = sft_clock();
		acumEncodingTime += clkend - clkstart;		
				
		clkstart = sft_clock();
		for ( tasks = 0 ; tasks<numConcurrentTasks ; tasks ++)
		{
		   if (tileno + tasks <cp->tw * cp->th ) 
			 ij2k_writeToCio(tileno + tasks, j2k,tcd[tasks], image);				   
		}
		clkend = sft_clock();
		acumWritingTime += clkend - clkstart;		
		//sft_leaveSection();	
		//cio_leaveSection();
	}

	fprintf(stderr, "TCD Encoding %d tiles  %f . Avg : %f secs. \n" ,cp->tw * cp->th , acumEncodingTime , acumEncodingTime /(cp->tw * cp->th)  );
	fprintf(stderr, "TCD Writing CIO  %f  \n" ,acumWritingTime );
	
	// destroy the tile encoder 
	for (tasks = 0 ; tasks < numConcurrentTasks ; tasks ++)
	{
		itcd_free_encode(tcd[tasks]);
		itcd_destroy(tcd[tasks]);
	}

	sft_free(j2k->cur_totnum_tp);
	cio_enterSection("Closing stream..",12);
	ij2k_write_eoc(j2k);
	cio_leaveSection();
	return OPJ_TRUE;
}

/* ----------------------------------------------------------------------- */
/* J2K encoder interface                                                       */
/* ----------------------------------------------------------------------- */

opj_j2k_t* ij2k_create_compress(opj_common_ptr cinfo) 
{
	opj_j2k_t *j2k = (opj_j2k_t*) sft_calloc(1, sizeof(opj_j2k_t));
	if(j2k) {
		j2k->cinfo = cinfo;
	}
	return j2k;
}

void ij2k_destroy_compress(opj_j2k_t *j2k) {
	int tileno;

	if(!j2k) return;
	if(j2k->cp != NULL) {
		opj_cp_t *cp = j2k->cp;

		if(cp->comment) {
			sft_free(cp->comment);
		}
		if(cp->matrice) {
			sft_free(cp->matrice);
		}
		for (tileno = 0; tileno < cp->tw * cp->th; tileno++) {
			sft_free(cp->tcps[tileno].tccps);
		}
		sft_free(cp->tcps);
		sft_free(cp);
	}

	sft_free(j2k);
}


opj_cinfo_t* OPJ_CALLCONV iopj_create_compress(OPJ_CODEC_FORMAT format)
{
	opj_cinfo_t *cinfo = (opj_cinfo_t*)sft_calloc(1, sizeof(opj_cinfo_t));
	if(!cinfo) return NULL;
	cinfo->is_decompressor = OPJ_FALSE;
	switch(format) {
		case CODEC_J2K:
			/* get a J2K coder handle */
			cinfo->j2k_handle = (void*)ij2k_create_compress((opj_common_ptr)cinfo);
			if(!cinfo->j2k_handle) {
				sft_free(cinfo);
				return NULL;
			}
			break;
		case CODEC_JP2:
			/* get a JP2 coder handle */
		//	cinfo->jp2_handle = (void*)jp2_create_compress((opj_common_ptr)cinfo);
			if(!cinfo->jp2_handle) {
				sft_free(cinfo);
				return NULL;
			}
			break;
		case CODEC_JPT:
		case CODEC_UNKNOWN:
		default:
			sft_free(cinfo);
			return NULL;
	}

	cinfo->codec_format = format;

	return cinfo;
}

void OPJ_CALLCONV iopj_set_default_encoder_parameters(opj_cparameters_t *parameters) 
{
	if(parameters) 
	{
		memset(parameters, 0, sizeof(opj_cparameters_t));
		/* default coding parameters */
		parameters->cp_cinema = OFF; 
		parameters->max_comp_size = 0;
		parameters->numresolution = 6;
		parameters->cp_rsiz = STD_RSIZ;
		parameters->cblockw_init = 64;
		parameters->cblockh_init = 64;
		parameters->prog_order = LRCP;
		parameters->roi_compno = -1;		/* no ROI */
		parameters->subsampling_dx = 1;
		parameters->subsampling_dy = 1;
		parameters->tp_on = 0;
		parameters->decod_format = -1;
		parameters->cod_format = -1;
		parameters->tcp_rates[0] = 0;   
		parameters->tcp_numlayers = 0;
    parameters->cp_disto_alloc = 0;
		parameters->cp_fixed_alloc = 0;
		parameters->cp_fixed_quality = 0;
		parameters->jpip_on = OPJ_FALSE;
/* UniPG>> */
#ifdef USE_JPWL

#endif /* USE_JPWL */
	}
}

void ij2k_setup_encoder(opj_j2k_t *j2k, opj_cparameters_t *parameters, opj_image_t *image) 
{
	int i, j, tileno, numpocs_tile;
	opj_cp_t *cp = NULL;

	if(!j2k || !parameters || ! image) {
		return;
	}

	/* create and initialize the coding parameters structure */
	cp = (opj_cp_t*) sft_calloc(1, sizeof(opj_cp_t));

	/* keep a link to cp so that we can destroy it later in j2k_destroy_compress */
	j2k->cp = cp;

	/* set default values for cp */
	cp->tw = 1;
	cp->th = 1;

	/* 
	copy user encoding parameters 
	*/
	cp->cinema = parameters->cp_cinema;
	cp->max_comp_size =	parameters->max_comp_size;
	cp->rsiz   = parameters->cp_rsiz;
	cp->disto_alloc = parameters->cp_disto_alloc;
	cp->fixed_alloc = parameters->cp_fixed_alloc;
	cp->fixed_quality = parameters->cp_fixed_quality;

	/* mod fixed_quality */
	if(parameters->cp_matrice) {
		size_t array_size = parameters->tcp_numlayers * parameters->numresolution * 3 * sizeof(int);
		cp->matrice = (int *) sft_malloc(array_size);
		memcpy(cp->matrice, parameters->cp_matrice, array_size);
	}

	/* tiles */
	cp->tdx = parameters->cp_tdx;
	cp->tdy = parameters->cp_tdy;

	/* tile offset */
	cp->tx0 = parameters->cp_tx0;
	cp->ty0 = parameters->cp_ty0;

	/* comment string */
	if(parameters->cp_comment) {
		cp->comment = (char*)sft_malloc(strlen(parameters->cp_comment) + 1);
		if(cp->comment) {
			strcpy(cp->comment, parameters->cp_comment);
		}
	}

	/*
	calculate other encoding parameters
	*/

	if (parameters->tile_size_on) {
		cp->tw = int_ceildiv(image->x1 - cp->tx0, cp->tdx);
		cp->th = int_ceildiv(image->y1 - cp->ty0, cp->tdy);
	} else {
		cp->tdx = image->x1 - cp->tx0;
		cp->tdy = image->y1 - cp->ty0;
	}

	if(parameters->tp_on){
		cp->tp_flag = parameters->tp_flag;
		cp->tp_on = 1;
	}
	
	cp->img_size = 0;
	for(i=0;i<image->numcomps ;i++){
	cp->img_size += (image->comps[i].w *image->comps[i].h * image->comps[i].prec);
	}

 /* USE_JPWL */

	/* initialize the mutiple tiles */
	/* ---------------------------- */
	cp->tcps = (opj_tcp_t*) sft_calloc(cp->tw * cp->th, sizeof(opj_tcp_t));

	for (tileno = 0; tileno < cp->tw * cp->th; tileno++) {
		opj_tcp_t *tcp = &cp->tcps[tileno];
		tcp->numlayers = parameters->tcp_numlayers;
		for (j = 0; j < tcp->numlayers; j++) 
		{
			
					tcp->rates[j] = parameters->tcp_rates[j];
			
		}
		tcp->csty = parameters->csty;
		tcp->prg = parameters->prog_order;
		tcp->mct = parameters->tcp_mct; 

		numpocs_tile = 0;
		tcp->POC = 0;
		if (parameters->numpocs) {
			/* initialisation of POC */
			tcp->POC = 1;
			for (i = 0; i < parameters->numpocs; i++) {
				if((tileno == parameters->POC[i].tile - 1) || (parameters->POC[i].tile == -1)) {
					opj_poc_t *tcp_poc = &tcp->pocs[numpocs_tile];
					tcp_poc->resno0		= parameters->POC[numpocs_tile].resno0;
					tcp_poc->compno0	= parameters->POC[numpocs_tile].compno0;
					tcp_poc->layno1		= parameters->POC[numpocs_tile].layno1;
					tcp_poc->resno1		= parameters->POC[numpocs_tile].resno1;
					tcp_poc->compno1	= parameters->POC[numpocs_tile].compno1;
					tcp_poc->prg1		= parameters->POC[numpocs_tile].prg1;
					tcp_poc->tile		= parameters->POC[numpocs_tile].tile;
					numpocs_tile++;
				}
			}
			tcp->numpocs = numpocs_tile -1 ;
		}else{ 
			tcp->numpocs = 0;
		}

		tcp->tccps = (opj_tccp_t*) sft_calloc(image->numcomps, sizeof(opj_tccp_t));

		for (i = 0; i < image->numcomps; i++) {
			opj_tccp_t *tccp = &tcp->tccps[i];
			tccp->csty = parameters->csty & 0x01;	/* 0 => one precinct || 1 => custom precinct  */
			tccp->numresolutions = parameters->numresolution;
			tccp->cblkw = int_floorlog2(parameters->cblockw_init);
			tccp->cblkh = int_floorlog2(parameters->cblockh_init);
			tccp->cblksty = parameters->mode;
			tccp->qmfbid = parameters->irreversible ? 0 : 1;
			tccp->qntsty = parameters->irreversible ? J2K_CCP_QNTSTY_SEQNT : J2K_CCP_QNTSTY_NOQNT;
			tccp->numgbits = 2;
			if (i == parameters->roi_compno) {
				tccp->roishift = parameters->roi_shift;
			} else {
				tccp->roishift = 0;
			}

			if(parameters->cp_cinema)
			{
				/*Precinct size for lowest frequency subband=128*/
				tccp->prcw[0] = 7;
				tccp->prch[0] = 7;
				/*Precinct size at all other resolutions = 256*/
				for (j = 1; j < tccp->numresolutions; j++) {
					tccp->prcw[j] = 8;
					tccp->prch[j] = 8;
				}
			}else{
				if (parameters->csty & J2K_CCP_CSTY_PRT) {
					int p = 0;
					for (j = tccp->numresolutions - 1; j >= 0; j--) {
						if (p < parameters->res_spec) {
							
							if (parameters->prcw_init[p] < 1) {
								tccp->prcw[j] = 1;
							} else {
								tccp->prcw[j] = int_floorlog2(parameters->prcw_init[p]);
							}
							
							if (parameters->prch_init[p] < 1) {
								tccp->prch[j] = 1;
							}else {
								tccp->prch[j] = int_floorlog2(parameters->prch_init[p]);
							}

						} else {
							int res_spec = parameters->res_spec;
							int size_prcw = parameters->prcw_init[res_spec - 1] >> (p - (res_spec - 1));
							int size_prch = parameters->prch_init[res_spec - 1] >> (p - (res_spec - 1));
							
							if (size_prcw < 1) {
								tccp->prcw[j] = 1;
							} else {
								tccp->prcw[j] = int_floorlog2(size_prcw);
							}
							
							if (size_prch < 1) {
								tccp->prch[j] = 1;
							} else {
								tccp->prch[j] = int_floorlog2(size_prch);
							}
						}
						p++;
						/*printf("\nsize precinct for level %d : %d,%d\n", j,tccp->prcw[j], tccp->prch[j]); */
					}	/*end for*/
				} else {
					for (j = 0; j < tccp->numresolutions; j++) {
						tccp->prcw[j] = 15;
						tccp->prch[j] = 15;
					}
				}
			}

			idwt_calc_explicit_stepsizes(tccp, image->comps[i].prec);
		}
	}
}

void OPJ_CALLCONV iopj_setup_encoder(opj_cinfo_t *cinfo, opj_cparameters_t *parameters, opj_image_t *image) 
{
	//JUAN
	
	if(cinfo && parameters && image) {
		switch(cinfo->codec_format) {
			case CODEC_J2K:
				ij2k_setup_encoder((opj_j2k_t*)cinfo->j2k_handle, parameters, image);
				break;
			case CODEC_JP2:
	//			jp2_setup_encoder((opj_jp2_t*)cinfo->jp2_handle, parameters, image);
				break;
			case CODEC_JPT:
			case CODEC_UNKNOWN:
			default:
				break;
		}
	}
	
}


int main(int argc, char **argv) 
{
	opj_bool bSuccess;
	opj_cparameters_t* parameters;	// compression parameters 
	img_fol_t img_fol;
	opj_event_mgr_t event_mgr;		// event manager 
	opj_image_t *image = NULL ;
	char *image2 = NULL;
	int num_images , iter ,offset;	
	dircnt_t *dirptr = NULL;
	
	opj_codestream_info_t cstr_info;		// Codestream information structure 
	char indexfilename[OPJ_PATH_LEN];	// index file name 
	/** input file name */
	char infile[OPJ_PATH_LEN];
	/** output file name */
	char outfile[OPJ_PATH_LEN];

	// JP2 format output 
	int codestream_length ;	
	size_t res;
	opj_cio_t *cio = NULL;
	FILE *f = NULL;
	opj_cinfo_t *cinfo = NULL;
	//configure the event callbacks (not required)
	//setting of each callback is optionnal

	double initialTime   , tTime;

	memset(&event_mgr, 0, sizeof(opj_event_mgr_t));
	event_mgr.error_handler = perror_callback;
	event_mgr.warning_handler = pwarning_callback;
	event_mgr.info_handler = pinfo_callback;

	
	// Initialize indexfilename and img_fol 
	*indexfilename = 0;
	memset(&img_fol,0,sizeof(img_fol_t));
	//Alojo 40 MB
	sft_preAlloc((360*1024*1024) / numConcurrentTasks,numConcurrentTasks,80* 1024 *1024);
	
	for ( iter = 1 ; iter < 3	; iter ++ )
	{
		
		sft_resetAllocated();
		parameters = (opj_cparameters_t*) sft_malloc(sizeof(opj_cparameters_t));
		// set encoding parameters to default values 
		iopj_set_default_encoder_parameters(parameters);
		
		// Seteo parametros por defecto
		parameters->decod_format = BMP_DFMT;
		parameters->cod_format = J2K_CFMT;

		sprintf(infile,"%s%d%s","I:/Trabajo/Test" ,iter , ".bmp");
		//sprintf(parameters.infile,"%s%d%s","Test" ,iter , ".bmp");
		
		sprintf(outfile,"%s%d%s","I:/Trabajo/Test" ,iter , ".j2k");
		//sprintf(parameters.outfile,"%s%d%s","Test" ,iter , ".j2k");

		parameters->cblockw_init = 64;
		parameters->cblockh_init = 64;
		parameters->tile_size_on = OPJ_TRUE;
		parameters->tcp_rates[0] = 0;	// MOD antonin : losslessbug 
		parameters->tcp_numlayers++;
		parameters->cp_disto_alloc = 1;

		parameters->cp_tdx = 64	;
		parameters->cp_tdy = 64 ;
		parameters->cp_disto_alloc = 1;

		// Create comment for codestream 
		if(parameters->cp_comment == NULL) 
		{
			const char comment[] = "Created by OpenJPEG version ";
			const size_t clen = strlen(comment);
			const char *version = opj_version();
			parameters->cp_comment = (char*)sft_malloc(clen+strlen(version)+1);
			sprintf(parameters->cp_comment,"%s%s", comment, version);
		}

		num_images=1;

		//Encoding image one by one
		// decode the source image 
		// ----------------------- 
		image = ibmptoimage(infile, parameters);
		offset = ((char*)image -  (char*)sft_base()) ;
		image2 = (opj_image_t*)( sft_base() );
		image2+= offset  ;
		//image = NULL;

		if( !image)
		{
			fprintf(stderr, "Unable to load file: got no image\n");
			return 1;
		}

		if( ((opj_image_t*)(image2))->x0 != image->x0)
		{
			fprintf(stderr, "Unable to load file: got no image\n");
			return 1;
		}

		// Decide if MCT should be used 
		if ( image->numcomps == 3 )
			parameters->tcp_mct = 1 ;
		else 
			parameters->tcp_mct = 0 ;
				
		initialTime = sft_clock() ;
	
		// get a J2K compressor handle 
		cinfo = iopj_create_compress(CODEC_J2K);

		// catch events using our callbacks and give a local context 
		opj_set_event_mgr((opj_common_ptr)cinfo, &event_mgr, stderr);

		// setup the encoder parameters using the current image and user parameters 
		iopj_setup_encoder(cinfo, parameters, image);

		// open a byte stream for writing 
		// allocate memory for all tiles 
		cio_enterSection("opj_cio_open" , 12);
		cio = opj_cio_open((opj_common_ptr)cinfo, NULL, 0);

		cio_leaveSection();
		// encode the image 
	
		bSuccess = ij2k_encode((opj_j2k_t*)cinfo->j2k_handle, cio, image, NULL);

		if (!bSuccess)
		{
			opj_cio_close(cio);
			fprintf(stderr, "failed to encode image\n");
			return 1;
		}
		sft_printAvailableMem();
		codestream_length = cio_tell(cio);

		tTime = sft_clock() - initialTime ;

		printf("Total time %f \n" , tTime);

		// write the buffer to disk 
		f = fopen(outfile, "wb");
		if (!f) 
		{
			fprintf(stderr, "failed to open %s for writing\n", outfile);
			return 1;
		}
		res = fwrite(cio->buffer, 1, codestream_length, f);
		if( res < (size_t)codestream_length ) 
		{ // FIXME 
			fprintf(stderr, "failed to write %d (%s)\n", codestream_length, outfile);
			return 1;
		}
		fclose(f);
	
		fprintf(stderr,"Generated outfile %s\n",outfile);
		fprintf(stderr,"Used memmory for coding  %d\n",totalMemPerTile);

	
		// close and free the byte stream 
		opj_cio_close(cio);

		fprintf(stderr, "Used memory [%d] in slots [%d]\n", sft_memoccupation() ,sft_memSlots());
		//sft_printResults(0,1600);
		//sft_printSections1();
		//cio_printOut();
		//sft_printResults(0,200);
		//sft_printSections();
	
		// free remaining compression structures 
		opj_destroy_compress(cinfo);
		

		//getchar();
		// free image data 
		iopj_image_destroy(image);

		// free user parameters structure 
		if(parameters->cp_comment) sft_free(parameters->cp_comment);
		if(parameters->cp_matrice) sft_free(parameters->cp_matrice);
	}
	getchar();
	return 0;
}