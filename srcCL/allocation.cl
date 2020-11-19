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

global char* sft_malloc(int threadID, global sft_AllocatedBlock* preAllocated, global char* memmory, int reqMem )
{	

	if ((int)(preAllocated[threadID].availableMem)>(int)(reqMem))
	{		
		preAllocated[threadID].availableMem -= reqMem;
		preAllocated[threadID].usedMem +=reqMem;
		
		
		global char* currentPosition = (__global char*) memmory;		
		currentPosition += preAllocated[threadID].offset;
		preAllocated[threadID].offset += reqMem;
		
		return currentPosition ;
	}	
	
	return -1;
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

    global int* allocatedBlock ;
	
	allocatedBlock =(global int*)(sft_malloc( thID , preAllocated , memmory,50 * sizeof(int) ) );
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

    global sft_SampleStruct* allocatedBlock ;
	
	allocatedBlock =(global sft_SampleStruct*)(sft_malloc( thID , preAllocated , memmory,50 * sizeof(sft_SampleStruct) ) );
	if ( !allocatedBlock) return ;
	for (x = 0 ; x < 50 ; x++ )
	{
	   allocatedBlock[x].id = thID;
	   allocatedBlock[x].usedMem = x;
	}
	
    
}