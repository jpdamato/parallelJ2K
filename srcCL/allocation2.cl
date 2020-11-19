#define NULL 0
/* 
==========================================================
   image typedef definitions
==========================================================
*/
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
Defines a single image component
*/
typedef struct opj_image_comp 
{
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
	global opj_image_comp_t *comps;
	/** 'restricted' ICC profile */
	global char *icc_profile_buf;
	/** size of ICC profile */
	int icc_profile_len;
	
	int offsetComps ;
} opj_image_t;

typedef struct sft_AllocatedBlock_t
{
	char* start;
	char* activePos;
	int availableMem;
	int usedMem ;
	int origSize;
	int offset;
	
} sft_AllocatedBlock;

typedef struct sft_SampleX
{
    int id;
	int usedMem ;
} sft_SampleStruct ;

/*
 
 Integer functions
  
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
/* ----------------------------------------------------------------------- */

/*
 
 Memmory functions
  
*/
void sft_memSet(int threadID, struct sft_AllocatedBlock_t* ab, global char* memmory, char value)
{
         global char* currentPosition = (__global char*) memmory;
		
		currentPosition += ab->offset;
		int x ; 
		//--- Reset memmory
		for (x = 0 ; x <ab->availableMem ; x++ )
		{
		      (*currentPosition) = value;
			  currentPosition += 1;			   
			   
		}
}

void sft_preAlloc(int threadID, global sft_AllocatedBlock* preAllocated, global char* memmory, int memBlockSize)
{
        preAllocated[threadID].availableMem = memBlockSize;
		preAllocated[threadID].usedMem =0;
        //preAllocated[threadID].start = (global char*) memmory[threadID * memBlockSize];
		preAllocated[threadID].activePos = preAllocated[threadID].start;
		preAllocated[threadID].offset = threadID * memBlockSize ;

		struct sft_AllocatedBlock_t ab;
		ab.start = preAllocated[threadID].start;
		ab.activePos = preAllocated[threadID].activePos;
		ab.availableMem = preAllocated[threadID].availableMem;
		ab.usedMem = preAllocated[threadID].usedMem;
		ab.origSize = preAllocated[threadID].origSize;
		ab.offset = preAllocated[threadID].offset;

		sft_memSet(threadID , &ab,  memmory , 0);
}

global char* sft_malloc(int threadID, global sft_AllocatedBlock* preAllocated, global char* memmory, int reqMem , int *assigned )
{	

	if ((int)(preAllocated[threadID].availableMem)>(int)(reqMem))
	{		
		preAllocated[threadID].availableMem -= reqMem;
		preAllocated[threadID].usedMem +=reqMem;		
		
		global char* currentPosition = (__global char*) memmory;		
		currentPosition += preAllocated[threadID].offset;
		preAllocated[threadID].offset += reqMem;
		*assigned = 1;
		return currentPosition ;
	}	
	*assigned = 0;
	return 0;
}

global  void* sft_base(global char* memmory)
{
	return (global   void*)(memmory) ;
}

/*
  Kernels
  
*/

// *****************************************************************************
// Aloja espacio e inicializa la estructura
// *****************************************************************************
__kernel void preAllocation(  global sft_AllocatedBlock* preAllocated , global char* memmory, int memBlockSize)
{
    // get index into global data array
    uint x = get_global_id(0);    
	if (x < 128 )
		sft_preAlloc( x , preAllocated , memmory,memBlockSize ) ;
}
// *****************************************************************************
// Intentar ocupar los datos con valores de tipo Entero. Anda OK
// *****************************************************************************
__kernel void Sample1(  global sft_AllocatedBlock* preAllocated , global char* memmory, int memBlockSize)
{
      // get index into global data array
    uint thID = get_global_id(0);    	
	uint gID = get_local_id(0); 
	int x = 0;
	int wasAssigned ;

    global int* allocatedBlock ;
	
	allocatedBlock =(global int*)(sft_malloc( thID , preAllocated , memmory,50 * sizeof(int)  , &wasAssigned ));
	if ( !allocatedBlock) return ;
	for (x = 0 ; x < 50 ; x++ )
	   allocatedBlock[x] = 1000*thID+x;
	
    
}

// *****************************************************************************
// Intentar ocupar los datos con valores de tipo Struct. Anda OK
//*****************************************************************************
__kernel void Sample2(  global sft_AllocatedBlock* preAllocated , global char* memmory, int memBlockSize)
{
      // get index into global data array
    uint thID = get_global_id(0);    	
	uint gID = get_local_id(0); 
	int x = 0;
	int wasAssigned ;

    global sft_SampleStruct* allocatedBlock ;
	
	allocatedBlock =(global sft_SampleStruct*)(sft_malloc( thID , preAllocated , memmory,50 * sizeof(sft_SampleStruct), &wasAssigned  ) );
	if ( !allocatedBlock) return ;
	for (x = 0 ; x < 50 ; x++ )
	{
	   allocatedBlock[x].id = thID;
	   allocatedBlock[x].usedMem = x;
	}
	
    
}

// *****************************************************************************
//*****************************************************************************
__kernel void Sample3(global sft_AllocatedBlock* preAllocated , global char* memmory, int memBlockSize, int offset)
{
// get index into global data array
    uint thID = get_global_id(0);    

    global char *image2 = NULL;
    global opj_image_t* img ;
    global opj_image_comp_t* comps ;
	int wasAssigned =0 ;
	if (thID == 0)
	{
		//image2 = (global char* )(  sft_base(memmory)  );
		// Salto el espacio de la imagen
		image2 = sft_malloc( thID , preAllocated , memmory, sizeof(opj_image_t),&wasAssigned ) ;
		
        img = (global opj_image_t*)(image2);
		img->x0= sft_base(memmory) ;
		img->y0= 1;
		img->x1= 1;
		img->y1= 1;
		img->numcomps= 5;
		img->color_space = CLRSPC_SRGB ;
		wasAssigned = 0;
        comps = (global opj_image_comp_t*)(sft_malloc( thID , preAllocated , memmory, sizeof(opj_image_comp_t),&wasAssigned ) );
		if (wasAssigned)
		{
		  img->x1 = comps;
		  img->y1 = 3;
		  img->numcomps= 7;
		  img->icc_profile_buf = (global char*)(sft_malloc( thID , preAllocated , memmory, 14*sizeof(char),&wasAssigned ) );
		  img->icc_profile_len = 14;		  		  
		  img->offsetComps = (char*)comps -(char*)img;
		  
		  for (int i=0;i<14;i++)
		     img->icc_profile_buf[i] = 17;
			 
		  img->comps = comps; 
		  img->comps->dx = 1;
		  img->comps->dy = 3;
		  img->comps->w = 3;
		  img->comps->h = 3;
		  img->comps->x0 = 3;
		  img->comps->y0 = 3;
		  img->comps->prec = 3;	
          
		  
		}
		else
		   img->x1= 33;
	
	}
}