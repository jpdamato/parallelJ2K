/*
 * Copyright (c) 2005, Herve Drolon, FreeImage Team
 * Copyright (c) 2007, Callum Lerwick <seg@haxxed.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS `AS IS'
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef __opj_malloc_H
#define __opj_malloc_H
/**
@file sft_malloc.h
@brief Internal functions

The functions in sft_malloc.h are internal utilities used for memory management.
*/

/** @defgroup MISC MISC - Miscellaneous internal functions */
/*@{*/

/** @name Exported functions */
/*@{*/
/* ----------------------------------------------------------------------- */

/**
Allocate an uninitialized memory block
@param size Bytes to allocate
@return Returns a void pointer to the allocated space, or NULL if there is insufficient memory available
*/

/**
Allocate a memory block with elements initialized to 0
@param num Blocks to allocate
@param size Bytes per block to allocate
@return Returns a void pointer to the allocated space, or NULL if there is insufficient memory available
*/
#ifdef ALLOC_PERF_OPT
void * OPJ_CALLCONV opj_calloc(size_t _NumOfElements, size_t _SizeOfElements);
#else
#define opj_calloc(num, size) calloc(num, size)
#endif



/**
Allocate memory aligned to a 16 byte boundry
@param size Bytes to allocate
@return Returns a void pointer to the allocated space, or NULL if there is insufficient memory available
*/
/* FIXME: These should be set with cmake tests, but we're currently not requiring use of cmake */
#ifdef _WIN32
	/* Someone should tell the mingw people that their malloc.h ought to provide _mm_malloc() */
	#ifdef __GNUC__
		#include <mm_malloc.h>
		#define HAVE_MM_MALLOC
	#else /* MSVC, Intel C++ */
		#include <malloc.h>
		#ifdef _mm_malloc
			#define HAVE_MM_MALLOC
		#endif
	#endif   
#else /* Not _WIN32 */
	#if defined(__sun)
		#define HAVE_MEMALIGN
	/* Linux x86_64 and OSX always align allocations to 16 bytes */
	#elif !defined(__amd64__) && !defined(__APPLE__)	
		#define HAVE_MEMALIGN
		#include <malloc.h>			
	#endif
#endif

#ifdef __GNUC__
#pragma GCC poison malloc calloc realloc free
#endif

/* ----------------------------------------------------------------------- */
/*@}*/

/*@}*/

void* sft_malloc(size_t size) ;
void* sft_aligned_malloc(size_t size);
size_t sft_memoccupation();
void* sft_base();
void sft_memSet( void* memmory, char value,int  dataSize) ;
void sft_memCopy(void* dst , void* src, int len);
int sft_memSlots();
void* sft_calloc(size_t num , size_t size);
void sft_printResults(int from, int to);	
void sft_freepreAlloc();
void sft_enterSection(char* section);
void sft_leaveSection();
void sft_printSections();
void sft_printAvailableMem();
void sft_preAlloc(size_t size, size_t numSections, size_t extraSlot0Mem);
void sft_free(void* m);
void sft_resetAllocated();
#endif /* __opj_malloc_H */

