#include "c_J2Kencoder.h"
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <memory.h>

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
/*
   SFT MEM ALLOC
*/
//#define MEM_LOGGING
#define USING_OPENMP

typedef struct sft_Sections 
{
	char sectionName[20] ; 
	int startIndex , endIndex;
	int mem_used;
}sft_Sections_t;

typedef struct sft_AllocatedBlock_t
{
	char* start;
	char* activePos;
	int availableMem;
	int usedMem ;
	int origSize;
	
} sft_AllocatedBlock;


size_t* allocations = NULL;
sft_Sections_t* sections = NULL;
//char* preAllocated = NULL;
//char* originalPrealocated = NULL;

sft_AllocatedBlock* preAllocated = NULL;
char* globalMem;
int numBlocks = 0;
//int availableMem = 0;
int allocatedIndex = 0;
int sectionIndex = 0;
int extraMemAllocation = 0;

void* sft_base()
{
   return (void*)(globalMem);
}

void sft_preAlloc(size_t size, size_t numSections, size_t extraSlot0Mem)
{
	size_t i = 0;
	int j;
	int extraMem = 0;
	int totalMem = (numSections *size +  extraSlot0Mem) ;
	preAllocated = (sft_AllocatedBlock*)malloc(sizeof(sft_AllocatedBlock) * numSections);
	numBlocks = numSections;

	globalMem = (char*)malloc(totalMem * sizeof(char));

	for (i=0; i<numSections ; i++)
	{
		// Alojo el doble para el primer Bloque
		if (i == 0)
		{
			preAllocated[i].activePos = globalMem ; 
			preAllocated[i].origSize = size+ extraSlot0Mem;		
			preAllocated[i].availableMem = size+ extraSlot0Mem;		
		}
		else
		{
			preAllocated[i].activePos = globalMem ; 
			preAllocated[i].activePos += i*size + extraSlot0Mem;
			preAllocated[i].origSize = size;		
		}
				
		preAllocated[i].availableMem = preAllocated[i].origSize;
		preAllocated[i].usedMem = 0;
		preAllocated[i].start  = preAllocated[i].activePos;	

		memset(preAllocated[i].start , 0 ,preAllocated[i].availableMem );

		for (j = 0 ; j<preAllocated[i].availableMem  ; j++)
		 if (globalMem[j+ preAllocated[i].activePos - globalMem ] != 0)
		 {
			 fprintf(stderr, "Error");
		 }
	}

	
}

void sft_resetAllocated()
{
	int i = 0;
	int extraMem = 0;
	
	for (i=0; i<numBlocks ; i++)
	{
		// Alojo el doble para el primer Bloque
		preAllocated[i].activePos = preAllocated[i].start ;
		preAllocated[i].availableMem = preAllocated[i].origSize;		
		preAllocated[i].usedMem = 0;				
		memset(preAllocated[i].start , 0 ,(preAllocated[i].origSize)*sizeof(char));
	}
}

void sft_memSet( void* memmory, char value,int  dataSize)
{
	memset(memmory,value,dataSize);   	
}

void sft_memCopy(void* dst , void* src, int len)
{
	memcpy(dst, src, len);
}

void sft_freepreAlloc()
{
   if (preAllocated)
   {
	   free(preAllocated[0].start);
       preAllocated[0].availableMem = 0;
	   free(preAllocated);
   }
}

void sft_free(void* m)
{
	if (preAllocated)
	{
		// Nothing to DO		
		return;
	}
	else
		return ;
	  //free(m);
}



void* sft_malloc(size_t size)
{
	void *res = NULL;

	int threadID = 0;
	#if defined USING_OPENMP
	threadID = omp_get_thread_num();
	#endif

#if defined MEM_LOGGING
	if (sections == NULL)
		sections = (sft_Sections_t*) malloc( 1500 * sizeof(sft_Sections_t));

	if (allocations == NULL)
	   allocations = (size_t*) malloc(90000 * sizeof(size_t)); 
#endif
	if ((preAllocated) && (int)(preAllocated[threadID].availableMem)>(int)(size))
	{		
		preAllocated[threadID].availableMem -= size;
		preAllocated[threadID].usedMem +=size;
		#if defined MEM_LOGGING
		allocations[allocatedIndex] = size+2;	
		allocatedIndex ++;
		#endif
		res = preAllocated[threadID].activePos;
		preAllocated[threadID].activePos = (char*)(preAllocated[threadID].activePos) + size;
		//(char*)(preAllocated)++ ; // += size;
		return res ;
	}
	else
	{			
		#if defined MEM_LOGGING
		allocations[allocatedIndex] = size;	
		allocatedIndex ++;
		#endif
		extraMemAllocation += size;
		return malloc(size);
	}
}

