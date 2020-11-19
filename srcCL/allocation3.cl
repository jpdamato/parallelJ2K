#define GPU 1
#define NULL 0
#pragma OPENCL EXTENSION cl_khr_fp64 : enable

#if !defined GPU
#include <math.h>
#endif

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

#define SFT_IMAGE 0

#define OPJ_TRUE 1
#define OPJ_FALSE 0


#define J2K_CFMT 0
#define JP2_CFMT 1
#define JPT_CFMT 2

#define PXM_DFMT 10
#define PGX_DFMT 11
#define BMP_DFMT 12
#define YUV_DFMT 13
#define TIF_DFMT 14
#define RAW_DFMT 15
#define TGA_DFMT 16
#define PNG_DFMT 17

/**
@file event.h
@brief Implementation of a event callback system

The functions in EVENT.C have for goal to send output messages (errors, warnings, debug) to the user.
*/

#define EVT_ERROR	1	/**< Error event type */
#define EVT_WARNING	2	/**< Warning event type */
#define EVT_INFO	4	/**< Debug event type */


/* Avoid compile-time warning because parameter is not used */
#define OPJ_ARG_NOT_USED(x) (void)(x)


#define OPJ_PATH_LEN 4096 /**< Maximum allowed size for filenames */

#define J2K_MAXRLVLS 33					/**< Number of maximum resolution level authorized */
#define J2K_MAXBANDS (3*J2K_MAXRLVLS-2)	/**< Number of maximum sub-band linked to number of resolution level */

/* UniPG>> */
#define JPWL_MAX_NO_TILESPECS	16 /**< Maximum number of tile parts expected by JPWL: increase at your will */
#define JPWL_MAX_NO_PACKSPECS	16 /**< Maximum number of packet parts expected by JPWL: increase at your will */
#define JPWL_MAX_NO_MARKERS	512 /**< Maximum number of JPWL markers: increase at your will */
#define JPWL_PRIVATEINDEX_NAME "jpwl_index_privatefilename" /**< index file name used when JPWL is on */
#define JPWL_EXPECTED_COMPONENTS 3 /**< Expect this number of components, so you'll find better the first EPB */
#define JPWL_MAXIMUM_TILES 8192 /**< Expect this maximum number of tiles, to avoid some crashes */
#define JPWL_MAXIMUM_HAMMING 2 /**< Expect this maximum number of bit errors in marker id's */
#define JPWL_MAXIMUM_EPB_ROOM 65450 /**< Expect this maximum number of bytes for composition of EPBs */

#define J2K_CP_CSTY_PRT 0x01
#define J2K_CP_CSTY_SOP 0x02
#define J2K_CP_CSTY_EPH 0x04
#define J2K_CCP_CSTY_PRT 0x01
#define J2K_CCP_CBLKSTY_LAZY 0x01     /**< Selective arithmetic coding bypass */
#define J2K_CCP_CBLKSTY_RESET 0x02    /**< Reset context probabilities on coding pass boundaries */
#define J2K_CCP_CBLKSTY_TERMALL 0x04  /**< Termination on each coding pass */
#define J2K_CCP_CBLKSTY_VSC 0x08      /**< Vertically stripe causal context */
#define J2K_CCP_CBLKSTY_PTERM 0x10    /**< Predictable termination */
#define J2K_CCP_CBLKSTY_SEGSYM 0x20   /**< Segmentation symbols are used */
#define J2K_CCP_QNTSTY_NOQNT 0
#define J2K_CCP_QNTSTY_SIQNT 1
#define J2K_CCP_QNTSTY_SEQNT 2

/* ----------------------------------------------------------------------- */

#define J2K_MS_SOC 0xff4f	/**< SOC marker value */
#define J2K_MS_SOT 0xff90	/**< SOT marker value */
#define J2K_MS_SOD 0xff93	/**< SOD marker value */
#define J2K_MS_EOC 0xffd9	/**< EOC marker value */
#define J2K_MS_SIZ 0xff51	/**< SIZ marker value */
#define J2K_MS_COD 0xff52	/**< COD marker value */
#define J2K_MS_COC 0xff53	/**< COC marker value */
#define J2K_MS_RGN 0xff5e	/**< RGN marker value */
#define J2K_MS_QCD 0xff5c	/**< QCD marker value */
#define J2K_MS_QCC 0xff5d	/**< QCC marker value */
#define J2K_MS_POC 0xff5f	/**< POC marker value */
#define J2K_MS_TLM 0xff55	/**< TLM marker value */
#define J2K_MS_PLM 0xff57	/**< PLM marker value */
#define J2K_MS_PLT 0xff58	/**< PLT marker value */
#define J2K_MS_PPM 0xff60	/**< PPM marker value */
#define J2K_MS_PPT 0xff61	/**< PPT marker value */
#define J2K_MS_SOP 0xff91	/**< SOP marker value */
#define J2K_MS_EPH 0xff92	/**< EPH marker value */
#define J2K_MS_CRG 0xff63	/**< CRG marker value */
#define J2K_MS_COM 0xff64	/**< COM marker value */

#define T1_NMSEDEC_BITS 7


#define T1_SIG_NE 0x0001	/**< Context orientation : North-East direction */
#define T1_SIG_SE 0x0002	/**< Context orientation : South-East direction */
#define T1_SIG_SW 0x0004	/**< Context orientation : South-West direction */
#define T1_SIG_NW 0x0008	/**< Context orientation : North-West direction */
#define T1_SIG_N 0x0010		/**< Context orientation : North direction */
#define T1_SIG_E 0x0020		/**< Context orientation : East direction */
#define T1_SIG_S 0x0040		/**< Context orientation : South direction */
#define T1_SIG_W 0x0080		/**< Context orientation : West direction */
#define T1_SIG_OTH (T1_SIG_N|T1_SIG_NE|T1_SIG_E|T1_SIG_SE|T1_SIG_S|T1_SIG_SW|T1_SIG_W|T1_SIG_NW)
#define T1_SIG_PRIM (T1_SIG_N|T1_SIG_E|T1_SIG_S|T1_SIG_W)

#define T1_SGN_N 0x0100
#define T1_SGN_E 0x0200
#define T1_SGN_S 0x0400
#define T1_SGN_W 0x0800
#define T1_SGN (T1_SGN_N|T1_SGN_E|T1_SGN_S|T1_SGN_W)

#define T1_SIG 0x1000
#define T1_REFINE 0x2000
#define T1_VISIT 0x4000

#define T1_NUMCTXS_ZC 9
#define T1_NUMCTXS_SC 5
#define T1_NUMCTXS_MAG 3
#define T1_NUMCTXS_AGG 1
#define T1_NUMCTXS_UNI 1

