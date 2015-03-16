/*
// Copyright 1997, 1998, 1999 University of Notre Dame.
// Authors: Andrew Lumsdaine, Jeremy G. Siek, Lie-Quan Lee
//
// This file is part of the Matrix Template Library
//
// You should have received a copy of the License Agreement for the
// Matrix Template Library along with the software;  see the
// file LICENSE.  If not, contact Office of Research, University of Notre
// Dame, Notre Dame, IN  46556.
//
// Permission to modify the code and to distribute modified code is
// granted, provided the text of this NOTICE is retained, a notice that
// the code was modified is included with the above COPYRIGHT NOTICE and
// with the COPYRIGHT NOTICE in the LICENSE file, and that the LICENSE
// file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
*/

#ifndef MTL_BLAS_H
#define MTL_BLAS_H

#include "mtl/mtl_config.h"

#if 0
#include "mtl/mtl_complex.h"
using std::complex;
#endif

/*--------------------------------------------------------
   Basic Linear Algebra Subprograms for C/C++
   Version 1.0
   Matthew E. Gaston
   May 6, 1998
----------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------------------------
    Level 1 BLAS
-----------------------------------------------------------*/


/*
//  Dot product functions
*/
float sdot_(int*, float*, int*, float*, int*);
double dsdot_(int*, float*, int*, float*, int*);
float sdsdot_(int*, float*, float*, int*, float*, int*);
double ddot_(int*, double*, int*, double*, int*);

/*
    AXPY
*/
void saxpy_(int*, float*, float*, int*, float*, int*);
void daxpy_(int*, double*, double*, int*, double*, int*);

/*
    Copy
*/
void scopy_(int*, float*, int*, float*, int*);
void dcopy_(int*, double*, int*, double*, int*);

/*
    Swap
*/
void sswap_(int*, float*, int*, float*, int*);
void dswap_(int*, double*, int*, double*, int*);

/*
    2 Norm
*/
float snrm2_(int *, float*, int*);
double dnrm2_(int *, double*, int*);

/*
    Sum of Absolute Values
*/
float sasum_(int *, float*, int*);
double dasum_(int *, double*, int*);

/*
    Scale
*/
void sscal_(int*, float*, float*, int*);
void dscal_(int*, double*, double*, int*);

/*
    Maximum absolute value
*/
int isamax_(int *, float*, int*);
int idamax_(int *, double*, int*);


/*
    Givens Plane Rotation
*/
void srotg_(float*, float*, float*, float*);
void drotg_(double*, double*, double*, double*);
#if 0
void crotg_(complex<float>*,complex<float>*,float*,complex<float>*);
void zrotg_(complex<double>*,complex<double>*,double*,complex<double>*);
#endif
void srot_(int*, float*, int*, float*, int*, float*, float*);
void drot_(int*, double*, int*, double*, int*, double*, double*);
#if 0
/* MTL implements ccrot and zzrot */
void csrot_(int*, complex<float>*, int*, complex<float>*, int*, 
	    complex<float>*, complex<float>*);
void zdrot_(int*, complex<double>*, int*, complex<double>*, int*,
	    double*, double*);
#endif

/*---------------------------------------------------------
    Level 2 BLAS
-----------------------------------------------------------*/

void dgemv_(char*, int*, int*, double*, double*, int*,
	    double*, int*, double*, double*, int*);

void dger_(int*, int*, double*, double*, int*, double*,
	   int*, double*, int*);

void dgbmv_(char*, int*, int*, int*, int*, double*, double*, int*,
	    double*, int*, double*, double*, int*);


void dtrsv_(char* uplo, char* trans, char* diag, int* n, double *da, 
	    int* lda, double *dx, int* incx);

/*---------------------------------------------------------
    Level 3 BLAS
-----------------------------------------------------------*/
void dgemm_(const char* transa, const char* transb, 
	    const int* m, const int* n, const int* k,
	    const double* alpha,  const double *da,  const int* lda,
	    const double *db, const int* ldb, const double* dbeta,
	    double *dc, const int* ldc);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
