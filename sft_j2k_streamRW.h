#include "j2k.h"
#include "openjpeg.h"
#include "tcd.h"

int ij2k_get_num_tp(opj_cp_t *cp,int pino,int tileno);
void ij2k_write_tlm(opj_j2k_t *j2k);
void ij2k_write_soc(opj_j2k_t *j2k) ;
void ij2k_write_siz(opj_j2k_t *j2k) ;
void ij2k_write_sod(opj_j2k_t *j2k, opj_tcd_t* tcd, int tileno, int tp_num , int cur_tp_num);
void ij2k_write_cod(opj_j2k_t *j2k,int curtileno) ;
void ij2k_write_qcd(opj_j2k_t *j2k, int curtileno) ;
void ij2k_write_com(opj_j2k_t *j2k) ;
void ij2k_write_qcc(opj_j2k_t *j2k, int compno, int curtileno);
void ij2k_write_coc(opj_j2k_t *j2k, int compno, int curtileno);
void ij2k_write_sot(opj_cio_t *cio , opj_j2k_t *j2k, int curtileno, int cur_tp_num);
void ij2k_write_poc(opj_j2k_t *j2k, int curtileno);
void ij2k_write_eoc(opj_j2k_t *j2k) ;
void ij2k_write_rgn(opj_j2k_t *j2k, int compno, int tileno) ;
void ij2k_writeToCio(int tileno , opj_j2k_t *j2k , opj_tcd_t *tcd,const opj_image_t *image );