#include "opj_config.h"
#include "openjpeg.h"
#include "opj_getopt.h"

#include "format_defs.h"
#include <omp.h>

#include "j2k.h"
#include "cio.h"
#include "tcd.h"
#include "mqc.h"
#include "raw.h"

typedef short flag_t;

#define iMACRO_t1_flags(x,y) t1->flags[((x)*(t1->flags_stride))+(y)]

/* Are restricted pointers available? (C99) */
#if (__STDC_VERSION__ != 199901L)
	/* Not a C99 compiler */
	#ifdef __GNUC__
		#define restrict __restrict__
	#else
		#define restrict /* restrict */
	#endif
#endif

/**
Tag node
*/
typedef struct iopj_tgt_node {
  struct iopj_tgt_node *parent;
  int value;
  int low;
  int known;
} iopj_tgt_node_t;

/**
Tag tree
*/
typedef struct iopj_tgt_tree 
{
  int numleafsh;
  int numleafsv;
  int numnodes;
  iopj_tgt_node_t *nodes;
} iopj_tgt_tree_t;


/* Routines that are to be used by both halves of the library are declared
 * to receive a pointer to this structure.  There are no actual instances of
 * opj_common_struct_t, only of opj_cinfo_t and opj_dinfo_t.
 */
typedef struct iopj_common_struct 
{
	opj_event_mgr_t *event_mgr;	/**< pointer to the event manager */\
	void * client_data;			/**< Available for use by application */\
	opj_bool is_decompressor;	/**< So common code can tell which is which */\
	OPJ_CODEC_FORMAT codec_format;	/**< selected codec */\
	void *j2k_handle;			/**< pointer to the J2K codec */\
	void *jp2_handle;			/**< pointer to the JP2 codec */\
	void *mj2_handle;			/**< pointer to the MJ2 codec */
		/* Fields common to both master struct types */
  /* Additional fields follow in an actual opj_cinfo_t or
   * opj_dinfo_t.  All three structs must agree on these
   * initial fields!  (This would be a lot cleaner in C++.)
   */
} iopj_common_struct_t;

typedef iopj_common_struct_t * iopj_common_ptr;

/**
Byte input-output stream (CIO)
*/
typedef struct iopj_cio 
{
	/** codec context */
	iopj_common_ptr cinfo;

	/** open mode (read/write) either OPJ_STREAM_READ or OPJ_STREAM_WRITE */
	int openmode;
	/** pointer to the start of the buffer */
	unsigned char *buffer;
	/** buffer size in bytes */
	int length;

	/** pointer to the start of the stream */
	unsigned char *start;
	/** pointer to the end of the stream */
	unsigned char *end;
	/** pointer to the current position */
	unsigned char *bp;
} iopj_cio_t;	

/**
Index structure : information regarding tiles 
*/
typedef struct iopj_tile_info {
	/** value of thresh for each layer by tile cfr. Marcela   */
	double *thresh;
	/** number of tile */
	int tileno;
	/** start position */
	int start_pos;
	/** end position of the header */
	int end_header;
	/** end position */
	int end_pos;
	/** precinct number for each resolution level (width) */
	int pw[33];
	/** precinct number for each resolution level (height) */
	int ph[33];
	/** precinct size (in power of 2), in X for each resolution level */
	int pdx[33];
	/** precinct size (in power of 2), in Y for each resolution level */
	int pdy[33];
	/** information concerning packets inside tile */
	opj_packet_info_t *packet;
	/** add fixed_quality */
	int numpix;
	/** add fixed_quality */
	double distotile;
  	/** number of markers */
	int marknum;
	/** list of markers */
	opj_marker_info_t *marker;
	/** actual size of markers array */
	int maxmarknum;
	/** number of tile parts */
	int num_tps;
	/** information concerning tile parts */
	opj_tp_info_t *tp;
} iopj_tile_info_t;
/**
Index structure of the codestream
*/
typedef struct iopj_codestream_info {
	/** maximum distortion reduction on the whole image (add for Marcela) */
	double D_max;
	/** packet number */
	int packno;
	/** writing the packet in the index with t2_encode_packets */
	int index_write;
	/** image width */
	int image_w;
	/** image height */
	int image_h;
	/** progression order */
	OPJ_PROG_ORDER prog;
	/** tile size in x */
	int tile_x;
	/** tile size in y */
	int tile_y;
	/** */
	int tile_Ox;
	/** */
	int tile_Oy;
	/** number of tiles in X */
	int tw;
	/** number of tiles in Y */
	int th;
	/** component numbers */
	int numcomps;
	/** number of layer */
	int numlayers;
	/** number of decomposition for each component */
	int *numdecompos;
/* UniPG>> */
	/** number of markers */
	int marknum;
	/** list of markers */
	opj_marker_info_t *marker;
	/** actual size of markers array */
	int maxmarknum;
/* <<UniPG */
	/** main header position */
	int main_head_start;
	/** main header position */
	int main_head_end;
	/** codestream's size */
	int codestream_size;
	/** information regarding tiles inside image */
	iopj_tile_info_t *tile;
} iopj_codestream_info_t;

