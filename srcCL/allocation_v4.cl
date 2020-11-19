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
#define SFT_CODEC 1
#define SFT_INFO 2
#define SFT_PARAMETERS 3
#define SFT_CIO 4
#define SFT_CODING_PARAMS 5
#define SFT_TILE_CODER_DECODER 6


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

//

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


typedef struct opj_cinfo {
	/** Fields shared with opj_dinfo_t */
	opj_event_mgr_t *event_mgr;	/**< pointer to the event manager */
	void * client_data;			/**< Available for use by application */
	opj_bool is_decompressor;	/**< So common code can tell which is which */
	OPJ_CODEC_FORMAT codec_format;	/**< selected codec */\
	void *j2k_handle;			/**< pointer to the J2K codec */
	void *jp2_handle;			/**< pointer to the JP2 codec */
	void *mj2_handle ;			/**< pointer to the MJ2 codec */
	/* other specific fields go here */
} opj_cinfo_t;

#if defined GPU
typedef global opj_common_struct_t * opj_common_ptr;
#else
typedef opj_common_struct_t * opj_common_ptr;
#endif


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
	#if defined GPU
	global opj_tccp_t *tccps;
	#else
	opj_tccp_t *tccps;
	#endif
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

} sft_AllocatedBlock_t;

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
    int cio_index ; 
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
	/** fixed layer */
	
	// Reference to memAllocator
#if defined GPU
	global struct sft_MemAllocationTable* allocationTable ;
                global int *matrice;
	global opj_tcp_t *tcps;
#else
	sft_MemAllocationTable* allocationTable ;
                int *matrice;
	/** tile coding parameters */
	opj_tcp_t *tcps;

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
	/** buffer size in bytes */
	int length;	
	int cio_index; 

  #if defined GPU
	/** pointer to the start of the buffer */
	global unsigned char *buffer;

	/** pointer to the start of the stream */
	global unsigned char *start;
	/** pointer to the end of the stream */
	global  unsigned char *end;
	/** pointer to the current position */
	global unsigned char *bp;
#else
	/** pointer to the start of the buffer */
	unsigned char *buffer;

	/** pointer to the start of the stream */
	unsigned char *start;
	/** pointer to the end of the stream */
	unsigned char *end;
	/** pointer to the current position */
	unsigned char *bp;

#endif


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
	
	
	/** pointer to the coding parameters */
    #if defined GPU
	global opj_cp_t *cp;
    global opj_cio_t *cio;
	global opj_image_t *image;
	global int *cur_totnum_tp; //  Total number of tileparts of the current tile
	#else
	opj_cp_t *cp;
    opj_cio_t *cio;
	opj_image_t *image; // pointer to the encoded / decoded image 	
	int *cur_totnum_tp;//  Total number of tileparts of the current tile
	#endif
	/** helper used to write the index file */
	opj_codestream_info_t *cstr_info;
	/** pointer to the byte i/o stream */
	
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
	#if defined GPU
	global iopj_pi_resolution_t *resolutions;
	#else	
	iopj_pi_resolution_t *resolutions;
	#endif
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
	
	#if defined GPU
	global iopj_pi_comp_t *comps;
	global short int *include;
	#else
	iopj_pi_comp_t *comps;// Components	
	short int *include;// precise if the packet has been already used (usefull for progression order change) 
	#endif
	
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

#if defined GPU
constant double dwt_norms[4][10] = {
	{1.000, 1.500, 2.750, 5.375, 10.68, 21.34, 42.67, 85.33, 170.7, 341.3},
	{1.038, 1.592, 2.919, 5.703, 11.33, 22.64, 45.25, 90.48, 180.9},
	{1.038, 1.592, 2.919, 5.703, 11.33, 22.64, 45.25, 90.48, 180.9},
	{.7186, .9218, 1.586, 3.043, 6.019, 12.01, 24.00, 47.97, 95.93}
};
#endif


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
/*
 MATH FUNCTIONS
*/

