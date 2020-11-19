#define NULL 0

#pragma OPENCL EXTENSION cl_khr_fp64 : enable

typedef int opj_bool;
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

#define OPJ_TRUE 1
#define OPJ_FALSE 0

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
	int *data;
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
	opj_image_comp_t *comps;
	/** 'restricted' ICC profile */
	unsigned char *icc_profile_buf;
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

#define MQC_NUMCTXS 19

/**
MQ coder
*/
typedef struct opj_mqc {
	unsigned int c;
	unsigned int a;
	unsigned int ct;
	unsigned char *bp;
	unsigned char *start;
	unsigned char *end;
	opj_mqc_state_t *ctxs[MQC_NUMCTXS];
	opj_mqc_state_t **curctx;
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

void* sft_calloc(size_t num , size_t size) { return NULL;}
void* sft_malloc(size_t size) { return NULL;}
void sft_free(void* m){}
void* sft_aligned_malloc(size_t size){ return NULL;}

void sft_memSet( void* memmory, char value,int  dataSize) { }


/* <summary> */
/* This array defines all the possible states for a context. */
/* </summary> */

/* This file was automatically generated by t1_generate_luts.c */
constant char lut_ctxno_sc[256] = {
  0x9, 0xa, 0xc, 0xd, 0xa, 0xa, 0xd, 0xd, 0xc, 0xd, 0xc, 0xd, 0xd, 0xd, 0xd, 0xd, 
  0x9, 0xa, 0xc, 0xb, 0xa, 0x9, 0xd, 0xc, 0xc, 0xb, 0xc, 0xb, 0xd, 0xc, 0xd, 0xc, 
  0x9, 0xa, 0xc, 0xb, 0xa, 0xa, 0xb, 0xb, 0xc, 0xd, 0x9, 0xa, 0xd, 0xd, 0xa, 0xa, 
  0x9, 0xa, 0xc, 0xd, 0xa, 0x9, 0xb, 0xc, 0xc, 0xb, 0x9, 0xa, 0xd, 0xc, 0xa, 0x9, 
  0x9, 0xa, 0xc, 0xd, 0xa, 0x9, 0xb, 0xc, 0xc, 0xd, 0xc, 0xd, 0xb, 0xc, 0xb, 0xc, 
  0x9, 0xa, 0xc, 0xb, 0xa, 0xa, 0xb, 0xb, 0xc, 0xb, 0xc, 0xb, 0xb, 0xb, 0xb, 0xb, 
  0x9, 0xa, 0xc, 0xb, 0xa, 0x9, 0xd, 0xc, 0xc, 0xd, 0x9, 0xa, 0xb, 0xc, 0xa, 0x9, 
  0x9, 0xa, 0xc, 0xd, 0xa, 0xa, 0xd, 0xd, 0xc, 0xb, 0x9, 0xa, 0xb, 0xb, 0xa, 0xa, 
  0x9, 0xa, 0xc, 0xd, 0xa, 0xa, 0xd, 0xd, 0xc, 0xb, 0x9, 0xa, 0xb, 0xb, 0xa, 0xa, 
  0x9, 0xa, 0xc, 0xb, 0xa, 0x9, 0xd, 0xc, 0xc, 0xd, 0x9, 0xa, 0xb, 0xc, 0xa, 0x9, 
  0x9, 0xa, 0xc, 0xb, 0xa, 0xa, 0xb, 0xb, 0xc, 0xb, 0xc, 0xb, 0xb, 0xb, 0xb, 0xb, 
  0x9, 0xa, 0xc, 0xd, 0xa, 0x9, 0xb, 0xc, 0xc, 0xd, 0xc, 0xd, 0xb, 0xc, 0xb, 0xc, 
  0x9, 0xa, 0xc, 0xd, 0xa, 0x9, 0xb, 0xc, 0xc, 0xb, 0x9, 0xa, 0xd, 0xc, 0xa, 0x9, 
  0x9, 0xa, 0xc, 0xb, 0xa, 0xa, 0xb, 0xb, 0xc, 0xd, 0x9, 0xa, 0xd, 0xd, 0xa, 0xa, 
  0x9, 0xa, 0xc, 0xb, 0xa, 0x9, 0xd, 0xc, 0xc, 0xb, 0xc, 0xb, 0xd, 0xc, 0xd, 0xc, 
  0x9, 0xa, 0xc, 0xd, 0xa, 0xa, 0xd, 0xd, 0xc, 0xd, 0xc, 0xd, 0xd, 0xd, 0xd, 0xd
};

constant  char lut_spb[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 
  0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 
  0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
  0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 
  0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};
constant  char lut_ctxno_zc[1024] = {
  0, 1, 1, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
  5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
  5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
  0, 1, 1, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
  5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
  0, 1, 1, 2, 1, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
  5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
  5, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
  7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
  8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
  0, 3, 3, 6, 3, 6, 6, 8, 3, 6, 6, 8, 6, 8, 8, 8, 1, 4, 4, 7, 4, 7, 7, 8, 4, 7, 7, 8, 7, 8, 8, 8, 
  1, 4, 4, 7, 4, 7, 7, 8, 4, 7, 7, 8, 7, 8, 8, 8, 2, 5, 5, 7, 5, 7, 7, 8, 5, 7, 7, 8, 7, 8, 8, 8, 
  1, 4, 4, 7, 4, 7, 7, 8, 4, 7, 7, 8, 7, 8, 8, 8, 2, 5, 5, 7, 5, 7, 7, 8, 5, 7, 7, 8, 7, 8, 8, 8, 
  2, 5, 5, 7, 5, 7, 7, 8, 5, 7, 7, 8, 7, 8, 8, 8, 2, 5, 5, 7, 5, 7, 7, 8, 5, 7, 7, 8, 7, 8, 8, 8, 
  1, 4, 4, 7, 4, 7, 7, 8, 4, 7, 7, 8, 7, 8, 8, 8, 2, 5, 5, 7, 5, 7, 7, 8, 5, 7, 7, 8, 7, 8, 8, 8, 
  2, 5, 5, 7, 5, 7, 7, 8, 5, 7, 7, 8, 7, 8, 8, 8, 2, 5, 5, 7, 5, 7, 7, 8, 5, 7, 7, 8, 7, 8, 8, 8, 
  2, 5, 5, 7, 5, 7, 7, 8, 5, 7, 7, 8, 7, 8, 8, 8, 2, 5, 5, 7, 5, 7, 7, 8, 5, 7, 7, 8, 7, 8, 8, 8, 
  2, 5, 5, 7, 5, 7, 7, 8, 5, 7, 7, 8, 7, 8, 8, 8, 2, 5, 5, 7, 5, 7, 7, 8, 5, 7, 7, 8, 7, 8, 8, 8
};


constant  short lut_nmsedec_sig[1 << T1_NMSEDEC_BITS] = {
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0180, 0x0300, 0x0480, 0x0600, 0x0780, 0x0900, 0x0a80, 
  0x0c00, 0x0d80, 0x0f00, 0x1080, 0x1200, 0x1380, 0x1500, 0x1680, 
  0x1800, 0x1980, 0x1b00, 0x1c80, 0x1e00, 0x1f80, 0x2100, 0x2280, 
  0x2400, 0x2580, 0x2700, 0x2880, 0x2a00, 0x2b80, 0x2d00, 0x2e80, 
  0x3000, 0x3180, 0x3300, 0x3480, 0x3600, 0x3780, 0x3900, 0x3a80, 
  0x3c00, 0x3d80, 0x3f00, 0x4080, 0x4200, 0x4380, 0x4500, 0x4680, 
  0x4800, 0x4980, 0x4b00, 0x4c80, 0x4e00, 0x4f80, 0x5100, 0x5280, 
  0x5400, 0x5580, 0x5700, 0x5880, 0x5a00, 0x5b80, 0x5d00, 0x5e80, 
  0x6000, 0x6180, 0x6300, 0x6480, 0x6600, 0x6780, 0x6900, 0x6a80, 
  0x6c00, 0x6d80, 0x6f00, 0x7080, 0x7200, 0x7380, 0x7500, 0x7680
};

constant  short lut_nmsedec_sig0[1 << T1_NMSEDEC_BITS] = {
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0080, 0x0080, 
  0x0080, 0x0080, 0x0100, 0x0100, 0x0100, 0x0180, 0x0180, 0x0200, 
  0x0200, 0x0280, 0x0280, 0x0300, 0x0300, 0x0380, 0x0400, 0x0400, 
  0x0480, 0x0500, 0x0580, 0x0580, 0x0600, 0x0680, 0x0700, 0x0780, 
  0x0800, 0x0880, 0x0900, 0x0980, 0x0a00, 0x0a80, 0x0b80, 0x0c00, 
  0x0c80, 0x0d00, 0x0e00, 0x0e80, 0x0f00, 0x1000, 0x1080, 0x1180, 
  0x1200, 0x1300, 0x1380, 0x1480, 0x1500, 0x1600, 0x1700, 0x1780, 
  0x1880, 0x1980, 0x1a80, 0x1b00, 0x1c00, 0x1d00, 0x1e00, 0x1f00, 
  0x2000, 0x2100, 0x2200, 0x2300, 0x2400, 0x2500, 0x2680, 0x2780, 
  0x2880, 0x2980, 0x2b00, 0x2c00, 0x2d00, 0x2e80, 0x2f80, 0x3100, 
  0x3200, 0x3380, 0x3480, 0x3600, 0x3700, 0x3880, 0x3a00, 0x3b00, 
  0x3c80, 0x3e00, 0x3f80, 0x4080, 0x4200, 0x4380, 0x4500, 0x4680, 
  0x4800, 0x4980, 0x4b00, 0x4c80, 0x4e00, 0x4f80, 0x5180, 0x5300, 
  0x5480, 0x5600, 0x5800, 0x5980, 0x5b00, 0x5d00, 0x5e80, 0x6080, 
  0x6200, 0x6400, 0x6580, 0x6780, 0x6900, 0x6b00, 0x6d00, 0x6e80, 
  0x7080, 0x7280, 0x7480, 0x7600, 0x7800, 0x7a00, 0x7c00, 0x7e00
};

constant  short lut_nmsedec_ref[1 << T1_NMSEDEC_BITS] = {
  0x1800, 0x1780, 0x1700, 0x1680, 0x1600, 0x1580, 0x1500, 0x1480, 
  0x1400, 0x1380, 0x1300, 0x1280, 0x1200, 0x1180, 0x1100, 0x1080, 
  0x1000, 0x0f80, 0x0f00, 0x0e80, 0x0e00, 0x0d80, 0x0d00, 0x0c80, 
  0x0c00, 0x0b80, 0x0b00, 0x0a80, 0x0a00, 0x0980, 0x0900, 0x0880, 
  0x0800, 0x0780, 0x0700, 0x0680, 0x0600, 0x0580, 0x0500, 0x0480, 
  0x0400, 0x0380, 0x0300, 0x0280, 0x0200, 0x0180, 0x0100, 0x0080, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0080, 0x0100, 0x0180, 0x0200, 0x0280, 0x0300, 0x0380, 
  0x0400, 0x0480, 0x0500, 0x0580, 0x0600, 0x0680, 0x0700, 0x0780, 
  0x0800, 0x0880, 0x0900, 0x0980, 0x0a00, 0x0a80, 0x0b00, 0x0b80, 
  0x0c00, 0x0c80, 0x0d00, 0x0d80, 0x0e00, 0x0e80, 0x0f00, 0x0f80, 
  0x1000, 0x1080, 0x1100, 0x1180, 0x1200, 0x1280, 0x1300, 0x1380, 
  0x1400, 0x1480, 0x1500, 0x1580, 0x1600, 0x1680, 0x1700, 0x1780
};

constant  short lut_nmsedec_ref0[1 << T1_NMSEDEC_BITS] = {
  0x2000, 0x1f00, 0x1e00, 0x1d00, 0x1c00, 0x1b00, 0x1a80, 0x1980, 
  0x1880, 0x1780, 0x1700, 0x1600, 0x1500, 0x1480, 0x1380, 0x1300, 
  0x1200, 0x1180, 0x1080, 0x1000, 0x0f00, 0x0e80, 0x0e00, 0x0d00, 
  0x0c80, 0x0c00, 0x0b80, 0x0a80, 0x0a00, 0x0980, 0x0900, 0x0880, 
  0x0800, 0x0780, 0x0700, 0x0680, 0x0600, 0x0580, 0x0580, 0x0500, 
  0x0480, 0x0400, 0x0400, 0x0380, 0x0300, 0x0300, 0x0280, 0x0280, 
  0x0200, 0x0200, 0x0180, 0x0180, 0x0100, 0x0100, 0x0100, 0x0080, 
  0x0080, 0x0080, 0x0080, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 
  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0080, 0x0080, 
  0x0080, 0x0080, 0x0100, 0x0100, 0x0100, 0x0180, 0x0180, 0x0200, 
  0x0200, 0x0280, 0x0280, 0x0300, 0x0300, 0x0380, 0x0400, 0x0400, 
  0x0480, 0x0500, 0x0580, 0x0580, 0x0600, 0x0680, 0x0700, 0x0780, 
  0x0800, 0x0880, 0x0900, 0x0980, 0x0a00, 0x0a80, 0x0b80, 0x0c00, 
  0x0c80, 0x0d00, 0x0e00, 0x0e80, 0x0f00, 0x1000, 0x1080, 0x1180, 
  0x1200, 0x1300, 0x1380, 0x1480, 0x1500, 0x1600, 0x1700, 0x1780, 
  0x1880, 0x1980, 0x1a80, 0x1b00, 0x1c00, 0x1d00, 0x1e00, 0x1f00
};


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
constant double dwt_norms[4][10] = {
	{1.000, 1.500, 2.750, 5.375, 10.68, 21.34, 42.67, 85.33, 170.7, 341.3},
	{1.038, 1.592, 2.919, 5.703, 11.33, 22.64, 45.25, 90.48, 180.9},
	{1.038, 1.592, 2.919, 5.703, 11.33, 22.64, 45.25, 90.48, 180.9},
	{.7186, .9218, 1.586, 3.043, 6.019, 12.01, 24.00, 47.97, 95.93}
};

/* <summary> */
/* This table contains the norms of the basis function of the reversible MCT. */
/* </summary> */
constant  double mct_norms[3] = { 1.732, .8292, .8292 };


#define mqc_setcurctx(mqc, ctxno)	(mqc)->curctx = &(mqc)->ctxs[(int)(ctxno)]

/* <summary> */
/* This array defines all the possible states for a context. */
/* </summary> */
constant opj_mqc_stateI_t mqc_statesI[47 * 2] = {
	{0x5601, 0, 2,3 },
	{0x5601, 1, 3,2 },
	{0x3401, 0, 4,12 },
	{0x3401, 1, 5,13},
	{0x1801, 0, 6,18},
	{0x1801, 1, 7,19},
	{0x0ac1, 0, 8,24},
	{0x0ac1, 1, 9,25},
	{0x0521, 0, 10,58},
	{0x0521, 1, 11,59},
	{0x0221, 0, 76,66},
	{0x0221, 1, 77,67},
	{0x5601, 0, 14,13},
	{0x5601, 1, 15,12},
	{0x5401, 0, 16,28},
	{0x5401, 1, 17,29},
	{0x4801, 0, 18,28},
	{0x4801, 1, 19,29},
	{0x3801, 0, 20,28},
	{0x3801, 1, 21,29},
	{0x3001, 0, 22,34},
	{0x3001, 1, 23,35},
	{0x2401, 0, 24,36},
	{0x2401, 1, 25,37},
	{0x1c01, 0, 26,40},
	{0x1c01, 1, 27,41},
	{0x1601, 0, 58,42},
	{0x1601, 1, 59,43},
	{0x5601, 0, 30,29},
	{0x5601, 1, 31,28},
	{0x5401, 0, 32,28},
	{0x5401, 1, 33, 29},
	{0x5101, 0, 34, 30},
	{0x5101, 1, 35, 31},
	{0x4801, 0, 36, 32},
	{0x4801, 1, 37, 33},
	{0x3801, 0, 38, 34},
	{0x3801, 1, 39, 35},
	{0x3401, 0, 40, 36},
	{0x3401, 1, 41, 37},
	{0x3001, 0, 42, 38},
	{0x3001, 1, 43, 39},
	{0x2801, 0, 44, 38},
	{0x2801, 1, 45, 39},
	{0x2401, 0, 46, 40},
	{0x2401, 1, 47, 41},
	{0x2201, 0, 48, 42},
	{0x2201, 1, 49, 43},
	{0x1c01, 0, 50, 44},
	{0x1c01, 1, 51, 45},
	{0x1801, 0, 52, 46},
	{0x1801, 1, 53, 47},
	{0x1601, 0, 54, 48},
	{0x1601, 1, 55, 49},
	{0x1401, 0, 56, 50},
	{0x1401, 1, 57, 51},
	{0x1201, 0, 58, 52},
	{0x1201, 1, 59, 53},
	{0x1101, 0, 60, 54},
	{0x1101, 1, 61, 55},
	{0x0ac1, 0, 62, 56},
	{0x0ac1, 1, 63, 57},
	{0x09c1, 0, 64, 58},
	{0x09c1, 1, 65, 59},
	{0x08a1, 0, 66, 60},
	{0x08a1, 1, 67, 61},
	{0x0521, 0, 68, 62},
	{0x0521, 1, 69, 63},
	{0x0441, 0, 70, 64},
	{0x0441, 1, 71, 65},
	{0x02a1, 0, 72, 66},
	{0x02a1, 1, 73, 67},
	{0x0221, 0, 74, 68},
	{0x0221, 1, 75, 69},
	{0x0141, 0, 76, 70},
	{0x0141, 1, 77, 71},
	{0x0111, 0, 78, 72},
	{0x0111, 1, 79, 73},
	{0x0085, 0, 80, 74},
	{0x0085, 1, 81, 75},
	{0x0049, 0, 82, 76},
	{0x0049, 1, 83, 77},
	{0x0025, 0, 84, 78},
	{0x0025, 1, 85, 79},
	{0x0015, 0, 86, 80},
	{0x0015, 1, 87, 81},
	{0x0009, 0, 88, 82},
	{0x0009, 1, 89, 83},
	{0x0005, 0, 90, 84},
	{0x0005, 1, 91, 85},
	{0x0001, 0, 90, 86},
	{0x0001, 1, 91, 87},
	{0x5601, 0, 92, 92},
	{0x5601, 1, 93, 93},
};

/*
  MQC
*/

void mqc_resetstates(opj_mqc_t *mqc) {
	int i;
	for (i = 0; i < MQC_NUMCTXS; i++) {
		mqc->ctxs[i] = mqc_states;
	}
}

void mqc_setstate(opj_mqc_t *mqc, int ctxno, int msb, int prob) {
	mqc->ctxs[ctxno] = &mqc_states[msb + (prob << 1)];
}

void mqc_init_enc(opj_mqc_t *mqc, unsigned char *bp) 
{
	mqc_setcurctx(mqc, 0);
	mqc->a = 0x8000;
	mqc->c = 0;
	mqc->bp = bp - 1;
	mqc->ct = 12;
	if (*mqc->bp == 0xff) 
	{
		mqc->ct = 13;
	}
	mqc->start = bp;
}

void mqc_byteout(opj_mqc_t *mqc) {
	if (*mqc->bp == 0xff) {
		mqc->bp++;
		*mqc->bp = mqc->c >> 20;
		mqc->c &= 0xfffff;
		mqc->ct = 7;
	} else {
		if ((mqc->c & 0x8000000) == 0) {	/* ((mqc->c&0x8000000)==0) CHANGE */
			mqc->bp++;
			*mqc->bp = mqc->c >> 19;
			mqc->c &= 0x7ffff;
			mqc->ct = 8;
		} else {
			(*mqc->bp)++;
			if (*mqc->bp == 0xff) {
				mqc->c &= 0x7ffffff;
				mqc->bp++;
				*mqc->bp = mqc->c >> 20;
				mqc->c &= 0xfffff;
				mqc->ct = 7;
			} else {
				mqc->bp++;
				*mqc->bp = mqc->c >> 19;
				mqc->c &= 0x7ffff;
				mqc->ct = 8;
			}
		}
	}
}

void mqc_setbits(opj_mqc_t *mqc) 
{
	unsigned int tempc = mqc->c + mqc->a;
	mqc->c |= 0xffff;
	if (mqc->c >= tempc) {
		mqc->c -= 0x8000;
	}
}


void mqc_flush(opj_mqc_t *mqc) {
	mqc_setbits(mqc);
	mqc->c <<= mqc->ct;
	mqc_byteout(mqc);
	mqc->c <<= mqc->ct;
	mqc_byteout(mqc);
	
	if (*mqc->bp != 0xff) {
		mqc->bp++;
	}
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

/* <summary>                            */
/* Forward 5-3 wavelet transform in 2-D. */
/* </summary>                           */
void idwt_encode(opj_tcd_tilecomp_t * tilec) 
{
}

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



char it1_getctxno_zc(int f, int orient)
{
	return lut_ctxno_zc[(orient << 8) | (f & T1_SIG_OTH)];
}

char it1_getctxno_sc(int f)
{
	return lut_ctxno_sc[(f & (T1_SIG_PRIM | T1_SGN)) >> 4];
}

char it1_getspb(int f) 
{
	return lut_spb[(f & (T1_SIG_PRIM | T1_SGN)) >> 4];
}


short it1_getnmsedec_sig(int x, int bitpos) 
{
	if (bitpos > T1_NMSEDEC_FRACBITS) {
		return lut_nmsedec_sig[(x >> (bitpos - T1_NMSEDEC_FRACBITS)) & ((1 << T1_NMSEDEC_BITS) - 1)];
	}

	return lut_nmsedec_sig0[x & ((1 << T1_NMSEDEC_BITS) - 1)];
}

/* Get norm of basis function of reversible MCT. */
/* </summary> */
double mct_getnorm(int compno) {
	return mct_norms[compno];
}

int mqc_numbytes(opj_mqc_t *mqc) {
	return mqc->bp - mqc->start;
}


void mqc_renorme(opj_mqc_t *mqc) 
{
	do {
		mqc->a <<= 1;
		mqc->c <<= 1;
		mqc->ct--;
		if (mqc->ct == 0) {
			mqc_byteout(mqc);
		}
	} while ((mqc->a & 0x8000) == 0);
}


void mqc_codemps(opj_mqc_t *mqc) {
	mqc->a -= (*mqc->curctx)->qeval;
	if ((mqc->a & 0x8000) == 0) {
		if (mqc->a < (*mqc->curctx)->qeval) {
			mqc->a = (*mqc->curctx)->qeval;
		} else {
			mqc->c += (*mqc->curctx)->qeval;
		}
		*mqc->curctx = (*mqc->curctx)->nmps;
		mqc_renorme(mqc);
	} else {
		mqc->c += (*mqc->curctx)->qeval;
	}
}

void mqc_codelps(opj_mqc_t *mqc) {
	mqc->a -= (*mqc->curctx)->qeval;
	if (mqc->a < (*mqc->curctx)->qeval) {
		mqc->c += (*mqc->curctx)->qeval;
	} else {
		mqc->a = (*mqc->curctx)->qeval;
	}
	*mqc->curctx = (*mqc->curctx)->nlps;
	mqc_renorme(mqc);
}
void mqc_bypass_init_enc(opj_mqc_t *mqc) {
	mqc->c = 0;
	mqc->ct = 8;
	/*if (*mqc->bp == 0xff) {
	mqc->ct = 7;
     } */
}


void mqc_encode(opj_mqc_t *mqc, int d) {
	if ((*mqc->curctx)->mps == d) {
		mqc_codemps(mqc);
	} else {
		mqc_codelps(mqc);
	}
}

void mqc_bypass_enc(opj_mqc_t *mqc, int d) {
	mqc->ct--;
	mqc->c = mqc->c + (d << mqc->ct);
	if (mqc->ct == 0) {
		mqc->bp++;
		*mqc->bp = mqc->c;
		mqc->ct = 8;
		if (*mqc->bp == 0xff) {
			mqc->ct = 7;
		}
		mqc->c = 0;
	}
}

int it1_getctxno_mag(int f) 
{
	int tmp1 = (f & T1_SIG_OTH) ? T1_CTXNO_MAG + 1 : T1_CTXNO_MAG;
	int tmp2 = (f & T1_REFINE) ? T1_CTXNO_MAG + 2 : tmp1;
	return (tmp2);
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

/* <summary>                */
/* Get norm of 5-3 wavelet. */
/* </summary>               */
double idwt_getnorm(int level, int orient) 
{
	return dwt_norms[orient][level];
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

void mqc_reset_enc(opj_mqc_t *mqc) {
	mqc_resetstates(mqc);
	mqc_setstate(mqc, T1_CTXNO_UNI, 0, 46);
	mqc_setstate(mqc, T1_CTXNO_AGG, 0, 3);
	mqc_setstate(mqc, T1_CTXNO_ZC, 0, 4);
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


void mqc_segmark_enc(opj_mqc_t *mqc) {
	int i;
	mqc_setcurctx(mqc, 18);
	
	for (i = 1; i < 5; i++) {
		mqc_encode(mqc, i % 2);
	}
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
		
		/*-----------RATE-ALLOCATE------------------*/
		/* INDEX */
		/* Normal Rate/distortion allocation */
		itcd_rateallocate(tcd->tcd_tile , tcd,  len, cstr_info);
	}
	/*--------------TIER2------------------*/
	/* INDEX */
	t2 = it2_create(tcd->cinfo, image, cp);
	//JUAN l = it2_encode_packets(t2,tileno, tile, tcd_tcp->numlayers, dest, len, cstr_info,
	//	tcd->tp_num,tcd->tp_pos,tcd->cur_pino,FINAL_PASS,
	//	j2k->cur_totnum_tp[tileno]);
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
		
	return l;
}


// *****************************************************************************/
//*****************************************************************************/
/*
__kernel void Sample3( int offset)
{
// get index into global data array
    uint thID = get_global_id(0);    

  ij2k_encode_Tile(NULL , NULL, NULL , 0,0,0);
}
*/