#define T1_CTXNO_ZC 0
#define T1_CTXNO_SC (T1_CTXNO_ZC+T1_NUMCTXS_ZC)
#define T1_CTXNO_MAG (T1_CTXNO_SC+T1_NUMCTXS_SC)
#define T1_CTXNO_AGG (T1_CTXNO_MAG+T1_NUMCTXS_MAG)
#define T1_CTXNO_UNI (T1_CTXNO_AGG+T1_NUMCTXS_AGG)
#define T1_NUMCTXS (T1_CTXNO_UNI+T1_NUMCTXS_UNI)

#define T1_NMSEDEC_FRACBITS (T1_NMSEDEC_BITS-1)

#define T1_TYPE_MQ 0	/**< Normal coding using entropy coder */
#define T1_TYPE_RAW 1	/**< No encoding the information is store under raw format in codestream (mode switch RAW)*/

typedef int opj_bool;
/* 
==========================================================
enum definitions
==========================================================
*/
/** 
Rsiz Capabilities
*/
typedef enum RSIZ_CAPABILITIES {
	STD_RSIZ = 0,		/** Standard JPEG2000 profile*/
	CINEMA2K = 3,		/** Profile name for a 2K image*/
	CINEMA4K = 4		/** Profile name for a 4K image*/
} OPJ_RSIZ_CAPABILITIES;

/** 
Digital cinema operation mode 
*/
typedef enum CINEMA_MODE {
	OFF = 0,					/** Not Digital Cinema*/
	CINEMA2K_24 = 1,	/** 2K Digital Cinema at 24 fps*/
	CINEMA2K_48 = 2,	/** 2K Digital Cinema at 48 fps*/
	CINEMA4K_24 = 3		/** 4K Digital Cinema at 24 fps*/
}OPJ_CINEMA_MODE;
/** 
Progression order 
*/
typedef enum PROG_ORDER {
	PROG_UNKNOWN = -1,	/**< place-holder */
	LRCP = 0,		/**< layer-resolution-component-precinct order */
	RLCP = 1,		/**< resolution-layer-component-precinct order */
	RPCL = 2,		/**< resolution-precinct-component-layer order */
	PCRL = 3,		/**< precinct-component-resolution-layer order */
	CPRL = 4		/**< component-precinct-resolution-layer order */
} OPJ_PROG_ORDER;

/**
Supported image color spaces
*/
typedef enum COLOR_SPACE {
	CLRSPC_UNKNOWN = -1,	/**< not supported by the library */
	CLRSPC_UNSPECIFIED = 0, /**< not specified in the codestream */ 
	CLRSPC_SRGB = 1,		/**< sRGB */
	CLRSPC_GRAY = 2,		/**< grayscale */
	CLRSPC_SYCC = 3			/**< YUV */
} OPJ_COLOR_SPACE;

typedef struct ij2k_prog_order{
	OPJ_PROG_ORDER enum_prog;
	char str_prog[5];
}ij2k_prog_order_t;


/**
Supported codec
*/
typedef enum CODEC_FORMAT {
	CODEC_UNKNOWN = -1,	/**< place-holder */
	CODEC_J2K  = 0,		/**< JPEG-2000 codestream : read/write */
	CODEC_JPT  = 1,		/**< JPT-stream (JPEG 2000, JPIP) : read only */
	CODEC_JP2  = 2 		/**< JPEG-2000 file format : read/write */
} OPJ_CODEC_FORMAT;

/** 
Limit decoding to certain portions of the codestream. 
*/
typedef enum LIMIT_DECODING {
	NO_LIMITATION = 0,				  /**< No limitation for the decoding. The entire codestream will de decoded */
	LIMIT_TO_MAIN_HEADER = 1,		/**< The decoding is limited to the Main Header */
	DECODE_ALL_BUT_PACKETS = 2	/**< Decode everything except the JPEG 2000 packets */
} OPJ_LIMIT_DECODING;

typedef int cpixelType ; 

typedef short flag_t;

/**
Callback function prototype for events
@param msg Event message
@param client_data 
*/
typedef void (*opj_msg_callback) (const char *msg, void *client_data);

/**
Message handler object
used for 
<ul>
<li>Error messages
<li>Warning messages
<li>Debugging messages
</ul>
*/
typedef struct opj_event_mgr 
{
	/** Error message callback if available, NULL otherwise */
	opj_msg_callback error_handler;
	/** Warning message callback if available, NULL otherwise */
	opj_msg_callback warning_handler;
	/** Debug message callback if available, NULL otherwise */
	opj_msg_callback info_handler;
} opj_event_mgr_t;


/* 
==========================================================
image typedef definitions
==========================================================
*/

/**
Defines a single image component
*/
typedef struct opj_image_comp {
	/** XRsiz: horizontal separation of a sample of ith component with respect to the reference grid */
	int dx;
	/** YRsiz: vertical separation of a sample of ith component with respect to the reference grid */
	int dy;
	/** data width */
	int w;
	/** data height */
	int h;
	/** x component offset compared to the whole image */
	int x0;
	/** y component offset compared to the whole image */
	int y0;
	/** precision */
	int prec;
	/** image depth in bits */
	int bpp;
	/** signed (1) / unsigned (0) */
	int sgnd;
	/** number of decoded resolution */
	int resno_decoded;
	/** number of division by 2 of the out image compared to the original size of image */
	int factor;
	/** image component data */

#if defined GPU 
	global int *data;
#else
	int *data;    
#endif 

} opj_image_comp_t;

/** 
Defines image data and characteristics
*/
typedef struct opj_image {
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
#if defined GPU 
	global opj_image_comp_t *comps;
	/** 'restricted' ICC profile */
	global unsigned char *icc_profile_buf;
#else
	opj_image_comp_t *comps;
	/** 'restricted' ICC profile */
	unsigned char *icc_profile_buf;
#endif 


	/** size of ICC profile */
	int icc_profile_len;
} opj_image_t;
/**
Tag node
*/
typedef struct opj_tgt_node 
{
	struct opj_tgt_node *parent;
	int value;
	int low;
	int known;
} opj_tgt_node_t;

/**
Tag tree
*/
typedef struct opj_tgt_tree 
{
	int numleafsh;
	int numleafsv;
	int numnodes;
	opj_tgt_node_t *nodes;
} opj_tgt_tree_t;


typedef unsigned char opj_data;
/** @defgroup TCD TCD - Implementation of a tile coder/decoder */
/*@{*/

/**
FIXME: documentation
*/
typedef struct opj_tcd_seg 
{
	unsigned char** data;
	int dataindex;
	int numpasses;
	int len;
	int maxpasses;
	int numnewpasses;
	int newlen;
} opj_tcd_seg_t;

/**
FIXME: documentation
*/
typedef struct opj_tcd_pass 
{
	int rate;
	double distortiondec;
	int term, len;
} opj_tcd_pass_t;

/**
FIXME: documentation
*/
typedef struct opj_tcd_layer {
	int numpasses;		/* Number of passes in the layer */
	int len;			/* len of information */
	double disto;			/* add for index (Cfr. Marcela) */
	unsigned char *data;		/* data */
} opj_tcd_layer_t;