void* sft_calloc(size_t num , size_t size)
{
	int i = 0;
	char* res ;
	int threadID = 0;
	#if defined USING_OPENMP
	threadID = omp_get_thread_num();
	#endif

	#if defined MEM_LOGGING
	if (sections == NULL)
		sections = (sft_Sections_t*) malloc( 1500 * sizeof(sft_Sections_t));
	if (allocations == NULL)
	   allocations = (size_t*) malloc(90000 * sizeof(size_t)); 
	
	#endif
		
	if ((preAllocated) && (int)(preAllocated[threadID].availableMem)>(int)(num*size))
	{
		preAllocated[threadID].availableMem -= num*size;
		preAllocated[threadID].usedMem +=num*size;
		#if defined MEM_LOGGING
		allocations[allocatedIndex] = num*size+2;	
		allocatedIndex ++;
		#endif
		res = preAllocated[threadID].activePos;		
		//memset(res,0,num*size * sizeof(char));		
		preAllocated[threadID].activePos = (char*)(preAllocated[threadID].activePos) + num*size;
		return res;
		//(char*)(preAllocated)++ ; // += size;
	}
	else
	{	
		#if defined MEM_LOGGING
		allocations[allocatedIndex] = num*size;	
		allocatedIndex ++;
		#endif
	    extraMemAllocation += num*size;
		return calloc(num, size);
	}
	/*
	
	*/
	
	//return malloc(num * size);
}

void* sft_aligned_malloc(size_t size)
{
	void* res = NULL;	
		
	int threadID = 0;
	#if defined USING_OPENMP
	threadID = omp_get_thread_num();
	#endif

	#if defined MEM_LOGGING
	if (allocations == NULL)
	   allocations = (size_t*) malloc(90000 * sizeof(size_t)); 
	#endif
		
	if ((preAllocated) && (int)(preAllocated[threadID].availableMem)>(int)(size))
	{
		preAllocated[threadID].availableMem -= size;
		preAllocated[threadID].usedMem +=size;
		#if defined MEM_LOGGING
		allocations[allocatedIndex] = size+2;	
		allocatedIndex ++;
		#endif
		res = preAllocated[threadID].activePos;
		preAllocated[threadID].activePos = preAllocated[threadID].activePos + size;
		//(char*)(preAllocated)++ ; // += size;
		return res ;
	}
	else
	{			
		#if defined MEM_LOGGING
		allocations[allocatedIndex] = size;	
		allocatedIndex ++;
		#endif
		extraMemAllocation += size;
		return malloc(size);
	}
}

int sft_memSlots()
{
	return allocatedIndex;
}

void sft_printResults(int from, int to)
{
	#if defined MEM_LOGGING
	int  i ,j ,lastJ ;
	//printf( "failed to open %s for writing\n", parameters.outfile);
	fprintf(stderr, "---------------------------------------------\n");
	j = 0;
	i = from;
	while (i<to) 
	{
		if (i>=allocatedIndex) break;
		while (sections[j].startIndex<i) j++; 		
		fprintf(stderr, "----------------------------------------------------");
		fprintf(stderr, "Enter section [%s]\n" , sections[j].sectionName);
		while (i<sections[j].endIndex)
		{
		  fprintf(stderr, "Used index [%d] size  [%d]\n", i , allocations[i]);
		  if ((i>0) && (i%200 == 0))
			getchar();
		  i++;
		}
	}
	#endif
}
void sft_leaveSection()
{
	#if defined MEM_LOGGING
	int i ;
	sections[sectionIndex].endIndex = allocatedIndex-1;
	sections[sectionIndex].mem_used = 0;
	for (i = sections[sectionIndex].startIndex ; i< allocatedIndex ; i++)
		sections[sectionIndex].mem_used += allocations[i];

	sectionIndex++;
	#endif
}
void sft_enterSection(char* section)
{
	#if defined MEM_LOGGING
	if (sections == NULL)
		sections = (sft_Sections_t*) malloc( 1500 * sizeof(sft_Sections_t));

	strcpy (sections[sectionIndex].sectionName,section);
	 
	sections[sectionIndex].startIndex = allocatedIndex;
	#endif
}

