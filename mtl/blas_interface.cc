//
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
//
//------------------------------------------------------------
// Basic Linear Algebra Subprograms for C/C++
// Version 1.0
// Matthew E. Gaston
// May 6, 1998
// ------------------------------------------------------------

#include "blas_interface.h"

#include "mtl/linalg_vec.h"
#include "mtl/matrix.h"
#include "mtl/mtl.h"
#include <math.h>


// TO DO: check whether int args should be const
// and then change the prototypes and func defs

using mtl;

typedef external_vec<float> svec;
typedef external_vec<double> dvec;

typedef matrix<double, rectangle<>, dense<>, column_major>::type col_matrix_d;
typedef matrix<double, rectangle<>, dense<>, column_major>::type col_matrix_s;



#define MTL_FCALL(X) X##_

using namespace mtl;

//------------------------------------------------------------
//  Dot product of floats return a float
//------------------------------------------------------------
float
MTL_FCALL(sdot)(int *n, float *sx, int *incx, float *sy, int *incy)
{
  int N = *n;
  int ix = *incx;
  int iy = *incy;
  
  float sw = 0.0;

  if (N <= 0) return sw;

  svec x(sx, N * ix);
  svec y(sy, N * iy);

  if (ix == 1 && iy == 1)
    sw = dot(x, y, sw);
  else if (ix == 1)
    sw = dot(x, strided(y, iy), sw);
  else if (iy == 1)
    sw = dot(strided(x, ix), y, sw);
  else
    sw = dot(strided(x, ix), strided(y, iy), sw);

  return sw;
}
    
//------------------------------------------------------------
//  Dot product of floats return a double
//------------------------------------------------------------
double
MTL_FCALL(dsdot)(int *n, float *sx, int *incx, float *sy, int *incy)
{
  int N = *n;
  int ix = *incx;
  int iy = *incy;
  
  double dw = 0.0;

  if (N <= 0) return dw;

  svec x(sx, N * ix);
  svec y(sy, N * iy);

  if (ix == 1 && iy == 1)
    dw = dot(x, y, dw);
  else if (ix == 1)
    dw = dot(x, strided(y, iy), dw);
  else if (iy == 1)
    dw = dot(strided(x, ix), y, dw);
  else
    dw = dot(strided(x, ix), strided(y, iy), dw);

  return dw;
}


//------------------------------------------------------------
//  Dot product of floats plus a float return a float
//------------------------------------------------------------
float
MTL_FCALL(sdsdot)(int *n, float *sb, float *sx, int *incx, float *sy, int *incy)
{
  int N = *n;
  int ix = *incx;
  int iy = *incy;
  float b = *sb;
  
  float sw = 0.0;

  if (N <= 0) return sw;

  svec x(sx, N * ix);
  svec y(sy, N * iy);

  if (ix == 1 && iy == 1)
    sw = dot(x, y, sw);
  else if (ix == 1)
    sw = dot(x, strided(y, iy), sw);
  else if (iy == 1)
    sw = dot(strided(x, ix), y, sw);
  else
    sw = dot(strided(x, ix), strided(y, iy), sw);

  sw += b;

  return sw;
}

//------------------------------------------------------------
//  Dot product of doubles return a double
//------------------------------------------------------------
double
MTL_FCALL(ddot)(int *n, double *dx, int *incx, double *dy, int *incy)
{
  int N = *n;
  int ix = *incx;
  int iy = *incy;
  
  double dw = 0.0;

  if (N <= 0) return dw;

  dvec x(dx, N * ix);
  dvec y(dy, N * iy);

  if (ix == 1 && iy == 1)
    dw = dot(x, y, dw);
  else if (ix == 1)
    dw = dot(x, strided(y, iy), dw);
  else if (iy == 1)
    dw = dot(strided(x, ix), y, dw);
  else
    dw = dot(strided(x, ix), strided(y, iy), dw);

  return dw;
}