/**
FIXME: documentation
*/
typedef struct opj_tcd_cblk_enc {
	unsigned char* data;	/* Data */
	opj_tcd_layer_t* layers;	/* layer information */
	opj_tcd_pass_t* passes;	/* information about the passes */
	int x0, y0, x1, y1;		/* dimension of the code-blocks : left upper corner (x0, y0) right low corner (x1,y1) */
	int numbps;
	int numlenbits;
	int numpasses;		/* number of pass already done for the code-blocks */
	int numpassesinlayers;	/* number of passes in the layer */
	int totalpasses;		/* total number of passes */
} opj_tcd_cblk_enc_t;

typedef struct opj_tcd_cblk_dec {
	unsigned char* data;	/* Data */
	opj_tcd_seg_t* segs;		/* segments informations */
	int x0, y0, x1, y1;		/* dimension of the code-blocks : left upper corner (x0, y0) right low corner (x1,y1) */
	int numbps;
	int numlenbits;
	int len;			/* length */
	int numnewpasses;		/* number of pass added to the code-blocks */
	int numsegs;			/* number of segments */
} opj_tcd_cblk_dec_t;

/**
FIXME: documentation
*/
typedef struct opj_tcd_precinct {
	int x0, y0, x1, y1;		/* dimension of the precinct : left upper corner (x0, y0) right low corner (x1,y1) */
	int cw, ch;			/* number of precinct in width and heigth */
	union{		/* code-blocks informations */
		opj_tcd_cblk_enc_t* enc;
		opj_tcd_cblk_dec_t* dec;
	} cblks;
	opj_tgt_tree_t *incltree;		/* inclusion tree */
	opj_tgt_tree_t *imsbtree;		/* IMSB tree */
} opj_tcd_precinct_t;

/**
FIXME: documentation
*/
typedef struct opj_tcd_band {
	int x0, y0, x1, y1;		/* dimension of the subband : left upper corner (x0, y0) right low corner (x1,y1) */
	int bandno;
	opj_tcd_precinct_t *precincts;	/* precinct information */
	int numbps;
	float stepsize;
} opj_tcd_band_t;

/**
FIXME: documentation
*/
typedef struct opj_tcd_resolution {
	int x0, y0, x1, y1;		/* dimension of the resolution level : left upper corner (x0, y0) right low corner (x1,y1) */
	int pw, ph;
	int numbands;			/* number sub-band for the resolution level */
	opj_tcd_band_t bands[3];		/* subband information */
} opj_tcd_resolution_t;


typedef struct opj_tcd_tilecomp 
{
	int x0, y0, x1, y1;		/* dimension of component : left upper corner (x0, y0) right low corner (x1,y1) */
	int numresolutions;		/* number of resolutions level */
	opj_tcd_resolution_t *resolutions;	/* resolutions information */
	cpixelType *data;			/* data of the component */
	int numpix;			/* add fixed_quality */
} opj_tcd_tilecomp_t;

/* Routines that are to be used by both halves of the library are declared
* to receive a pointer to this structure.  There are no actual instances of
* opj_common_struct_t, only of opj_cinfo_t and opj_dinfo_t.
*/
typedef struct opj_common_struct
{
	opj_event_mgr_t *event_mgr;	/**< pointer to the event manager */\
		void * client_data;			/**< Available for use by application */\
		opj_bool is_decompressor;	/**< So common code can tell which is which */\
		OPJ_CODEC_FORMAT codec_format;	/**< selected codec */\
		void *j2k_handle;			/**< pointer to the J2K codec */\
		void *jp2_handle;			/**< pointer to the JP2 codec */\
		void *mj2_handle;		/* Fields common to both master struct types */
	/* Additional fields follow in an actual opj_cinfo_t or
	* opj_dinfo_t.  All three structs must agree on these
	* initial fields!  (This would be a lot cleaner in C++.)
	*/
} opj_common_struct_t;

typedef opj_common_struct_t * opj_common_ptr;


/**
FIXME: documentation
*/
typedef struct opj_tcd_tile 
{
	int x0, y0, x1, y1;		/* dimension of the tile : left upper corner (x0, y0) right low corner (x1,y1) */
	int numcomps;			/* number of components in tile */
	opj_tcd_tilecomp_t *comps;	/* Components information */  
	int numpix;			/* add fixed_quality */
	double distotile;		/* add fixed_quality */
	double distolayer[8];	/* add fixed_quality */
	/** packet number */
	int packno;   
	int ID ;
} opj_tcd_tile_t;

/**
Quantization stepsize
*/
typedef struct opj_stepsize {
	/** exponent */
	int expn;
	/** mantissa */
	int mant;
} opj_stepsize_t;

/**
Tile-component coding parameters
*/
typedef struct opj_tccp {
	/** coding style */
	int csty;
	/** number of resolutions */
	int numresolutions;
	/** code-blocks width */
	int cblkw;
	/** code-blocks height */
	int cblkh;
	/** code-block coding style */
	int cblksty;
	/** discrete wavelet transform identifier */
	int qmfbid;
	/** quantisation style */
	int qntsty;
	/** stepsizes used for quantization */
	opj_stepsize_t stepsizes[J2K_MAXBANDS];
	/** number of guard bits */
	int numgbits;
	/** Region Of Interest shift */
	int roishift;
	/** precinct width */
	int prcw[J2K_MAXRLVLS];
	/** precinct height */
	int prch[J2K_MAXRLVLS];	
} opj_tccp_t;

/**
Progression order changes
*/
typedef struct opj_poc 
{
	/** Resolution num start, Component num start, given by POC */
	int resno0, compno0;
	/** Layer num end,Resolution num end, Component num end, given by POC */
	int layno1, resno1, compno1;
	/** Layer num start,Precinct num start, Precinct num end */
	int layno0, precno0, precno1;
	/** Progression order enum*/
	OPJ_PROG_ORDER prg1,prg;
	/** Progression order string*/
	char progorder[5];
	/** Tile number */
	int tile;
	/** Start and end values for Tile width and height*/
	int tx0,tx1,ty0,ty1;
	/** Start value, initialised in pi_initialise_encode*/
	int layS, resS, compS, prcS;
	/** End value, initialised in pi_initialise_encode */
	int layE, resE, compE, prcE;
	/** Start and end values of Tile width and height, initialised in pi_initialise_encode*/
	int txS,txE,tyS,tyE,dx,dy;
	/** Temporary values for Tile parts, initialised in pi_create_encode */
	int lay_t, res_t, comp_t, prc_t,tx0_t,ty0_t;
} opj_poc_t;