int int_min(int a, int b) {
	return a < b ? a : b;
}
/**
Get the maximum of two integers
@return Returns a if a > b else b
*/
int int_max(int a, int b) {
	return (a > b) ? a : b;
}
/**
Clamp an integer inside an interval
@return
<ul>
<li>Returns a if (min < a < max)
<li>Returns max if (a > max)
<li>Returns min if (a < min) 
</ul>
*/
int int_clamp(int a, int min, int max) {
	if (a < min)
		return min;
	if (a > max)
		return max;
	return a;
}
/**
@return Get absolute value of integer
*/
int int_abs(int a) {
	return a < 0 ? -a : a;
}
/**
Divide an integer and round upwards
@return Returns a divided by b
*/
int int_ceildiv(int a, int b) {
	return (a + b - 1) / b;
}
/**
Divide an integer by a power of 2 and round upwards
@return Returns a divided by 2^b
*/
int int_ceildivpow2(int a, int b)
{
	return (a + (1 << b) - 1) >> b;
}
/**
Divide an integer by a power of 2 and round downwards
@return Returns a divided by 2^b
*/
int int_floordivpow2(int a, int b) {
	return a >> b;
}
/**
Get logarithm of an integer and round downwards
@return Returns log2(a)
*/
int int_floorlog2(int a) 
{
	int l;
	for (l = 0; a > 1; l++) {
		a >>= 1;
	}
	return l;
}

void idwt_encode_stepsize(int stepsize, int numbps, global opj_stepsize_t *bandno_stepsize) 
{
	int p, n;
	p = int_floorlog2(stepsize) - 13;
	n = 11 - int_floorlog2(stepsize);
	bandno_stepsize->mant = (n < 0 ? stepsize >> -n : stepsize << n) & 0x7ff;
	bandno_stepsize->expn = numbps - p;
}

void idwt_calc_explicit_stepsizes(global opj_tccp_t * tccp, int prec) 
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
/* ----------------------------------------------------------------------- */
/* 

CIO 

*/
/*
 * Get position in byte stream.
 */
int cio_tell(global opj_cio_t *cio) 
{
	return cio->cio_index ; //cio->bp - cio->start;
}

/*
 * Set position in byte stream.
 *
 * pos : position, in number of bytes, from the beginning of the stream
 */
void cio_seek(global opj_cio_t *cio, int pos) 
{
	cio->cio_index =pos ; // int_max(cio->cio_index , pos);
	//cio->bp = cio->start + pos;
}

/*
 * Number of bytes left before the end of the stream.
 */
int cio_numbytesleft(global opj_cio_t *cio) 
{
	return cio->length - cio->cio_index; //cio->end - cio->bp;
}

// Get pointer to the current position in the stream. 
global unsigned char *cio_getbp(global  opj_cio_t *cio) 
{
	return &cio->bp[cio->cio_index];
}

/*
 * Write a byte.
 */
opj_bool cio_byteout(global opj_cio_t *cio, unsigned char v) 
{	
	if (cio->bp >= cio->end)
	{
		return OPJ_FALSE;
	}
	cio->bp[cio->cio_index] = v;
	cio->cio_index ++;
	return OPJ_TRUE;
}

/*
 * Read a byte.
 */
unsigned char cio_bytein(global opj_cio_t *cio) 
{
	if (cio->bp >= cio->end) 
	{
//		opj_event_msg(cio->cinfo, EVT_ERROR, "read error: passed the end of the codestream (start = %d, current = %d, end = %d\n", cio->start, cio->bp, cio->end);
		return 0;
	}
                cio->cio_index++;
	return cio->bp[cio->cio_index];
}

/*
 * Write some bytes.
 *
 * v : value to write
 * n : number of bytes to write
 */
unsigned int cio_write(global opj_cio_t *cio, unsigned long int v, int n) 
{
	int i;
	
	for (i = n - 1; i >= 0; i--) 
	{
            unsigned char zz = (unsigned char) ((v >> (i << 3)) & 0xff);
 	        if( !cio_byteout(cio, zz) )
			    return 0;
	}
	return n;
}