/**
Coding parameters
*/
typedef struct iopj_cp 
{
	/** Digital cinema profile*/
	OPJ_CINEMA_MODE cinema;
	/** Maximum rate for each component. If == 0, component size limitation is not considered */
	int max_comp_size;
	/** Size of the image in bits*/
	int img_size;
	/** Rsiz*/
	OPJ_RSIZ_CAPABILITIES rsiz;
	/** Enabling Tile part generation*/
	char tp_on;
	/** Flag determining tile part generation*/
	char tp_flag;
	/** Position of tile part flag in progression order*/
	int tp_pos;
	/** allocation by rate/distortion */
	int disto_alloc;
	/** allocation by fixed layer */
	int fixed_alloc;
	/** add fixed_quality */
	int fixed_quality;
	/** if != 0, then original dimension divided by 2^(reduce); if == 0 or not used, image is decoded to the full resolution */
	int reduce;
	/** if != 0, then only the first "layer" layers are decoded; if == 0 or not used, all the quality layers are decoded */
	int layer;
	/** if == NO_LIMITATION, decode entire codestream; if == LIMIT_TO_MAIN_HEADER then only decode the main header */
	OPJ_LIMIT_DECODING limit_decoding;
	/** XTOsiz */
	int tx0;
	/** YTOsiz */
	int ty0;
	/** XTsiz */
	int tdx;
	/** YTsiz */
	int tdy;
	/** comment for coding */
	char *comment;
	/** number of tiles in width */
	int tw;
	/** number of tiles in heigth */
	int th;
	/** ID number of the tiles present in the codestream */
	int *tileno;
	/** size of the vector tileno */
	int tileno_size;
	/** packet header store there for futur use in t2_decode_packet */
	unsigned char *ppm_data;
	/** pointer remaining on the first byte of the first header if ppm is used */
	unsigned char *ppm_data_first;
	/** if ppm == 1 --> there was a PPM marker for the present tile */
	int ppm;
	/** use in case of multiple marker PPM (number of info already store) */
	int ppm_store;
	/** use in case of multiple marker PPM (case on non-finished previous info) */
	int ppm_previous;
	/** ppmbug1 */
	int ppm_len;
	/** tile coding parameters */
	opj_tcp_t *tcps;
	/** fixed layer */
	int *matrice;
/* UniPG>> */
#ifdef USE_JPWL
#endif /* USE_JPWL */
/* <<UniPG */
} iopj_cp_t;
/** 
Defines image data and characteristics
*/
typedef struct iopj_image {
	/** XOsiz: horizontal offset from the origin of the reference grid to the left side of the image area */
	int x0;
	/** YOsiz: vertical offset from the origin of the reference grid to the top side of the image area */
	int y0;
	/** Xsiz: width of the reference grid */
	int x1;
	/** Ysiz: height of the reference grid */
	int y1;
	/** number of components in the image */
	int numcomps;
	/** color space: sRGB, Greyscale or YUV */
	OPJ_COLOR_SPACE color_space;
	/** image components */
	opj_image_comp_t *comps;
	/** 'restricted' ICC profile */
	unsigned char *icc_profile_buf;
	/** size of ICC profile */
	int icc_profile_len;
} iopj_image_t;