void sft_printAvailableMem()
{
	if (preAllocated)
	{
		int i = 0;
		int total =0;
		fprintf(stderr, " ------------ Memmory Allocation Size --------------\n");	
		for (i = 0 ; i<numBlocks ; i++)
		{
			total +=preAllocated[i].availableMem;
			fprintf(stderr, "Thread %d KB mem. Allocated  [%d] .Used : [%d]. Available : [%d] \n",i,preAllocated[i].origSize/1024,preAllocated[i].usedMem/1024,preAllocated[i].availableMem/1024);	

		}
		fprintf(stderr, "Extra memmory used [%d] in KBytes \n",extraMemAllocation/1024);	
		fprintf(stderr, "Available allocated mem to [%d] in KBytes \n",total/1024);	
	}
}
void sft_printSections()
{
	
	//printf( "failed to open %s for writing\n", parameters.outfile);
	#if defined MEM_LOGGING
	int i ;
	fprintf(stderr, "---------------------------------------------\n");	
	for (i = 0 ; i<sectionIndex ; i++)
		fprintf(stderr, "Sections [%s] mem [%d ]index from  [%d] to [%d] \n",sections[i].sectionName,sections[i].mem_used , sections[i].startIndex,sections[i].endIndex);
	
	if (preAllocated)
	{
		int i = 0;
		int total =0;
		for (i = 0 ; i<numBlocks ; i++)
			total +=preAllocated[i].availableMem;
		
		fprintf(stderr, "Available allocated mem to [%d] in Bytes \n",total);	
	}
	#endif
}

size_t sft_memoccupation()
{
	size_t total = 0 ;
	#if defined MEM_LOGGING
	int i ;
	for ( i=0; i<allocatedIndex ; i++)
	{
		total += allocations[i]; 
	}
	#endif
	return total ;	
}

ij2k_prog_order_t ij2k_prog_order_list[] = {
	{CPRL, "CPRL"},
	{LRCP, "LRCP"},
	{PCRL, "PCRL"},
	{RLCP, "RLCP"},
	{RPCL, "RPCL"},
	{(OPJ_PROG_ORDER)-1, ""}
};


/* <summary> */
/* This array defines all the possible states for a context. */
/* </summary> */

/* This file was automatically generated by t1_generate_luts.c */
#if defined GPU
constant 
#endif
	char lut_ctxno_sc[256] = {
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

#if defined GPU
constant 
#endif 
char lut_spb[256] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 
  0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 
  0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
  0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 
  0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};
