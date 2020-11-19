
#include "openjpeg.h"
/* -------------------------------------------------------------------------- */
/* -->> -->> -->> -->>

BMP IMAGE FORMAT

<<-- <<-- <<-- <<-- */

/* WORD defines a two byte word */
typedef unsigned short int WORD;

/* DWORD defines a four byte word */
typedef unsigned long int DWORD;

typedef struct dircnt{
	/** Buffer for holding images read from Directory*/
	char *filename_buf;
	/** Pointer to the buffer*/
	char **filename;
}dircnt_t;

typedef struct img_folder{
	/** The directory path of the folder containing input images*/
	char *imgdirpath;
	/** Output format*/
	char *out_format;
	/** Enable option*/
	char set_imgdir;
	/** Enable Cod Format for output*/
	char set_out_format;
	/** User specified rate stored in case of cinema option*/
	float *rates;
}img_fol_t;



typedef struct 
{
	WORD bfType;			// 'BM' for Bitmap (19776) 
	DWORD bfSize;			// Size of the file        
	WORD bfReserved1;		// Reserved : 0            
	WORD bfReserved2;		// Reserved : 0            
	DWORD bfOffBits;		// Offset                  
} BITMAPFILEHEADER_t;

typedef struct 
{
	DWORD biSize;			// Size of the structure in bytes 
	DWORD biWidth;		// Width of the image in pixels 
	DWORD biHeight;		// Heigth of the image in pixels 
	WORD biPlanes;		// 1 
	WORD biBitCount;		// Number of color bits by pixels 
	DWORD biCompression;		// Type of encoding 0: none 1: RLE8 2: RLE4 
	DWORD biSizeImage;		// Size of the image in bytes 
	DWORD biXpelsPerMeter;	// Horizontal (X) resolution in pixels/meter 
	DWORD biYpelsPerMeter;	// Vertical (Y) resolution in pixels/meter 
	DWORD biClrUsed;		// Number of color used in the image (0: ALL) 
	DWORD biClrImportant;		// Number of important color (0: ALL) 
} BITMAPINFOHEADER_t;

void iopj_image_destroy(opj_image_t *image);
opj_image_t* iopj_image_create(int numcmpts, opj_image_cmptparm_t *cmptparms, OPJ_COLOR_SPACE clrspc);
opj_image_t* ibmptoimage(const char *filename, opj_cparameters_t *parameters);