//------------------------------------------------------------
//  AXPY for floats
//------------------------------------------------------------
void
MTL_FCALL(saxpy)(int *n, float *sa, float *sx, int *incx, float *sy, int *incy)
{
  int N = *n;
  int ix = *incx;
  int iy = *incy;
  float a = *sa;

  if (a == 0 || N <= 0) return;

  svec x(sx, N * ix);
  svec y(sy, N * iy);

  // no scale to x -- i.e. a = 1;
  if (a == 1) {
    if (ix == 1 && iy == 1)
      add(x, y, y);
    else if (iy == 1)
      add(strided(x, ix), y, y);
    else if (ix == 1)
      add(x, strided(y, iy), strided(y, iy));
    else
      add(strided(x, ix), strided(y, iy), strided(y,iy));
  }
  // must scale x
  else {
    if (ix == 1 && iy == 1)
      add(scaled(x, a), y, y);
    else if (iy == 1)
      add(scaled(strided(x, ix), a), y, y);
    else if (ix == 1)
      add(scaled(x, a), strided(y, iy), strided(y, iy));
    else
      add(scaled(strided(x, ix), a), strided(y, iy), strided(y, iy));
  }

}


//------------------------------------------------------------
//  AXPY for doubles
//------------------------------------------------------------
void
MTL_FCALL(daxpy)(int *n, double *da, double *dx, int *incx,
       double *dy, int *incy)
{
  int N = *n;
  int ix = *incx;
  int iy = *incy;
  double a = *da;

  if (a == 0 || N <= 0) return;

  dvec x(dx, N * ix);
  dvec y(dy, N * iy);

  // no scale to x -- i.e. a = 1;
  if (a == 1) {
    if (ix == 1 && iy == 1)
      add(x, y, y);
    else if (iy == 1)
      add(strided(x, ix), y, y);
    else if (ix == 1)
      add(x, strided(y, iy), strided(y,iy));
     else 
      add(strided(x, ix), strided(y,iy), strided(y,iy));
  }
  // must scale x
  else {
    if (ix == 1 && iy == 1)
      add(scaled(x, a), y, y);
    else if (iy == 1)
      add(scaled(strided(x, ix), a), y, y);
    else if (ix == 1)
      add(scaled(x, a), strided(y,iy), strided(y,iy));
    else
      add(scaled(strided(x, ix), a), strided(y,  iy), strided(y,  iy));
  }

}

//------------------------------------------------------------
//  COPY for floats
//------------------------------------------------------------
void
MTL_FCALL(scopy)(int *n, float *sx, int *incx, float *sy, int *incy)
{
  int N = *n;
  int ix = *incx;
  int iy = *incy;

  if (N <= 0) return;

  svec x(sx, N * ix);
  svec y(sy, N * iy);

  if (ix == 1 && iy == 1)
    copy(x, y);
  else if (ix == 1)
    copy(x, strided(y, iy));
  else if (iy == 1)
    copy(strided(x, ix), y);
  else
    copy(strided(x, ix), strided(y, iy));
}


//------------------------------------------------------------
//  COPY for doubles
//------------------------------------------------------------
void
MTL_FCALL(dcopy)(int *n, double *dx, int *incx, double *dy, int *incy)
{
  int N = *n;
  int ix = *incx;
  int iy = *incy;

  if (N <= 0) return;

  dvec x(dx, N * ix);
  dvec y(dy, N * iy);

  if (ix == 1 && iy == 1)
    copy(x, y);
  else if (ix == 1)
    copy(x, strided(y,  iy));
  else if (iy == 1)
    copy(strided(x, ix), y);
  else
    copy(strided(x, ix), strided(y,  iy));
}

//------------------------------------------------------------
//  SWAP for floats
//------------------------------------------------------------
void
MTL_FCALL(sswap)(int *n, float *sx, int *incx, float *sy, int *incy)
{
  int N = *n; 
  int ix = *incx;
  int iy = *incy;

  if (N <= 0) return;

  svec x(sx, N * ix);
  svec y(sy, N * iy);

  if (ix == 1 && iy == 1)
    swap(x, y);
  else if (ix == 1)
    swap(x, strided(y,  iy));
  else if (iy == 1)
    swap(strided(x, ix), y);
   else
    swap(strided(x, ix), strided(y,  iy));

}