/**
Tile coding parameters : 
this structure is used to store coding/decoding parameters common to all
tiles (information like COD, COC in main header)
*/
typedef struct opj_tcp {
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
	float rates[25];
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
	float distoratio[25];
	/** tile-component coding parameters */
	opj_tccp_t *tccps;
	/* Data was already converted */
	int YUV ;
} opj_tcp_t;


/**
FIXME: documentation
*/
typedef struct opj_tcd_image 
{
	int tw, th;			/* number of tiles in width and heigth */
	opj_tcd_tile_t *tiles;		/* Tiles information */
} opj_tcd_image_t;



typedef struct sft_Sections 
{
	char sectionName[20] ; 
	int startIndex , endIndex;
	int mem_used;
}sft_Sections_t;

typedef struct sft_AllocatedBlock
{
#if defined GPU
	global char* start;
	global char* activePos;	
#else
	char* start;
	char* activePos;
#endif
	int availableMem;
	int usedMem ;
	int origSize;
	int offset;

} ;

typedef struct sft_MemAllocationTable
{
#if defined GPU
	global struct sft_AllocatedBlock* preAllocated ;
	global unsigned int* allocations ;
	global struct sft_Sections_t* sections ;
	global char* globalMem;
	global char* globalVariables[20];
#else
	unsigned int* allocations ;
	sft_Sections_t* sections ;
	sft_AllocatedBlock* preAllocated ;
	char* globalMem;
	char* globalVariables[20];
#endif

	int numBlocks ;
	//int availableMem = 0;
	int allocatedIndex ;
	int sectionIndex ;
	int extraMemAllocation ;
} sft_MemAllocationTable_T;

/**
Coding parameters
*/
typedef struct opj_cp 
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
	// Reference to memAllocator
#if defined GPU
	global struct sft_MemAllocationTable* allocationTable ;
#else
	sft_MemAllocationTable* allocationTable ;
#endif
	/* UniPG>> */
#ifdef USE_JPWL
#endif /* USE_JPWL */
	/* <<UniPG */
} opj_cp_t;


/**
Tile coder/decoder
*/
typedef struct opj_tcd 
{
	/** Position of the tilepart flag in Progression order*/
	int tp_pos;
	/** Tile part number*/
	int tp_num;
	/** Current tile part number*/
	int cur_tp_num;
	/** Total number of tileparts of the current tile*/
	//int cur_totnum_tp;
	/** Current Packet iterator number */
	int cur_pino;
	/** codec context */
	opj_common_ptr cinfo;
	/** info on each image tile */
	opj_tcd_image_t *tcd_image;
	/** image */
	opj_image_t *image;
	/** coding parameters */
	opj_cp_t *cp;
	/** pointer to the current encoded/decoded tile */
	opj_tcd_tile_t *tcd_tile;
	/** coding/decoding parameters common to all tiles */
	opj_tcp_t *tcp;
	/** current encoded/decoded tile */
	int tcd_tileno;
	/** Time taken to encode a tile*/
	double encoding_time;
	opj_data* packetData;
	int dataLength ;
} opj_tcd_t;

/* 
==========================================================
Information on the JPEG 2000 codestream
==========================================================
*/

/**
Index structure : Information concerning a packet inside tile
*/
typedef struct opj_packet_info {
	/** packet start position (including SOP marker if it exists) */
	int start_pos;
	/** end of packet header position (including EPH marker if it exists)*/
	int end_ph_pos;
	/** packet end position */
	int end_pos;
	/** packet distorsion */
	double disto;
} opj_packet_info_t;


/* UniPG>> */
/**
Marker structure
*/
typedef struct opj_marker_info_t {
	/** marker type */
	unsigned short int type;
	/** position in codestream */
	int pos;
	/** length, marker val included */
	int len;
} opj_marker_info_t;
/* <<UniPG */

/**
Index structure : Information concerning tile-parts
*/
typedef struct opj_tp_info {
	/** start position of tile part */
	int tp_start_pos;
	/** end position of tile part header */
	int tp_end_header;
	/** end position of tile part */
	int tp_end_pos;
	/** start packet of tile part */
	int tp_start_pack;
	/** number of packets of tile part */
	int tp_numpacks;
} opj_tp_info_t;

/**
Index structure : information regarding tiles 
*/
typedef struct opj_tile_info {
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
	//int pw[33];
	/** precinct number for each resolution level (height) */
	//int ph[33];
	/** precinct size (in power of 2), in X for each resolution level */
	//int pdx[33];
	/** precinct size (in power of 2), in Y for each resolution level */
	//int pdy[33];
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
} opj_tile_info_t;

/**
Index structure of the codestream
*/
typedef struct opj_codestream_info 
{
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
	opj_tile_info_t *tile;
} opj_codestream_info_t;


/*
* Stream open flags.
*/
/** The stream was opened for reading. */
#define OPJ_STREAM_READ	0x0001
/** The stream was opened for writing. */
#define OPJ_STREAM_WRITE 0x0002


/**
Byte input-output stream (CIO)
*/
typedef struct opj_cio 
{
	/** codec context */
	opj_common_ptr cinfo;

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
} opj_cio_t;

/**
JPEG-2000 codestream reader/writer
*/
typedef struct opj_j2k 
{
	/** codec context */
	opj_common_ptr cinfo;
	/** locate in which part of the codestream the decoder is (main header, tile header, end) */
	int state;
	/** number of the tile curently concern by coding/decoding */
	//int curtileno;
	/** Tile part number*/
	//int tp_num;
	/** Tilepart number currently coding*/
	//int cur_tp_num;
	/** Total number of tileparts of the current tile*/
	int *cur_totnum_tp;
	/**
	locate the start position of the TLM marker  
	after encoding the tilepart, a jump (in j2k_write_sod) is done to the TLM marker to store the value of its length. 
	*/
	int tlm_start;
	/** Total num of tile parts in whole image = num tiles* num tileparts in each tile*/
	/** used in TLMmarker*/
	int totnum_tp;	
	/** 
	locate the position of the end of the tile in the codestream, 
	used to detect a truncated codestream (in j2k_read_sod)
	*/
	unsigned char *eot;
	/**
	locate the start position of the SOT marker of the current coded tile:  
	after encoding the tile, a jump (in j2k_write_sod) is done to the SOT marker to store the value of its length. 
	*/
	int sot_start;
	int sod_start;
	/**
	as the J2K-file is written in several parts during encoding, 
	it enables to make the right correction in position return by cio_tell
	*/
	int pos_correction;
	/** array used to store the data of each tile */
	unsigned char **tile_data;
	/** array used to store the length of each tile */
	int *tile_len;
	/** 
	decompression only : 
	store decoding parameters common to all tiles (information like COD, COC in main header)
	*/
	opj_tcp_t *default_tcp;
	/** pointer to the encoded / decoded image */
	opj_image_t *image;
	/** pointer to the coding parameters */
	opj_cp_t *cp;
	/** helper used to write the index file */
	opj_codestream_info_t *cstr_info;
	/** pointer to the byte i/o stream */
	opj_cio_t *cio;
} opj_j2k_t;

