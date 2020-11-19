/*
 * Copyright (c) 2002-2007, Communications and Remote Sensing Laboratory, Universite catholique de Louvain (UCL), Belgium
 * Copyright (c) 2002-2007, Professor Benoit Macq
 * Copyright (c) 2001-2003, David Janssens
 * Copyright (c) 2002-2003, Yannick Verschueren
 * Copyright (c) 2003-2007, Francois-Olivier Devaux and Antonin Descampe
 * Copyright (c) 2005, Herve Drolon, FreeImage Team
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

#ifdef __SSE__
#include <xmmintrin.h>
#endif

#include "mct.h"

/* Are restricted pointers available? (C99) */
#if (__STDC_VERSION__ != 199901L)
	/* Not a C99 compiler */
	#ifdef __GNUC__
		#define restrict __restrict__
	#else
		#define restrict /* restrict */
	#endif
#endif

/* <summary> */
/* This table contains the norms of the basis function of the reversible MCT. */
/* </summary> */
static const double mct_norms[3] = { 1.732, .8292, .8292 };

/* <summary> */
/* This table contains the norms of the basis function of the irreversible MCT. */
/* </summary> */
static const double mct_norms_real[3] = { 1.732, 1.805, 1.573 };

/* <summary> */
/* Foward reversible MCT. */
/* </summary> */
void mct_encode(
		int* restrict c0,
		int* restrict c1,
		int* restrict c2,
		int n)
{
	int i;

	for(i = 0; i < n; ++i) {
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

/* <summary> */
/* Inverse reversible MCT. */
/* </summary> */
void mct_decode(
		int* restrict c0,
		int* restrict c1, 
		int* restrict c2, 
		int n)
{
	int i;
	for (i = 0; i < n; ++i) {
		int y = c0[i];
		int u = c1[i];
		int v = c2[i];
		int g = y - ((u + v) >> 2);
		int r = v + g;
		int b = u + g;
		c0[i] = r;
		c1[i] = g;
		c2[i] = b;
	}
}

/* <summary> */
/* Get norm of basis function of reversible MCT. */
/* </summary> */
double mct_getnorm(int compno) {
	return mct_norms[compno];
}

