#include "opj_malloc.h"
#include <stdio.h>
#include <memory.h>
#include <stdio.h>
#include <omp.h>


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