/**
This struct defines the state of a context.
*/
typedef struct opj_mqc_state {
	/** the probability of the Least Probable Symbol (0.75->0x8000, 1.5->0xffff) */
	unsigned int qeval;
	/** the Most Probable Symbol (0 or 1) */
	int mps;
	/** next state if the next encoded symbol is the MPS */
	struct opj_mqc_state *nmps;
	/** next state if the next encoded symbol is the LPS */
	struct opj_mqc_state *nlps;
} opj_mqc_state_t;

#define MQC_NUMCTXS 19

/**
MQ coder
*/
typedef struct opj_mqc_stateI {
	/** the probability of the Least Probable Symbol (0.75->0x8000, 1.5->0xffff) */
	unsigned int qeval;
	/** the Most Probable Symbol (0 or 1) */
	int mps;
	/** next state if the next encoded symbol is the MPS */
	int  nmps;
	/** next state if the next encoded symbol is the LPS */
	int nlps;
} opj_mqc_stateI_t;

typedef struct opj_mqc {
	unsigned int c;
	unsigned int a;
	unsigned int ct;
	unsigned char *bp;
	unsigned char *start;
	unsigned char *end;
	opj_mqc_stateI_t *ctxs[MQC_NUMCTXS];
	opj_mqc_stateI_t **curctx;
#ifdef MQC_PERF_OPT
	unsigned char *buffer;
#endif
} opj_mqc_t;

/**
RAW encoding operations
*/
typedef struct opj_raw {
	/** temporary buffer where bits are coded or decoded */
	unsigned char c;
	/** number of bits already read or free to write */
	unsigned int ct;
	/** maximum length to decode */
	unsigned int lenmax;
	/** length decoded */
	unsigned int len;
	/** pointer to the current position in the buffer */
	unsigned char *bp;
	/** pointer to the start of the buffer */
	unsigned char *start;
	/** pointer to the end of the buffer */
	unsigned char *end;
} opj_raw_t;

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


/**
FIXME: documentation
*/
/** 
T2 encoding mode 
*/
typedef enum T2_MODE {
	THRESH_CALC = 0,	/** Function called in Rate allocation process*/
	FINAL_PASS = 1		/** Function called in Tier 2 process*/
}J2K_T2_MODE;

typedef struct iopj_pi_resolution {
	int pdx, pdy;
	int pw, ph;
} iopj_pi_resolution_t;

typedef struct iopj_pi_comp {
	int dx, dy;
	/** number of resolution levels */
	int numresolutions;
	iopj_pi_resolution_t *resolutions;
} iopj_pi_comp_t;

/**
Progression order changes
*/
typedef struct iopj_poc {
	/** Resolution num start, Component num start, given by POC */
	int resno0, compno0;
	/** Layer num end,Resolution num end, Component num end, given by POC */
	int layno1, resno1, compno1;
	/** Layer num start,Precinct num start, Precinct num end */
	int layno0, precno0, precno1;
	/** Progression order enum*/
	OPJ_PROG_ORDER prg1,prg;
	/** Progression order string*/
	char progorder[5];
	/** Tile number */
	int tile;
	/** Start and end values for Tile width and height*/
	int tx0,tx1,ty0,ty1;
	/** Start value, initialised in pi_initialise_encode*/
	int layS, resS, compS, prcS;
	/** End value, initialised in pi_initialise_encode */
	int layE, resE, compE, prcE;
	/** Start and end values of Tile width and height, initialised in pi_initialise_encode*/
	int txS,txE,tyS,tyE,dx,dy;
	/** Temporary values for Tile parts, initialised in pi_create_encode */
	int lay_t, res_t, comp_t, prc_t,tx0_t,ty0_t;
} iopj_poc_t;

/** 
Packet iterator 
*/
typedef struct iopj_pi_iterator 
{
	/** Enabling Tile part generation*/
	char tp_on;
	/** precise if the packet has been already used (usefull for progression order change) */
	short int *include;
	/** layer step used to localize the packet in the include vector */
	int step_l;
	/** resolution step used to localize the packet in the include vector */
	int step_r;	
	/** component step used to localize the packet in the include vector */
	int step_c;	
	/** precinct step used to localize the packet in the include vector */
	int step_p;	
	/** component that identify the packet */
	int compno;
	/** resolution that identify the packet */
	int resno;
	/** precinct that identify the packet */
	int precno;
	/** layer that identify the packet */
	int layno;   
	/** 0 if the first packet */
	int first;
	/** progression order change information */
	iopj_poc_t poc;
	/** number of components in the image */
	int numcomps;
	/** Components*/
	iopj_pi_comp_t *comps;
	int tx0, ty0, tx1, ty1;
	int x, y, dx, dy;
} iopj_pi_iterator_t;




/**
Individual bit input-output stream (BIO)
*/
typedef struct opj_bio {
	/** pointer to the start of the buffer */
	unsigned char *start;
	/** pointer to the end of the buffer */
	unsigned char *end;
	/** pointer to the present position in the buffer */
	unsigned char *bp;
	/** temporary place where each byte is read or written */
	unsigned int buf;
	/** coder : number of bits free to write. decoder : number of bits read */
	int ct;
} opj_bio_t;

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