//------------------------------------------------------------
//  SWAP for doubles
//------------------------------------------------------------
void
MTL_FCALL(dswap)(int *n, double *dx, int *incx, double *dy, int *incy)
{
  int N = *n; 
  int ix = *incx;
  int iy = *incy;

  if (N <= 0) return;

  dvec x(dx, N * ix);
  dvec y(dy, N * iy);

  if (ix == 1 && iy == 1)
    swap(x, y);
  else if (ix == 1)
    swap(x, strided(y,  iy));
  else if (iy == 1)
    swap(strided(x, ix), y);
  else
    swap(strided(x), strided(y,  iy));
}

//------------------------------------------------------------
//  NRM2 for floats (the 2 norm)
//------------------------------------------------------------
float
MTL_FCALL(snrm2)(int *n, float *sx, int *incx)
{
  int N = *n;
  int ix = *incx;

  float sw = 0.0;

  if (N <= 0) return sw;

  svec x(sx, N * ix);
  
  if (ix == 1)
    sw = two_norm(x);
  else
    sw = two_norm(strided(x, ix));

  return sw;

}

//------------------------------------------------------------
//  NRM2 for doubles (the 2 norm)
//------------------------------------------------------------
double
MTL_FCALL(dnrm2)(int *n, double *dx, int *incx)
{
  int N = *n;
  int ix = *incx;

  double dw = 0.0;

  if (N <= 0) return dw;

  dvec x(dx, N * ix);
  
  if (ix == 1)
    dw = two_norm(x);
  else
    dw = two_norm(strided(x, ix));

  return dw;
  
}

//------------------------------------------------------------
//  SUM of abs. values for floats
//------------------------------------------------------------

float
MTL_FCALL(sasum)(int *n, float *sx, int *incx)
{
  int N = *n;
  int ix = *incx;
  float sw = 0.0;

  if (N <= 0) return sw;

  svec x(sx, N * ix);

  if (ix == 1)
    sw = one_norm(x);
  else
    sw = one_norm(strided(x, ix));    

  return sw;
}

//------------------------------------------------------------
//  SUM of abs. values for doubles
//------------------------------------------------------------

double
MTL_FCALL(dasum)(int *n, double *dx, int *incx)
{
  int N = *n;
  int ix = *incx;

  double dw = 0.0;

  if (N <= 0) return dw;

  dvec x(dx, N * ix);

  if (ix == 1)
    dw = one_norm(x);
  else
    dw = one_norm(strided(x, ix));

  return dw;
}

//------------------------------------------------------------
//  Scale for floats
p//------------------------------------------------------------
void
MTL_FCALL(sscal)(int *n, float *sa, float *sx, int *incx)
{
  int N = *n;
  float a = *sa;
  int ix = *incx;

  if (N <= 0 || a == 1) return;
  
  svec x(sx, N * ix);

  if (ix == 1)
    scaled(x, a);
  else {
    scaled(strided(x, ix), a);
  }

}

//------------------------------------------------------------
//  Scale for doubles
//------------------------------------------------------------
void
MTL_FCALL(dscal)(int *n, double *da, double *dx, int *incx)
{
  int N = *n;
  double a = *da;
  int ix = *incx;

  if (N <= 0 || a == 1) return;
  
  dvec x(dx, N * ix);

  if (ix == 1)
    scaled(x, a);
  else {
    strided<dvec> sx(x, ix);
    scaled(sx, a);
  }

}
  
//------------------------------------------------------------
//  Largest component for floats (abs. value
//------------------------------------------------------------
int
MTL_FCALL(isamax)(int *n, float *sx, int *incx)
{
  int N = *n;
  int ix = *incx;
  int imax = 0;

  if (N <= 0) return imax;

  svec x(sx, N * ix);

  if (ix == 1)
    imax = max_index(x);
  else
    imax = max_index(strided(x, ix));

  return imax + 1;
}
 
//------------------------------------------------------------
//  Largest component for doubles (abs. value
//------------------------------------------------------------
int
MTL_FCALL(idamax)(int *n, double *dx, int *incx)
{
  int N = *n;
  int ix = *incx;
  int imax = 0;

  if (N <= 0) return imax;

  dvec x(dx, N * ix);

  if (ix == 1)
    imax = max_index(x);
  else
    imax = max_index(strided(x, ix));

  return imax + 1;
}