/**
Tile coding parameters : 
this structure is used to store coding/decoding parameters common to all
tiles (information like COD, COC in main header)
*/
typedef struct iopj_tcp {
	/** 1 : first part-tile of a tile */
	int first;
	/** coding style */
	int csty;
	/** progression order */
	OPJ_PROG_ORDER prg;
	/** number of layers */
	int numlayers;
	/** multi-component transform identifier */
	int mct;
	/** rates of layers */
	float rates[100];
	/** number of progression order changes */
	int numpocs;
	/** indicates if a POC marker has been used O:NO, 1:YES */
	int POC;
	/** progression order changes */
	opj_poc_t pocs[32];
	/** packet header store there for futur use in t2_decode_packet */
	unsigned char *ppt_data;
	/** pointer remaining on the first byte of the first header if ppt is used */
	unsigned char *ppt_data_first;
	/** If ppt == 1 --> there was a PPT marker for the present tile */
	int ppt;
	/** used in case of multiple marker PPT (number of info already stored) */
	int ppt_store;
	/** ppmbug1 */
	int ppt_len;
	/** add fixed_quality */
	float distoratio[100];
	/** tile-component coding parameters */
	opj_tccp_t *tccps;
} iopj_tcp_t;
/**
JPEG-2000 codestream reader/writer
*/
typedef struct iopj_j2k 
{
	// codec context 
	iopj_common_ptr cinfo;
	// locate in which part of the codestream the decoder is (main header, tile header, end) 
	int state;
	// number of the tile curently concern by coding/decoding 
	//int curtileno;
	/** Tile part number*/
	//int tp_num;
	/** Tilepart number currently coding*/
	//int cur_tp_num;
	// Total number of tileparts of the current tile
	int *cur_totnum_tp;
	//	locate the start position of the TLM marker after encoding the tilepart, a jump (in j2k_write_sod) is done to the TLM marker to store the value of its length. 	
	int tlm_start;
	// Total num of tile parts in whole image = num tiles* num tileparts in each tile used in TLMmarker
	int totnum_tp;	
	// 	locate the position of the end of the tile in the codestream, used to detect a truncated codestream (in j2k_read_sod)	
	unsigned char *eot;
	// 	locate the start position of the SOT marker of the current coded tile:  after encoding the tile, a jump (in j2k_write_sod) is done to the SOT marker to store the value of its length. 	
	int sot_start;
	int sod_start;
	// as the J2K-file is written in several parts during encoding,	it enables to make the right correction in position return by cio_tell
	
	int pos_correction;
	// array used to store the data of each tile 
	unsigned char **tile_data;
	// array used to store the length of each tile 
	int *tile_len;
	//decompression only : store decoding parameters common to all tiles (information like COD, COC in main header)
	
	iopj_tcp_t *default_tcp;
	// pointer to the encoded / decoded image 
	iopj_image_t *image;
	// pointer to the coding parameters 
	iopj_cp_t *cp;
	// helper used to write the index file 
	iopj_codestream_info_t *cstr_info;
	// pointer to the byte i/o stream 
	iopj_cio_t *cio;
} iopj_j2k_t;

/** @name Local data structures */
/*@{*/

typedef struct idwt_local {
	int* mem;
	int dn;
	int sn;
	int cas;
} idwt_t;

/**
Tier-1 coding (coding of code-block coefficients)
*/
typedef struct iopj_t1 {
	/** codec context */
	opj_common_ptr cinfo;

	/** MQC component */
	opj_mqc_t *mqc;
	/** RAW component */
	opj_raw_t *raw;

	int *data;
	flag_t *flags;
	int w;
	int h;
	int datasize;
	int flagssize;
	int flags_stride;
} iopj_t1_t;

typedef struct iopj_t2 {
	/** codec context */
	opj_common_ptr cinfo;

	/** Encoding: pointer to the src image. Decoding: pointer to the dst image. */
	opj_image_t *image;
	/** pointer to the image coding parameters */
	opj_cp_t *cp;
} iopj_t2_t;

static int totalMemPerTile = 0;

void ij2k_encodeTile(const opj_cp_t *cp  ,int tileno , opj_j2k_t *j2k , opj_tcd_t *gtcd,const opj_image_t *image );

opj_tgt_tree_t *itgt_create(int numleafsh, int numleafsv);

void itcd_malloc_encode(opj_j2k_t *j2k, opj_tcd_t *tcd,const opj_image_t * image, opj_cp_t * cp, int curtileno);
void itcd_init_encode(const opj_tcd_t *tcd,const opj_image_t * image,const opj_cp_t * cp, int curtileno);

void ij2k_encodeTile(const opj_cp_t *cp  ,int tileno , opj_j2k_t *j2k , opj_tcd_t *gtcd,const opj_image_t *image ) ;

void itgt_reset(opj_tgt_tree_t *tree) ;
void itgt_destroy(opj_tgt_tree_t *tree) ;
opj_tgt_tree_t *itgt_create(int numleafsh, int numleafsv);
void idwt_encode_1(int *a, int dn, int sn, int cas)  ;
void idwt_interleave_h(idwt_t* h, int *a) ;
void idwt_interleave_v(idwt_t* v, int *a, int x);
void idwt_deinterleave_v(int *a, int *b, int dn, int sn, int x, int cas) ;
void idwt_deinterleave_h(int *a, int *b, int dn, int sn, int cas) ;

void idwt_calc_explicit_stepsizes(opj_tccp_t * tccp, int prec);

// T1 Tier functions
void it1_encode_cblks(	iopj_t1_t *t1,	opj_tcd_tile_t *tile,	opj_tcp_t *tcp) ;

void it1_enc_refpass_step(iopj_t1_t *t1,flag_t *flagsp,	int *datap,	int bpno,	int one,
	int *nmsedec,	char type,	int vsc);
void it1_enc_refpass(	iopj_t1_t *t1,	int bpno,	int *nmsedec,	char type,	int cblksty);
opj_bool iallocate_buffers(iopj_t1_t *t1,	int w,	int h);

opj_tcd_t* itcd_create(opj_common_ptr cinfo)  ;
void itcd_destroy(opj_tcd_t *tcd) ;
void itcd_malloc_encode(opj_j2k_t *j2k, opj_tcd_t *tcd,const  opj_image_t * image, opj_cp_t * cp, int curtileno) ;
void itcd_free_encode(opj_tcd_t *tcd);