/**
Compression parameters
*/
typedef struct opj_cparameters 
{
	int Width ; 
	int Height;
	/** size of tile: tile_size_on = false (not in argument) or = true (in argument) */
	opj_bool tile_size_on;
	/** XTOsiz */
	int cp_tx0;
	/** YTOsiz */
	int cp_ty0;
	/** XTsiz */
	int cp_tdx;
	/** YTsiz */
	int cp_tdy;
	/** allocation by rate/distortion */
	int cp_disto_alloc;
	/** allocation by fixed layer */
	int cp_fixed_alloc;
	/** add fixed_quality */
	int cp_fixed_quality;
	/** fixed layer */
	int *cp_matrice;
	/** comment for coding */
	char *cp_comment;
	/** csty : coding style */
	int csty;
	/** progression order (default LRCP) */
	OPJ_PROG_ORDER prog_order;
	/** progression order changes */
	opj_poc_t POC[32];
	/** number of progression order changes (POC), default to 0 */
	int numpocs;
	/** number of layers */
	int tcp_numlayers;
	/** rates of layers */
	float tcp_rates[100];
	/** different psnr for successive layers */
	float tcp_distoratio[100];
	/** number of resolutions */
	int numresolution;
	/** initial code block width, default to 64 */
	int cblockw_init;
	/** initial code block height, default to 64 */
	int cblockh_init;
	/** mode switch (cblk_style) */
	int mode;
	/** 1 : use the irreversible DWT 9-7, 0 : use lossless compression (default) */
	int irreversible;
	/** region of interest: affected component in [0..3], -1 means no ROI */
	int roi_compno;
	/** region of interest: upshift value */
	int roi_shift;
	/* number of precinct size specifications */
	int res_spec;
	/** initial precinct width */
	int prcw_init[J2K_MAXRLVLS];
	/** initial precinct height */
	int prch_init[J2K_MAXRLVLS];

	/**@name command line encoder parameters (not used inside the library) */
	/*@{*/

	/** DEPRECATED. Index generation is now handeld with the opj_encode_with_info() function. Set to NULL */
	int index_on;

	/** subimage encoding: origin image offset in x direction */
	int image_offset_x0;
	/** subimage encoding: origin image offset in y direction */
	int image_offset_y0;
	/** subsampling value for dx */
	int subsampling_dx;
	/** subsampling value for dy */
	int subsampling_dy;
	/** input file format 0: PGX, 1: PxM, 2: BMP 3:TIF*/
	int decod_format;
	/** output file format 0: J2K, 1: JP2, 2: JPT */
	int cod_format;
	/*@}*/

	/* UniPG>> */
	/**@name JPWL encoding parameters */
	/*@{*/
	/** enables writing of EPC in MH, thus activating JPWL */
	opj_bool jpwl_epc_on;
	/** error protection method for MH (0,1,16,32,37-128) */
	int jpwl_hprot_MH;
	/** tile number of header protection specification (>=0) */
	int jpwl_hprot_TPH_tileno[JPWL_MAX_NO_TILESPECS];
	/** error protection methods for TPHs (0,1,16,32,37-128) */
	int jpwl_hprot_TPH[JPWL_MAX_NO_TILESPECS];
	/** tile number of packet protection specification (>=0) */
	int jpwl_pprot_tileno[JPWL_MAX_NO_PACKSPECS];
	/** packet number of packet protection specification (>=0) */
	int jpwl_pprot_packno[JPWL_MAX_NO_PACKSPECS];
	/** error protection methods for packets (0,1,16,32,37-128) */
	int jpwl_pprot[JPWL_MAX_NO_PACKSPECS];
	/** enables writing of ESD, (0=no/1/2 bytes) */
	int jpwl_sens_size;
	/** sensitivity addressing size (0=auto/2/4 bytes) */
	int jpwl_sens_addr;
	/** sensitivity range (0-3) */
	int jpwl_sens_range;
	/** sensitivity method for MH (-1=no,0-7) */
	int jpwl_sens_MH;
	/** tile number of sensitivity specification (>=0) */
	int jpwl_sens_TPH_tileno[JPWL_MAX_NO_TILESPECS];
	/** sensitivity methods for TPHs (-1=no,0-7) */
	int jpwl_sens_TPH[JPWL_MAX_NO_TILESPECS];
	/*@}*/
	/* <<UniPG */

	/** Digital Cinema compliance 0-not compliant, 1-compliant*/
	OPJ_CINEMA_MODE cp_cinema;
	/** Maximum rate for each component. If == 0, component size limitation is not considered */
	int max_comp_size;
	/** Profile name*/
	OPJ_RSIZ_CAPABILITIES cp_rsiz;
	/** Tile part generation*/
	char tp_on;
	/** Flag for Tile part generation*/
	char tp_flag;
	/** MCT (multiple component transform) */
	char tcp_mct;
	/** Enable JPIP indexing*/
	opj_bool jpip_on;
} opj_cparameters_t;
/**
Component parameters structure used by the opj_image_create function
*/
typedef struct opj_image_comptparm {
	/** XRsiz: horizontal separation of a sample of ith component with respect to the reference grid */
	int dx;
	/** YRsiz: vertical separation of a sample of ith component with respect to the reference grid */
	int dy;
	/** data width */
	int w;
	/** data height */
	int h;
	/** x component offset compared to the whole image */
	int x0;
	/** y component offset compared to the whole image */
	int y0;
	/** precision */
	int prec;
	/** image depth in bits */
	int bpp;
	/** signed (1) / unsigned (0) */
	int sgnd;
} opj_image_cmptparm_t;

/*

HEADERS

*/
#if !defined GPU
void* sft_calloc(unsigned int num , unsigned int size, sft_MemAllocationTable* allocationTable);
void* sft_malloc(unsigned int size, sft_MemAllocationTable* allocationTable);
void* sft_aligned_malloc(unsigned int size, sft_MemAllocationTable* allocationTable);
void sft_memSet( void* memmory, char value,int  dataSize);
void sft_memCopy(void* dst , void* src, int len);
void sft_freepreAlloc(sft_MemAllocationTable* allocationTable);
void sft_free(void* m,sft_MemAllocationTable* allocationTable);
void sft_resetAllocated(sft_MemAllocationTable* allocationTable);
void* sft_base(sft_MemAllocationTable* allocationTable);
void sft_preAlloc(unsigned int size, unsigned int numSections, unsigned int extraSlot0Mem,sft_MemAllocationTable* allocationTable);
sft_MemAllocationTable* sft_initAllocTable();


opj_event_mgr_t* opj_set_event_mgr(opj_common_ptr cinfo, opj_event_mgr_t *event_mgr, void *context) ;
opj_bool opj_event_msg(opj_common_ptr cinfo, int event_type, const char *fmt, ...)  ;

int int_max(int a, int b) ;
int int_min(int a, int b);
int int_ceildiv(int a, int b) ;
int int_ceildivpow2(int a, int b);
int int_floordivpow2(int a, int b) ;
int int_floorlog2(int a) ;

void pi_destroy(iopj_pi_iterator_t *pi, opj_cp_t *cp, int tileno);
int ij2k_get_num_tp(opj_cp_t *cp,int pino,int tileno);
void ij2k_encodeTile(opj_cp_t *cp  ,int tileno , opj_j2k_t *j2k , opj_tcd_t *gtcd, sft_MemAllocationTable* mt) ;
void itgt_reset(opj_tgt_tree_t *tree)  ;
int idwt_getgain(int orient) ;
opj_tgt_tree_t *itgt_create(int numleafsh, int numleafsv, sft_MemAllocationTable* mt);
void itgt_destroy(opj_tgt_tree_t *tree, sft_MemAllocationTable* mt)  ;
void idwt_calc_explicit_stepsizes(opj_tccp_t * tccp, int prec)  ;
void iopj_image_destroy(sft_MemAllocationTable* mt);
opj_image_t* iopj_image_create(int numcmpts, opj_image_cmptparm_t *cmptparms, OPJ_COLOR_SPACE clrspc,sft_MemAllocationTable* mt);
opj_image_t * innerConvertion(unsigned char* RGB,int W, int H, opj_cparameters_t *parameters,sft_MemAllocationTable* mt);
opj_image_t* ibmptoimage(unsigned char *RGB, opj_cparameters_t *parameters,sft_MemAllocationTable* mt);

#else