//**************************************************
// This stuff is the givens transformatioin
// stuff that is not implemented yet.  That
// is why it is #ifdef'ed out.
//**************************************************

#ifdef GIVENS

//****************************************
//  Modified rotations are omitted
//****************************************

//------------------------------------------------------------
//  Construct a Givens Plane Rotation for floats
//------------------------------------------------------------
void
srotg(float *sa, float *sb, float *sc, float *ss)
{
  float a = *sa;
  float b = *sb;

  givens_rotation<float> g(a, b);

  *sa = g.a();
  *sb = g.b();
  *sc = g.c();
  *ss = g.s();

#if 0
  float c = *sc;
  float s = *ss;
  float r = 0.0;
  float s = 0.0;
  int sigma = 1;  // should always equal 1 or -1;
  if (std::abs(a) > std::abs(b))
    sigma = std::abs(a)/a;
  else if (std::abs(b) >= std::abs(a))
    sigma = std::abs(b)/b;
  else 
    cout << "Error in SROTG caculating sigma" << endl;

  r = sigma*(sqrt(a*a) + (b*b));
  
  if (r != 0){
    c = a/r;
    s = b/r;
  }
  else {
    c = 1;
    s = 0;
  }

  float z = 0.0;

  if (c != 0) {
    if (std::abs(a) > std::abs(b))
      z = s;
    else
      z = 1/c;
  } else
    z = 1;

  *sa = r;
  *sb = z;
  *sc = c;
  *ss = s;
#endif
}


//------------------------------------------------------------
//  Construct a Givens Plane Rotation for doubles
//------------------------------------------------------------
void
MTL_FCALL(drotg)(double *da, double *db, double *dc, double *ds)
{
  double a = *da;
  double b = *db;

  givens_rotation<double> g(a, b);

  *da = g.a();
  *db = g.b();
  *dc = g.c();
  *ds = g.s();
}

//------------------------------------------------------------
//  Applying a Givens Plane Rotation for floats
//------------------------------------------------------------
void
MTL_FCALL(srot)(int *n, float *sx, int *incx, float *sy, 
      int *incy, float *sc, float *ss)
{
  int N = *n;
  int ix = *incx;
  int iy = *incy;
  float c = *sc;
  float s = *ss;

  if (N <= 0) return;

  s_vec x(sx, N), y(sy, N);
  givens_rotation<float> g;
  g.set_cs(c, s);
  g.apply(strided(x, ix), strided(y, iy));
}

//------------------------------------------------------------
//  Applying a Givens Plane Rotation for doubles
//------------------------------------------------------------
void
MTL_FCALL(drot)(int *n, double *dx, int *incx, double *dy, 
		int *incy, double *dc, double *ds)
{
  int N = *n;
  int ix = *incx;
  int iy = *incy;
  double c = *dc;
  double s = *ds;

  if (N <= 0) return;

  d_vec x(dx, N), y(dy, N);
  givens_rotation<double> g;
  g.set_cs(c, s);
  g.apply(strided(x, ix), strided(y, iy));
}



void MTL_FCALL(dgemv)(char* transp, int* m, int* n, double* alphap,
		      double* a, int* ldap, double* xp, int* incxp,
		      double* betap, double* yp, int* incyp)
{
  char trans = *transp;
  int M = *m;
  int N = *n;
  double alpha = *alphap;
  double beta = *betap;
  int lda = *ldap;
  int incx = *incxp;
  int incy = *incyp;

  dvec x(xp, N), y(yp, M);
  col_matrix_d A(a, M, N, lda);

  if (trans == 'N' || trans == 'n')
     mult(A, strided(scaled(x,alpha),incx), 
	  strided(scaled(s, beta), iy));
  else
    mult(trans(A), strided(scaled(x,alpha),incx), 
	 strided(scaled(y, beta), iy));
}



void MTL_FCALL(dger)(int*, int*, double*, double*, int*, double*,
		     int*, double*, int*)
{

}


#endif