#if defined GPU
constant 
#endif  
char lut_ctxno_zc[1024] = {
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


#if defined GPU
constant 
#endif  
short lut_nmsedec_sig[1 << T1_NMSEDEC_BITS] = {
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

#if defined GPU
constant 
#endif  

short lut_nmsedec_sig0[1 << T1_NMSEDEC_BITS] = {
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

#if defined GPU
constant 
#endif  
short lut_nmsedec_ref[1 << T1_NMSEDEC_BITS] = {
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

#if defined GPU
constant 
#endif  
	short lut_nmsedec_ref0[1 << T1_NMSEDEC_BITS] = {
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
#if defined GPU
constant 
#endif
	double dwt_norms[4][10] = {
	{1.000, 1.500, 2.750, 5.375, 10.68, 21.34, 42.67, 85.33, 170.7, 341.3},
	{1.038, 1.592, 2.919, 5.703, 11.33, 22.64, 45.25, 90.48, 180.9},
	{1.038, 1.592, 2.919, 5.703, 11.33, 22.64, 45.25, 90.48, 180.9},
	{.7186, .9218, 1.586, 3.043, 6.019, 12.01, 24.00, 47.97, 95.93}
};

/* <summary> */
/* This table contains the norms of the basis function of the reversible MCT. */
/* </summary> */
#if defined GPU
constant 
#endif
	double mct_norms[3] = { 1.732, .8292, .8292 };


#define mqc_setcurctx(mqc, ctxno)	(mqc)->curctx = &(mqc)->ctxs[(int)(ctxno)]

/* <summary> */
/* This array defines all the possible states for a context. */
/* </summary> */
static opj_mqc_stateI_t mqc_statesI[47 * 2] = {
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
  PI
*/


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
/*
  MQC
*/

void mqc_resetstates(opj_mqc_t *mqc) {
	int i;
	for (i = 0; i < MQC_NUMCTXS; i++) {
		mqc->ctxs[i] = mqc_statesI;
	}
}

void mqc_setstate(opj_mqc_t *mqc, int ctxno, int msb, int prob) {
	mqc->ctxs[ctxno] = &mqc_statesI[msb + (prob << 1)];
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
  Event MGR
*/
opj_event_mgr_t* opj_set_event_mgr(opj_common_ptr cinfo, opj_event_mgr_t *event_mgr, void *context) 
{
	if(cinfo) {
		opj_event_mgr_t *previous = cinfo->event_mgr;
		cinfo->event_mgr = event_mgr;
		cinfo->client_data = context;
		return previous;
	}

	return NULL;
}

opj_bool opj_event_msg(opj_common_ptr cinfo, int event_type, const char *fmt, ...) 
{
	return OPJ_FALSE; 
}
/* 
==========================================================
   local functions
==========================================================
*/
/**
Write a bit
@param bio BIO handle
@param b Bit to write (0 or 1)
*/
int bio_byteout(opj_bio_t *bio) 
{
	bio->buf = (bio->buf << 8) & 0xffff;
	bio->ct = bio->buf == 0xff00 ? 7 : 8;
	if (bio->bp >= bio->end) 
	{
		return 1;
	}
	*bio->bp++ = bio->buf >> 8;
	return 0;
}
/**
Read a bit
@param bio BIO handle
@return Returns the read bit
*/

int bio_bytein(opj_bio_t *bio) 
{
	bio->buf = (bio->buf << 8) & 0xffff;
	bio->ct = bio->buf == 0xff00 ? 7 : 8;
	if (bio->bp >= bio->end) 
	{
		return 1;
	}
	bio->buf |= *bio->bp++;
	return 0;
}

/**
Write a byte
@param bio BIO handle
@return Returns 0 if successful, returns 1 otherwise
*/

void bio_putbit(opj_bio_t *bio, int b) 
{
	if (bio->ct == 0) 
	{
		bio_byteout(bio);
	}
	bio->ct--;
	bio->buf |= b << bio->ct;
}

/**
Read a byte
@param bio BIO handle
@return Returns 0 if successful, returns 1 otherwise
*/


int bio_getbit(opj_bio_t *bio) 
{
	if (bio->ct == 0) 
	{
		bio_bytein(bio);
	}
	bio->ct--;
	return (bio->buf >> bio->ct) & 1;
}

/* 
==========================================================
   Bit Input/Output interface
==========================================================
*/

opj_bio_t* bio_create(void) 
{
	opj_bio_t *bio = (opj_bio_t*)sft_malloc(sizeof(opj_bio_t));
	return bio;
}

void bio_destroy(opj_bio_t *bio) 
{
	if(bio) {
		sft_free(bio);
	}
}

int bio_numbytes(opj_bio_t *bio) 
{
	return (bio->bp - bio->start);
}

void bio_init_enc(opj_bio_t *bio, unsigned char *bp, int len) 
{
	bio->start = bp;
	bio->end = bp + len;
	bio->bp = bp;
	bio->buf = 0;
	bio->ct = 8;
}

void bio_init_dec(opj_bio_t *bio, unsigned char *bp, int len) 
{
	bio->start = bp;
	bio->end = bp + len;
	bio->bp = bp;
	bio->buf = 0;
	bio->ct = 0;
}

void bio_write(opj_bio_t *bio, int v, int n) 
{
	int i;
	for (i = n - 1; i >= 0; i--) 
	{
		bio_putbit(bio, (v >> i) & 1);
	}
}

int bio_read(opj_bio_t *bio, int n) 
{
	int i, v;
	v = 0;
	for (i = n - 1; i >= 0; i--) 
	{
		v += bio_getbit(bio) << i;
	}
	return v;
}

int bio_flush(opj_bio_t *bio) 
{
	bio->ct = 0;
	if (bio_byteout(bio)) 
	{
		return 1;
	}
	if (bio->ct == 7) 
	{
		bio->ct = 0;
		if (bio_byteout(bio)) 
		{
			return 1;
		}
	}
	return 0;
}

int bio_inalign(opj_bio_t *bio) 
{
	bio->ct = 0;
	if ((bio->buf & 0xff) == 0xff) 
	{
		if (bio_bytein(bio)) 
		{
			return 1;
		}
		bio->ct = 0;
	}
	return 0;
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

/* <summary>                             */  
/* Forward lazy transform (horizontal & vertical).    */
/* </summary>                            */ 

void idwt_deinterleave_h(int *a, int *b, int dn, int sn, int cas) 
{
	int i;
	for (i=0; i<sn; i++) b[i]=a[2*i+cas];
	for (i=0; i<dn; i++) b[sn+i]=a[(2*i+1-cas)];
}

void idwt_deinterleave_v(int *a, int *b, int dn, int sn, int x, int cas) 
{
	int i;
	for (i=0; i<sn; i++) b[i*x]=a[2*i+cas];
	for (i=0; i<dn; i++) b[(sn+i)*x]=a[(2*i+1-cas)];
}

/* <summary>                            */
/* Forward 5-3 wavelet transform in 2-D. */
/* </summary>   
*/
void idwt_encode_1(int *a, int dn, int sn, int cas) 
{
	int i;

	if (!cas) {
		if ((dn > 0) || (sn > 1)) {	/* NEW :  CASE ONE ELEMENT */
			for (i = 0; i < dn; i++) D(i) -= (S_(i) + S_(i + 1)) >> 1;
			for (i = 0; i < sn; i++) S(i) += (D_(i - 1) + D_(i) + 2) >> 2;
		}
	} else {
		if (!sn && dn == 1)		    /* NEW :  CASE ONE ELEMENT */
			S(0) *= 2;
		else {
			for (i = 0; i < dn; i++) S(i) -= (DD_(i) + DD_(i - 1)) >> 1;
			for (i = 0; i < sn; i++) D(i) += (SS_(i) + SS_(i + 1) + 2) >> 2;
		}
	}
}

void idwt_encode(opj_tcd_tilecomp_t * tilec) 
{
	int i, j, k;
	cpixelType *a = NULL;
	int *aj = NULL;
	int *bj = NULL;
	int w, l;

	w = tilec->x1-tilec->x0;
	l = tilec->numresolutions-1;
	a = tilec->data;

	for (i = 0; i < l; i++) 
	{
		int rw;			/* width of the resolution level computed                                                           */
		int rh;			/* height of the resolution level computed                                                          */
		int rw1;		/* width of the resolution level once lower than computed one                                       */
		int rh1;		/* height of the resolution level once lower than computed one                                      */
		int cas_col;	/* 0 = non inversion on horizontal filtering 1 = inversion between low-pass and high-pass filtering */
		int cas_row;	/* 0 = non inversion on vertical filtering 1 = inversion between low-pass and high-pass filtering   */
		int dn, sn;

		rw = tilec->resolutions[l - i].x1 - tilec->resolutions[l - i].x0;
		rh = tilec->resolutions[l - i].y1 - tilec->resolutions[l - i].y0;
		rw1= tilec->resolutions[l - i - 1].x1 - tilec->resolutions[l - i - 1].x0;
		rh1= tilec->resolutions[l - i - 1].y1 - tilec->resolutions[l - i - 1].y0;

		cas_row = tilec->resolutions[l - i].x0 % 2;
		cas_col = tilec->resolutions[l - i].y0 % 2;

		sn = rh1;
		dn = rh - rh1;
		bj = (int*)sft_malloc(rh * sizeof(int));
		for (j = 0; j < rw; j++) 
		{
			aj = a + j;
			for (k = 0; k < rh; k++)  bj[k] = aj[k*w];
			idwt_encode_1(bj, dn, sn, cas_col);
			idwt_deinterleave_v(bj, aj, dn, sn, w, cas_col);
		}
		sft_free(bj);

		sn = rw1;
		dn = rw - rw1;
		bj = (int*)sft_malloc(rw * sizeof(int));
		for (j = 0; j < rh; j++) 
		{
			aj = a + j * w;
			for (k = 0; k < rw; k++)  bj[k] = aj[k];
			idwt_encode_1(bj, dn, sn, cas_row);
			idwt_deinterleave_h(bj, aj, dn, sn, cas_row);
		}
		sft_free(bj);
	}
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
		*mqc->curctx =  &mqc_statesI[(*mqc->curctx)->nmps];
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
	*mqc->curctx =  &mqc_statesI[ (*mqc->curctx)->nlps ];
	mqc_renorme(mqc);
}

void mqc_bypass_init_enc(opj_mqc_t *mqc) {
	mqc->c = 0;
	mqc->ct = 8;
	/*if (*mqc->bp == 0xff) {
	mqc->ct = 7;
     } */
}


void mqc_encode(opj_mqc_t *mqc, int d)
{
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


void itgt_setvalue(iopj_tgt_tree_t *tree, int leafno, int value) 
{
	iopj_tgt_node_t *node;
	node = &tree->nodes[leafno];
	while (node && node->value > value) {
		node->value = value;
		node = node->parent;
	}
}

void itgt_encode(opj_bio_t *bio, opj_tgt_tree_t *tree, int leafno, int threshold) 
{
	opj_tgt_node_t *stk[31];
	opj_tgt_node_t **stkptr;
	opj_tgt_node_t *node;
	int low;

	stkptr = stk;
	node = &tree->nodes[leafno];
	while (node->parent) 
	{
		*stkptr++ = node;
		node = node->parent;
	}

	low = 0;
	for (;;) 
	{
		if (low > node->low) 
		{
			node->low = low;
		} 
		else 
		{
			low = node->low;
		}

		while (low < threshold) 
		{
			if (low >= node->value) 
			{
				if (!node->known) 
{
					bio_write(bio, 1, 1);
					node->known = 1;
				}
				break;
			}
			bio_write(bio, 0, 1);
			++low;
		}

		node->low = low;
		if (stkptr == stk)
			break;
		node = *--stkptr;
	}
}
/*
PI


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

opj_bool pi_next(iopj_pi_iterator_t * pi)
{
	return pi_next_lrcp(pi);
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

void it2_putcommacode(opj_bio_t *bio, int n) 
{
	while (--n >= 0) {
		bio_write(bio, 1, 1);
	}
	bio_write(bio, 0, 1);
}

void it2_putnumpasses(opj_bio_t *bio, int n) {
	if (n == 1) {
		bio_write(bio, 0, 1);
	} else if (n == 2) {
		bio_write(bio, 2, 2);
	} else if (n <= 5) {
		bio_write(bio, 0xc | (n - 3), 4);
	} else if (n <= 36) {
		bio_write(bio, 0x1e0 | (n - 6), 9);
	} else if (n <= 164) {
		bio_write(bio, 0xff80 | (n - 37), 16);
	}
}


int it2_encode_packet(opj_tcd_tile_t * tile, opj_tcp_t * tcp, iopj_pi_iterator_t *pi, unsigned char *dest, int length, opj_codestream_info_t *cstr_info, int tileno) 
{
	int bandno, cblkno;
	unsigned char *c = dest;

	int compno = pi->compno;	/* component value */
	int resno  = pi->resno;		/* resolution level value */
	int precno = pi->precno;	/* precinct value */
	int layno  = pi->layno;		/* quality layer value */

	opj_tcd_tilecomp_t *tilec = &tile->comps[compno];
	opj_tcd_resolution_t *res = &tilec->resolutions[resno];

	opj_bio_t *bio = NULL;	/* BIO component */

	/* <SOP 0xff91> */
	if (tcp->csty & J2K_CP_CSTY_SOP) 
	{
		c[0] = 255;
		c[1] = 145;
		c[2] = 0;
		c[3] = 4;
		c[4] = (unsigned char)((tile->packno % 65536) / 256);
		c[5] = (unsigned char)((tile->packno % 65536) % 256);
		c += 6;
	}
	/* </SOP> */

	if (!layno) 
	{
		for (bandno = 0; bandno < res->numbands; bandno++) 
		{
			opj_tcd_band_t *band = &res->bands[bandno];
			opj_tcd_precinct_t *prc = &band->precincts[precno];
			itgt_reset(prc->incltree);
			itgt_reset(prc->imsbtree);
			for (cblkno = 0; cblkno < prc->cw * prc->ch; cblkno++) 
			{
				opj_tcd_cblk_enc_t* cblk = &prc->cblks.enc[cblkno];
				cblk->numpasses = 0;
				itgt_setvalue((iopj_tgt_tree_t*)(prc->imsbtree), cblkno, band->numbps - cblk->numbps);
			}
		}
	}

	bio = bio_create();
	bio_init_enc(bio, c, length);
	bio_write(bio, 1, 1);		/* Empty header bit */

	/* Writing Packet header */
	for (bandno = 0; bandno < res->numbands; bandno++) 
	{
		opj_tcd_band_t *band = &res->bands[bandno];
		opj_tcd_precinct_t *prc = &band->precincts[precno];
		for (cblkno = 0; cblkno < prc->cw * prc->ch; cblkno++) 
		{
			opj_tcd_cblk_enc_t* cblk = &prc->cblks.enc[cblkno];
			opj_tcd_layer_t *layer = &cblk->layers[layno];
			if (!cblk->numpasses && layer->numpasses) 
			{
				itgt_setvalue((iopj_tgt_tree_t*)(prc->incltree), cblkno, layno);
			}
		}
		for (cblkno = 0; cblkno < prc->cw * prc->ch; cblkno++) 
		{
			opj_tcd_cblk_enc_t* cblk = &prc->cblks.enc[cblkno];
			opj_tcd_layer_t *layer = &cblk->layers[layno];
			int increment = 0;
			int nump = 0;
			int len = 0, passno;
			/* cblk inclusion bits */
			if (!cblk->numpasses) 
			{
				itgt_encode(bio, prc->incltree, cblkno, layno + 1);
			} 
			else 
			{
				bio_write(bio, layer->numpasses != 0, 1);
			}
			/* if cblk not included, go to the next cblk  */
			if (!layer->numpasses) 
			{
				continue;
			}
			/* if first instance of cblk --> zero bit-planes information */
			if (!cblk->numpasses) 
			{
				cblk->numlenbits = 3;
				itgt_encode(bio, prc->imsbtree, cblkno, 999);
			}
			/* number of coding passes included */
			it2_putnumpasses(bio, layer->numpasses);

			/* computation of the increase of the length indicator and insertion in the header     */
			for (passno = cblk->numpasses; passno < cblk->numpasses + layer->numpasses; passno++) 
			{
				opj_tcd_pass_t *pass = &cblk->passes[passno];
				nump++;
				len += pass->len;
				if (pass->term || passno == (cblk->numpasses + layer->numpasses) - 1) 
				{
					increment = int_max(increment, int_floorlog2(len) + 1 - (cblk->numlenbits + int_floorlog2(nump)));
					len = 0;
					nump = 0;
				}
			}
			it2_putcommacode(bio, increment);

			/* computation of the new Length indicator */
			cblk->numlenbits += increment;

			/* insertion of the codeword segment length */
			for (passno = cblk->numpasses; passno < cblk->numpasses + layer->numpasses; passno++)
			{
				opj_tcd_pass_t *pass = &cblk->passes[passno];
				nump++;
				len += pass->len;
				if (pass->term || passno == (cblk->numpasses + layer->numpasses) - 1) 
				{
					bio_write(bio, len, cblk->numlenbits + int_floorlog2(nump));
					len = 0;
					nump = 0;
				}
			}
		}
	}

	if (bio_flush(bio)) 
	{
		bio_destroy(bio);
		return -999;		/* modified to eliminate longjmp !! */
	}

	c += bio_numbytes(bio);
	bio_destroy(bio);

	/* <EPH 0xff92> */
	if (tcp->csty & J2K_CP_CSTY_EPH) 
	{
		c[0] = 255;
		c[1] = 146;
		c += 2;
	}
	/* </EPH> */

	/* << INDEX */
	/* End of packet header position. Currently only represents the distance to start of packet
	// Will be updated later by incrementing with packet start value */
	if(cstr_info && cstr_info->index_write) 
	{
		opj_packet_info_t *info_PK = &cstr_info->tile[tileno].packet[cstr_info->packno];
		info_PK->end_ph_pos = (int)(c - dest);
	}
	/* INDEX >> */

	/* Writing the packet body */

	for (bandno = 0; bandno < res->numbands; bandno++) 
	{
		opj_tcd_band_t *band = &res->bands[bandno];
		opj_tcd_precinct_t *prc = &band->precincts[precno];
		for (cblkno = 0; cblkno < prc->cw * prc->ch; cblkno++) 
		{
			opj_tcd_cblk_enc_t* cblk = &prc->cblks.enc[cblkno];
			opj_tcd_layer_t *layer = &cblk->layers[layno];
			if (!layer->numpasses) 
			{
				continue;
			}
			if (c + layer->len > dest + length) 
			{
				return -999;
			}
			
			sft_memCopy(c, layer->data, layer->len);
			cblk->numpasses += layer->numpasses;
			c += layer->len;
			/* << INDEX */ 			
			/* INDEX >> */
		}
	}

	return (c - dest);
}

int it2_encode_packets(iopj_t2_t* t2,int tileno, opj_tcd_tile_t *tile, int maxlayers, unsigned char *dest, int len, opj_codestream_info_t *cstr_info,int tpnum, int tppos,int pino, J2K_T2_MODE t2_mode, int cur_totnum_tp)
{
	unsigned char *c = dest;
	int e = 0;

	iopj_pi_iterator_t *pi = NULL;

	opj_image_t *image = t2->image;
	opj_cp_t *cp = t2->cp;
	opj_tcp_t *tcp = &cp->tcps[tileno];
	int pocno = cp->cinema == CINEMA4K_24? 2: 1;
	int maxcomp = cp->max_comp_size > 0 ? image->numcomps : 1;

	pi = pi_initialise_encode(image, cp, tileno, t2_mode);
	if(!pi) {
		/* TODO: throw an error */
		return -999;
	}

	if(t2_mode == THRESH_CALC )
	{ /* Calculating threshold */		
	}
	else{  /* t2_mode == FINAL_PASS  */
		pi_create_encode(pi, cp,tileno,pino,tpnum,tppos,t2_mode,cur_totnum_tp);
		// JUAN : Only LRCP encoding
		while (pi_next(&pi[pino])) 		
		{
			if (pi[pino].layno < maxlayers) 
			{
				e = it2_encode_packet(tile, &cp->tcps[tileno], &pi[pino], c, dest + len - c, cstr_info, tileno);
				if (e == -999) {
					break;
				} else {
					c += e;
				}
				/* INDEX >> */
				if(cstr_info) {					
				}
				/* << INDEX */
				tile->packno++;
			}
		}
	}

	pi_destroy(pi, cp, tileno);

	if (e == -999) {
		return e;
	}

	return (c - dest);
}

int itcd_encode_tile(const opj_j2k_t *j2k ,  const opj_tcd_t *tcd,const int tileno,unsigned char *dest,const int len, opj_codestream_info_t *cstr_info) 
{
	int compno;
	int l = 0,  numpacks = 0;
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
	l = it2_encode_packets(t2,tileno, tile, tcd_tcp->numlayers, dest, len, cstr_info,
		tcd->tp_num,tcd->tp_pos,tcd->cur_pino,FINAL_PASS,
		j2k->cur_totnum_tp[tileno]);
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

char *ij2k_convert_progression_order(OPJ_PROG_ORDER prg_order)
{
	ij2k_prog_order_t *po;
	for(po = ij2k_prog_order_list; po->enum_prog != -1; po++ ){
		if(po->enum_prog == prg_order){
			break;
		}
	}
	return po->str_prog;
}


int ij2k_get_num_tp(opj_cp_t *cp,int pino,int tileno)
{
	char *prog;
	int i;
	int tpnum=1,tpend=0;
	opj_tcp_t *tcp = &cp->tcps[tileno];
	prog = ij2k_convert_progression_order(tcp->prg);
	
	if(cp->tp_on == 1)
	{
		for(i=0;i<4;i++)
		{
			if(tpend!=1)
			{
				if( cp->tp_flag == prog[i] )
				{
					tpend=1;
					cp->tp_pos=i;
				}
				switch(prog[i])
				{
				case 'C':
					tpnum= tpnum * tcp->pocs[pino].compE;
					break;
				case 'R':
					tpnum= tpnum * tcp->pocs[pino].resE;
					break;
				case 'P':
					tpnum= tpnum * tcp->pocs[pino].prcE;
					break;
				case 'L':
					tpnum= tpnum * tcp->pocs[pino].layE;
					break;
				}
			}
		}
	}else{
		tpnum=1;
	}
	return tpnum;
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



void ij2k_encodeTile(opj_cp_t *cp  ,int tileno , opj_j2k_t *j2k , opj_tcd_t *gtcd,const opj_image_t *image )
{
	//opj_cp_t *cp = j2k->cp;		
	int pino;
	int tilepartno=0;
	// UniPG>> 
	int acc_pack_num = 0;
	int compno = 0;
	int curtileno = tileno;
	int cur_tp_num = 0;
	opj_tcd_t* tcd =NULL;

	// <<UniPG 
	opj_tcp_t *tcp = &cp->tcps[tileno];
	curtileno = tileno;

	// initialisation before tile encoding  	
	//tcd = iclone_TCD_tile(gtcd);
	tcd = gtcd;
	//Deprecated :tcd->cur_totnum_tp = j2k->cur_totnum_tp[tileno];
	itcd_init_encode(tcd, image, cp, tileno);


	// INDEX >> 
	for(pino = 0; pino <= tcp->numpocs; pino++) 
	{
		int tot_num_tp;
		tcd->cur_pino=pino;
		//Get number of tile parts
		tot_num_tp = ij2k_get_num_tp(cp,pino,tileno);
		tcd->tp_pos = cp->tp_pos;

		for(tilepartno = 0; tilepartno < tot_num_tp ; tilepartno++)
		{
			int tp_num = tilepartno;			
			unsigned char* bpos = NULL;
			//Encoding
			tcd->tp_num = tp_num ;
			tcd->cur_tp_num = cur_tp_num;
			tcd->tcd_tileno = tileno;
			tcd->tcd_tile = tcd->tcd_image->tiles;
			tcd->tcp = &tcd->cp->tcps[tileno];
			ij2k_encode_Tile(j2k->cp, j2k,tcd,tileno, tp_num,cur_tp_num);
			cur_tp_num++;
		}			
	}

}

void idwt_encode_stepsize(int stepsize, int numbps, opj_stepsize_t *bandno_stepsize) 
{
	int p, n;
	p = int_floorlog2(stepsize) - 13;
	n = 11 - int_floorlog2(stepsize);
	bandno_stepsize->mant = (n < 0 ? stepsize >> -n : stepsize << n) & 0x7ff;
	bandno_stepsize->expn = numbps - p;
}

void idwt_calc_explicit_stepsizes(opj_tccp_t * tccp, int prec) 
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