global void* sft_malloc(unsigned int size, global struct sft_MemAllocationTable* allocationTable)
{
	global void *res = NULL;

	int threadID = 0;


	if ((allocationTable->preAllocated) && (int)(allocationTable->preAllocated[threadID].availableMem)>(int)(size))
	{		
		allocationTable->preAllocated[threadID].availableMem -= size;
		allocationTable->preAllocated[threadID].usedMem +=size;
#if defined MEM_LOGGING
		allocations[allocatedIndex] = size+2;	
		allocatedIndex ++;
#endif
		res = allocationTable->preAllocated[threadID].activePos;
		allocationTable->preAllocated[threadID].activePos = (global char*)(allocationTable->preAllocated[threadID].activePos) + size;

		return res ;
	}
#if defined MEM_LOGGING
	allocations[allocatedIndex] = size;	
	allocatedIndex ++;
#endif

}

void sft_memSet( global void* memmory, char value,int  dataSize)
{
	int i = 0;
	global char* localM = (global char*)(memmory);
	for ( i =0 ;i<dataSize ; i++)
	{
		*localM = value;
		localM++;
	}
}
#endif

/*
PI
*/

#if defined GPU
kernel void sft_preAlloc(int size, int numSections, int extraSlot0Mem,
	//-- These variables are initialized from outside
	global struct sft_MemAllocationTable* allocationTable,
	global struct sft_AllocatedBlock*	preAllocated,
	global char* globalMem)
{
	
	int i = 0;
	int j;
	int extraMem = 0;
	int totalMem = (numSections *size +  extraSlot0Mem) ;
	
	uint thID = get_global_id(0);    
	if (thID > 0) return;

	allocationTable->preAllocated = preAllocated; //(sft_AllocatedBlock*)malloc(sizeof(sft_AllocatedBlock) * numSections);
	allocationTable->numBlocks = numSections;

	allocationTable->globalMem = globalMem ; // (char*)malloc(totalMem * sizeof(char));

	for (i=0; i<numSections ; i++)
	{
		// Alojo el doble para el primer Bloque
		if (i == 0)
		{
			allocationTable->preAllocated[i].activePos = allocationTable->globalMem ; 
			allocationTable->preAllocated[i].origSize = size+ extraSlot0Mem;		
			allocationTable->preAllocated[i].availableMem = size+ extraSlot0Mem;		
		}
		else
		{
			allocationTable->preAllocated[i].activePos = allocationTable->globalMem ; 
			allocationTable->preAllocated[i].activePos += i*size + extraSlot0Mem;
			allocationTable->preAllocated[i].origSize = size;		
		}

		allocationTable->preAllocated[i].availableMem = allocationTable->preAllocated[i].origSize;
		allocationTable->preAllocated[i].usedMem = 0;
		allocationTable->preAllocated[i].start  = allocationTable->preAllocated[i].activePos;	

		//		memset(allocationTable->preAllocated[i].start , 0 ,allocationTable->preAllocated[i].availableMem );

		for (j = 0 ; j<allocationTable->preAllocated[i].availableMem  ; j++)
			if (allocationTable->globalMem[j+ allocationTable->preAllocated[i].activePos - allocationTable->globalMem ] != 0)
			{
				//		 fprintf(stderr, "Error");
			}
	}	

}

global void* sft_calloc(unsigned int num , unsigned int size, global struct sft_MemAllocationTable* allocationTable)
{
	int i = 0;
	global char* res ;
	uint threadID = get_global_id(0); 

	if ((allocationTable->preAllocated) && (int)(allocationTable->preAllocated[threadID].availableMem)>(int)(num*size))
	{
		allocationTable->preAllocated[threadID].availableMem -= num*size;
		allocationTable->preAllocated[threadID].usedMem +=num*size;

		res = allocationTable->preAllocated[threadID].activePos;		
		//memset(res,0,num*size * sizeof(char));		
		allocationTable->preAllocated[threadID].activePos = (global char*)(allocationTable->preAllocated[threadID].activePos) + num*size;
		return res;

	}
}

void  sft_memCopy(global unsigned char *dst, global unsigned char* src,int offset , int count )
{    
	int i = 0;
	src+= offset;
	for (i = 0 ;i<count ; i++)
	{
	  *dst = *src;
	  src++;
	}    

}


kernel void sft_resetAllocated(global struct sft_MemAllocationTable* allocationTable)
{
	int i = 0;
	int extraMem = 0;

	for (i=0; i<allocationTable->numBlocks ; i++)
	{
		// Alojo el doble para el primer Bloque
		allocationTable->preAllocated[i].activePos = allocationTable->preAllocated[i].start ;
		allocationTable->preAllocated[i].availableMem = allocationTable->preAllocated[i].origSize;		
		allocationTable->preAllocated[i].usedMem = 0;				
		//	memset(allocationTable->preAllocated[i].start , 0 ,(allocationTable->preAllocated[i].origSize)*sizeof(char));
	}

}


kernel void iopj_set_default_encoder_parameters(global struct opj_cparameters *parameters) 
{
	uint thID = get_global_id(0); 

	if(thID == 0) 
	{
		sft_memSet(parameters, 0, sizeof(opj_cparameters_t));
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
		//---- Parameters JUAN
		parameters->decod_format = BMP_DFMT;
		parameters->cod_format = J2K_CFMT;
		parameters->cblockw_init = 64;
		parameters->cblockh_init = 64;
		parameters->tile_size_on = OPJ_TRUE;
		parameters->tcp_rates[0] = 0;	// MOD antonin : losslessbug 
		parameters->tcp_numlayers++;
		parameters->cp_disto_alloc = 1;
		parameters->cp_tdx = 64	;
		parameters->cp_tdy = 64 ;


	}
}

void iopj_image_destroy(global struct sft_MemAllocationTable* mt) 
{

}

global opj_image_t* iopj_image_create(int numcmpts,global opj_image_cmptparm_t *cmptparms, OPJ_COLOR_SPACE clrspc,global struct sft_MemAllocationTable* mt) 
{
    int compno;
	global opj_image_t* image = NULL;
    image = (global opj_image_t*) sft_calloc(1, sizeof( opj_image_t),mt);
    if(image) 
	{
		image->numcomps = 1;
        image->color_space = clrspc;                                
        image->numcomps = numcmpts;
		
		// allocate memory for the per-component information 
		
        image->comps = (global opj_image_comp_t*)sft_malloc(image->numcomps * sizeof(opj_image_comp_t),mt);
		if(!image->comps) 
		{
			iopj_image_destroy(mt);
			return NULL;
		}
		// create the individual image components 

		for(compno = 0; compno < numcmpts; compno++) 
		{
			global opj_image_comp_t *comp = &image->comps[compno];
			comp->dx = cmptparms[compno].dx;
			comp->dy = cmptparms[compno].dy;
			comp->w = cmptparms[compno].w;
			comp->h = cmptparms[compno].h;
			comp->x0 = cmptparms[compno].x0;
			comp->y0 = cmptparms[compno].y0;
			comp->prec = cmptparms[compno].prec;
			comp->bpp = cmptparms[compno].bpp;
			comp->sgnd = cmptparms[compno].sgnd;
			comp->data = (global int*) sft_calloc(comp->w * comp->h, sizeof(int),mt);
			if(!comp->data) 
			{
				//fprintf(stderr,"Unable to allocate memory for image.\n");
				iopj_image_destroy(mt);
				return NULL;
			}
		}
	}

	return image;
}