/* 
 * Skip some bytes.
 *
 * n : number of bytes to skip
 */
void cio_skip(global opj_cio_t *cio, int n) 
{
	cio->cio_index += n;
}






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
		
		sft_memSet(allocationTable->preAllocated[i].start , 0 ,allocationTable->preAllocated[i].availableMem );

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
		sft_memSet(res , 0 ,num*size  );
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
		sft_memSet(allocationTable->preAllocated[i].start , 0 ,(allocationTable->preAllocated[i].origSize)*sizeof(char) );
	}

}

void sft_free(global void* m,global struct sft_MemAllocationTable* allocationTable)
{
	if ((allocationTable) && (allocationTable->preAllocated))
	{
		// Nothing to DO		
		return;
	}
	else
		return ;
	  //free(m);
}


void pi_destroy(global iopj_pi_iterator_t *pi, global opj_cp_t *cp, int tileno)
{
	/*int compno, pino;
	global opj_tcp_t *tcp = &cp->tcps[tileno];
	if(pi) {
		for (pino = 0; pino < tcp->numpocs + 1; pino++) {	
			if(pi[pino].comps) {
				for (compno = 0; compno < pi->numcomps; compno++) 
				{
					global iopj_pi_comp_t *comp = &pi[pino].comps[compno];
					if(comp->resolutions) {
						sft_free(comp->resolutions ,cp->allocationTable);
					}
				}
				sft_free(pi[pino].comps,cp->allocationTable);
			}
		}
		if(pi->include) 
		{
			sft_free(pi->include,cp->allocationTable);
		}
		sft_free(pi,cp->allocationTable);
	}
*/
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
/*
		for(y = 0; y < (int)H; y++) 
		{
			global unsigned char *scanline = RGB + (3 * W + PAD) * (H - 1 - y);
			for(x = 0; x < (int)W; x++) 
			{
			   global unsigned char *pixel = &scanline[3 * x];
			   RGB[0] = 1;
			   RGB[1] = 1;
			   
			}
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
	*/
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
// *****************************************************************************
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


kernel void clCreateCompress(global struct sft_MemAllocationTable* mt)
{
	uint thID = get_global_id(0);    
	if (thID > 0) return;
	
	OPJ_CODEC_FORMAT format = CODEC_J2K;
	global opj_cinfo_t *cinfo = (global opj_cinfo_t*)sft_calloc(1, sizeof(opj_cinfo_t),mt);
	if(!cinfo) return ;
	//cinfo->j2k_handle = (void*)ij2k_create_compress((opj_common_ptr)cinfo,mt);
	
	cinfo->is_decompressor = OPJ_FALSE;
	global opj_j2k_t *j2k = (global opj_j2k_t*) sft_calloc(1, sizeof(opj_j2k_t),mt);
	cinfo->j2k_handle = (void*)j2k;
	j2k->cinfo = (opj_common_ptr)cinfo;
	cinfo->codec_format = format;
	
	mt->globalVariables[SFT_CODEC] = (global char*) j2k;
	mt->globalVariables[SFT_INFO] = (global char*) cinfo;	
}

// setup the encoder parameters using the current image and user parameters 
kernel void clSetupEncoder(global struct sft_MemAllocationTable* mt)
{
	// iopj_setup_encoder(cinfo, parameters, image,mt);
	int i, j, tileno, numpocs_tile;
	global opj_cp_t *cp = NULL;
	global  opj_j2k_t *j2k = (global opj_j2k_t*) mt->globalVariables[SFT_CODEC];
	global  opj_image_t *image = (global opj_image_t *) mt->globalVariables[SFT_IMAGE];
	global  struct opj_cparameters* parameters = (global struct opj_cparameters*) mt->globalVariables[SFT_PARAMETERS];


	if(!j2k || !parameters || ! image) 
	{
		return ;
	}

	// create and initialize the coding parameters structure 
	cp = (global opj_cp_t*) sft_calloc(1, sizeof(opj_cp_t),mt);

// keep a link to cp so that we can destroy it later in j2k_destroy_compress 
    mt->globalVariables[SFT_CODING_PARAMS] = (global char *) cp;
	j2k->cp = cp;

	// set default values for cp 
	cp->tw = 1;
	cp->th = 1;

	// 
	// copy user encoding parameters 
	//
	cp->cinema = parameters->cp_cinema;
	cp->max_comp_size =	parameters->max_comp_size;
	cp->rsiz   = parameters->cp_rsiz;
	cp->disto_alloc = parameters->cp_disto_alloc;
	cp->fixed_alloc = parameters->cp_fixed_alloc;
	cp->fixed_quality = parameters->cp_fixed_quality;

	// mod fixed_quality 
	if(parameters->cp_matrice) 
	{
		unsigned int array_size = parameters->tcp_numlayers * parameters->numresolution * 3 * sizeof(int);
		cp->matrice = (global  int *) sft_malloc(array_size,mt );
		//sft_memCopy(cp->matrice, parameters->cp_matrice,0, array_size);
	}

	// tiles 
	cp->tdx = parameters->cp_tdx;
	cp->tdy = parameters->cp_tdy;

	// tile offset 
	cp->tx0 = parameters->cp_tx0;
	cp->ty0 = parameters->cp_ty0;

	// comment string 
	if(parameters->cp_comment) 
	{

	}

	//
	// calculate other encoding parameters
	//

	if (parameters->tile_size_on) {
		cp->tw = int_ceildiv(image->x1 - cp->tx0, cp->tdx);
		cp->th = int_ceildiv(image->y1 - cp->ty0, cp->tdy);
	} else {
		cp->tdx = image->x1 - cp->tx0;
		cp->tdy = image->y1 - cp->ty0;
	}

	if(parameters->tp_on)
               {
		cp->tp_flag = parameters->tp_flag;
		cp->tp_on = 1;
	}
	
	cp->img_size = 0;
	for(i=0;i<image->numcomps ;i++)
               {
	   cp->img_size += (image->comps[i].w *image->comps[i].h * image->comps[i].prec);
	}

// USE_JPWL 

	// initialize the mutiple tiles 
	// ---------------------------- 
	cp->tcps = (global opj_tcp_t*) sft_calloc(cp->tw * cp->th, sizeof(opj_tcp_t),mt);

	for (tileno = 0; tileno < cp->tw * cp->th; tileno++) 
	{
		global opj_tcp_t *tcp = &cp->tcps[tileno];
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

		if (parameters->numpocs) 
		{
			// initialisation of POC 
			tcp->POC = 1;
			for (i = 0; i < parameters->numpocs; i++) 
                                               {
				if((tileno == parameters->POC[i].tile - 1) || (parameters->POC[i].tile == -1)) 
                                                                {
					global opj_poc_t *tcp_poc = &tcp->pocs[numpocs_tile];
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

		tcp->tccps = (global opj_tccp_t*) sft_calloc(image->numcomps, sizeof(opj_tccp_t),mt);

		for (i = 0; i < image->numcomps; i++) 
                            {
			global opj_tccp_t *tccp = &tcp->tccps[i];
			tccp->csty = parameters->csty & 0x01;	// 0 => one precinct || 1 => custom precinct  
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
				//Precinct size for lowest frequency subband=128
				tccp->prcw[0] = 7;
				tccp->prch[0] = 7;
				//Precinct size at all other resolutions = 256
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
						//printf("\nsize precinct for level %d : %d,%d\n", j,tccp->prcw[j], tccp->prch[j]); 
					}	//end for
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
	
	return ;
}

kernel void opj_cio_open(global struct sft_MemAllocationTable* mt) 
{
	global opj_cp_t *cp = NULL;
	global  opj_cio_t *cio = (global opj_cio_t*)sft_malloc(sizeof(opj_cio_t),mt);
	opj_common_ptr cinfo = (opj_common_ptr) mt->globalVariables[SFT_INFO] ;
	if(!cio) return ;
	cio->cinfo = cinfo;
	cio->cio_index=0;
                global unsigned char *buffer = NULL ;  
                int length = 0;
	if(buffer && length) 
	{
		// wrap a user buffer containing the encoded image 
		cio->openmode = OPJ_STREAM_READ;
		cio->buffer = buffer;
		cio->length = length;
	}
	else 
	if(!buffer && !length && cinfo) 
	{
		// allocate a buffer for the encoded image 
		cio->openmode = OPJ_STREAM_WRITE;
		cp = ((opj_j2k_t*)cinfo->j2k_handle)->cp;

		cio->length = (unsigned int) (0.1625 * cp->img_size + 2000); // ** 0.1625 = 1.3/8 and 2000 bytes as a minimum for headers 
		cio->buffer = (global unsigned char *)sft_malloc(cio->length,mt);
		if(!cio->buffer) 
		{
			//opj_event_msg(cio->cinfo, EVT_ERROR, "Error allocating memory for compressed bitstream\n");
			//sft_free(cio,mt);
			return ;
		}
	}
	else 
                  {
		//sft_free(cio,mt);
		return ;
	}

	// Initialize byte IO 
	cio->start = cio->buffer;
	cio->end = cio->buffer + cio->length;
	cio->bp = cio->buffer;

	return ;

}


void ij2k_write_soc(global opj_j2k_t *j2k) 
{
	cio_write(j2k->cio, J2K_MS_SOC, 2);
}

void ij2k_write_siz(global opj_j2k_t *j2k) 
{
	int i;
	int lenp, len;

	global opj_cio_t *cio = j2k->cio;
	global opj_image_t *image = j2k->image;
	global opj_cp_t *cp = j2k->cp;

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

void ij2k_write_cod(global opj_j2k_t *j2k,int curtileno) 
{
	global opj_cp_t *cp = NULL;
	global opj_tcp_t *tcp = NULL;
	int lenp, len;
	int i;

	global opj_cio_t *cio = j2k->cio;
	global  opj_tccp_t *tccp = NULL;

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

void ij2k_write_qcd(global opj_j2k_t *j2k, int curtileno) 
{
	int lenp, len;
	int bandno, numbands;
	int expn, mant;
	int compno = 0;
	global opj_cio_t *cio = j2k->cio;	
	global opj_cp_t *cp = j2k->cp;
	global opj_tcp_t *tcp = &cp->tcps[curtileno];
	global opj_tccp_t *tccp = &tcp->tccps[compno];
	
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

void ij2k_write_rgn(global opj_j2k_t *j2k, int compno, int tileno) 
{
	global opj_cp_t *cp = j2k->cp;
	global opj_tcp_t *tcp = &cp->tcps[tileno];
	global opj_cio_t *cio = j2k->cio;
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

global iopj_pi_iterator_t *ipi_initialise_encode(global opj_image_t *image, global opj_cp_t *cp, int tileno, J2K_T2_MODE t2_mode,global struct sft_MemAllocationTable* mt)
{ 
	int p, q, pino;
	int compno, resno;
	int maxres = 0;
	int maxprec = 0;
	global iopj_pi_iterator_t *pi = NULL;
	global opj_tcp_t *tcp = NULL;
	global opj_tccp_t *tccp = NULL;
	
	tcp = &cp->tcps[tileno];

	pi = (global iopj_pi_iterator_t*) sft_calloc((tcp->numpocs + 1), sizeof(iopj_pi_iterator_t),mt);
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
		
		pi[pino].comps = (global iopj_pi_comp_t*) sft_calloc(image->numcomps, sizeof(iopj_pi_comp_t),mt);
		if(!pi[pino].comps) 
		{
			pi_destroy(pi, cp, tileno);
			return NULL;
		}
		
		for (compno = 0; compno < pi[pino].numcomps; compno++) 
		{
			int tcx0, tcy0, tcx1, tcy1;
			global iopj_pi_comp_t *comp = &pi[pino].comps[compno];
			tccp = &tcp->tccps[compno];
			comp->dx = image->comps[compno].dx;
			comp->dy = image->comps[compno].dy;
			comp->numresolutions = tccp->numresolutions;

			comp->resolutions = (global iopj_pi_resolution_t*) sft_malloc(comp->numresolutions * sizeof(iopj_pi_resolution_t),mt);
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
				global iopj_pi_resolution_t *res = &comp->resolutions[resno];
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
			global iopj_pi_comp_t *comp = &pi->comps[compno];
			for (resno = 0; resno < comp->numresolutions; resno++) 
			{
				int dx, dy;
				global iopj_pi_resolution_t *res = &comp->resolutions[resno];
				dx = comp->dx * (1 << (res->pdx + comp->numresolutions - 1 - resno));
				dy = comp->dy * (1 << (res->pdy + comp->numresolutions - 1 - resno));
				pi[pino].dx = !pi->dx ? dx : int_min(pi->dx, dx);
				pi[pino].dy = !pi->dy ? dy : int_min(pi->dy, dy);
			}
		}

		if (pino == 0) 
		{
			pi[pino].include = (global short int*) sft_calloc(tcp->numlayers * pi[pino].step_l, sizeof(short int),mt);
			if(!pi[pino].include) {
				pi_destroy(pi, cp, tileno);
				return NULL;
			}
		}
		else 
		{
			pi[pino].include = pi[pino - 1].include;
		}
		
		// Generation of boundaries for each prog flag
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
void clJ2K_calculate_tp(global opj_cp_t *cp,int img_numcomp, global opj_image_t *image, global opj_j2k_t *j2k,   global struct sft_MemAllocationTable* mt )
{
	int pino,tileno,totnum_tp=0;

	OPJ_ARG_NOT_USED(img_numcomp);
	j2k->totnum_tp = -1;
	j2k->cur_totnum_tp = (global int *) sft_malloc(cp->tw * cp->th * sizeof(int),mt);
	for (tileno = 0; tileno < cp->tw * cp->th; tileno++) 
	{
		int cur_totnum_tp = 0;
		global opj_tcp_t *tcp = &cp->tcps[tileno];
		for(pino = 0; pino <= tcp->numpocs; pino++) 
		{
			int tp_num=0;
			global iopj_pi_iterator_t *pi = ipi_initialise_encode(image, cp, tileno,FINAL_PASS,mt);
			if(!pi) { return ;}
			tp_num = 1 ; // ij2k_get_num_tp(cp,pino,tileno);
			totnum_tp = totnum_tp + tp_num;
			cur_totnum_tp = cur_totnum_tp + tp_num;
			//pi_destroy(pi, cp, tileno);
		}
		j2k->cur_totnum_tp[tileno] = cur_totnum_tp;

		// << INDEX 
	}
	j2k->totnum_tp = totnum_tp;
	return ;// totnum_tp;
}

kernel void clJ2K_encode0(global struct sft_MemAllocationTable* mt) 
{
	int tileno, compno , tasks ;	
	global opj_cp_t *cp = NULL;
	
	global opj_image_t *image = (global opj_image_t *) mt->globalVariables[SFT_IMAGE];
	global opj_j2k_t *j2k = (global opj_j2k_t*)mt->globalVariables[SFT_CODEC];
	global opj_cio_t *cio= (global opj_cio_t*)mt->globalVariables[SFT_CIO];
	j2k->cio = cio;	
	j2k->image = image;

	cp = j2k->cp;
	
	//Preparing Headers
	// INDEX >> 
	
                 ij2k_write_soc(j2k);
	ij2k_write_siz(j2k);
	ij2k_write_cod(j2k,0);
	ij2k_write_qcd(j2k,0);

	for (compno = 0; compno < image->numcomps; compno++) 
	{
		global opj_tcp_t *tcp = &cp->tcps[0];
		if (tcp->tccps[compno].roishift)
			ij2k_write_rgn(j2k, compno, 0);
	}

    clJ2K_calculate_tp(cp,image->numcomps,image,j2k,mt);
	
	
}

#endif