global opj_image_t * innerConvertion(global unsigned char* RGB,int W, int H, global opj_cparameters_t *parameters, global struct sft_MemAllocationTable* mt)
{
	int subsampling_dx = parameters->subsampling_dx;
	int subsampling_dy = parameters->subsampling_dy;

	int i, numcomps,PAD, index,x,y;
	OPJ_COLOR_SPACE color_space;
	global opj_image_cmptparm_t* cmptparm;	// maximum of 3 components 
	global opj_image_t * image = NULL;

	numcomps = 3;
	color_space = CLRSPC_SRGB;
	// initialize image components 
	cmptparm = (global opj_image_cmptparm_t*) sft_malloc( 3* sizeof(opj_image_cmptparm_t),mt);
	sft_memSet( (global void*)(cmptparm) , 0, 3 * sizeof(opj_image_cmptparm_t) );
	for(i = 0; i < numcomps; i++) 
	{
			cmptparm[i].prec = 8;
			cmptparm[i].bpp = 8;
			cmptparm[i].sgnd = 0;
			cmptparm[i].dx = subsampling_dx;
			cmptparm[i].dy = subsampling_dy;
			cmptparm[i].w = W;
			cmptparm[i].h = H;
	}
		// create the image 
		image = iopj_image_create(numcomps, (global void*)(cmptparm), color_space,mt);
		if(!image) 
		{			
			return NULL;
		}

		// set image offset and reference grid 
		image->x0 = parameters->image_offset_x0;
		image->y0 = parameters->image_offset_y0;
		image->x1 =	!image->x0 ? (W - 1) * subsampling_dx + 1 : image->x0 + (W - 1) * subsampling_dx + 1;
		image->y1 =	!image->y0 ? (H - 1) * subsampling_dy + 1 : image->y0 + (H - 1) * subsampling_dy + 1;

		// set image data 
		// PAD = 4 - (3 * W) % 4; 
		// PAD = (PAD == 4) ? 0 : PAD; 
		PAD = (3 * W) % 4 ? 4 - (3 * W) % 4 : 0;

		index = 0;

		for(y = 0; y < (int)H; y++) 
		{
			global unsigned char *scanline = RGB + (3 * W + PAD) * (H - 1 - y);
			for(x = 0; x < (int)W; x++) 
			{
				global unsigned char *pixel = &scanline[3 * x];
				image->comps[0].data[index] = pixel[2];	// R 
				image->comps[1].data[index] = pixel[1];	// G 
				image->comps[2].data[index] = pixel[0];	// B 
				index++;
			}
		}

	return image;	
}

kernel void  ibmptoimage(global unsigned char *RGB, global opj_cparameters_t *parameters,global struct sft_MemAllocationTable* mt)
{
    uint thID = get_global_id(0);    
	if (thID > 0) return;
    mt->globalVariables[SFT_IMAGE] = (global char*)( innerConvertion(RGB,parameters->Width,parameters->Height,parameters,mt));
/* 
 global opj_image_t* image = NULL;
    image = (global opj_image_t*) sft_calloc(1, sizeof( opj_image_t),mt);
    if(image) 
	{
		image->x0 = 1001;
		image->y0 = 1001;
        image->numcomps = 3;		
		// allocate memory for the per-component information 		
        image->comps = (global opj_image_comp_t*)sft_malloc(image->numcomps * sizeof(opj_image_comp_t),mt);
		
		mt->globalVariables[SFT_IMAGE] = (global char*)(image);
	}
	*/
}

kernel void  readimage(global unsigned char *RGB, global opj_cparameters_t *parameters,global struct sft_MemAllocationTable* mt,global opj_image_t * image2)
{
    uint thID = get_global_id(0);    
	if (thID > 0) return;
	sft_memSet( (global opj_image_t*)(image2) , 0, sizeof(opj_image_t));
	
	global opj_image_t * image = (global opj_image_t *) mt->globalVariables[SFT_IMAGE];
	image2->x0 = image->x0;
	image2->y0 = image->y0 ;
	image2->x1 = image->x1;
	image2->y1 = image->y1 ;
	image2->numcomps = 3; 
	image2->comps = image->comps; 
	int W = parameters->Width;
	int H = parameters->Height;
	
	int PAD = (3 * W) % 4 ? 4 - (3 * W) % 4 : 0;

		for(y = 0; y < (int)H; y++) 
		{
			global unsigned char *scanline = RGB + (3 * W + PAD) * (H - 1 - y);
			for(x = 0; x < (int)W; x++) 
			{
			   global unsigned char *pixel = &scanline[3 * x];
			   RGB[0] = 1;
			   RGB[1] = 1;
			   
			}
			/*global unsigned char *scanline = RGB + (3 * W + PAD) * (H - 1 - y);
			for(x = 0; x < (int)W; x++) 
			{
				global unsigned char *pixel = &scanline[3 * x];
				image->comps[0].data[index] = pixel[2];	// R 
				image->comps[1].data[index] = pixel[1];	// G 
				image->comps[2].data[index] = pixel[0];	// B 
				index++;
			}
			*/
		}
	
	return ;
	
	
	
	image2->x0 = image->x0;
	image2->y0 = image->y0;
	image2->numcomps = 3; 
	
}


kernel void  memCopy(global unsigned char *dst, global unsigned char* src,int offset , int count )
{
    uint thID = get_global_id(0);    
	if (thID > 0) return;
	sft_memCopy(dst, src,offset,count);	

}



__kernel void Sample3( int offset)
{
// get index into global data array
    uint thID = get_global_id(0);    

//  ij2k_encode_Tile(NULL , NULL, NULL , 0,0,0);
}


// *****************************************************************************
//*****************************************************************************
__kernel void Sample5(global struct sft_AllocatedBlock* preAllocated , global char* memmory, int memBlockSize, int offset,global struct sft_MemAllocationTable* mt)
{
	// get index into global data array
	uint thID = get_global_id(0);    

	global char *image2 = NULL;
	global opj_image_t* img ;
	global opj_image_comp_t* comps ;
	int wasAssigned =0 ;
	//image2 = (global char* )(  sft_base(memmory)  );
	// Salto el espacio de la imagen
	image2 = sft_malloc(  sizeof(opj_image_t),mt) ;

	img = (global opj_image_t*)(image2);

	img->y0= 1;
	img->x1= 1;
	img->y1= 1;


}

#endif
