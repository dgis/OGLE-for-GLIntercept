// -*- c++ -*-
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
//===========================================================================

#ifndef MTL_LU_H
#define MTL_LU_H

#include "mtl/matrix.h"
#include "mtl/mtl.h"

  /* Note, the calls to copy() for row and column are 
     because I can't use row and column directly in the rank_one_update
     due to their indexing being in terms of A, not subA 
     I need to create some way to do this in constant time :)
  */

namespace mtl {

//: LU Factorization of a general (dense) matrix
//
// This is the outer product (a level-2 operation) form of the LU
// Factorization with pivoting algorithm . This is equivalent to
// LAPACK's dgetf2. Also see "Matrix Computations" 3rd Ed.  by Golub
// and Van Loan section 3.2.5 and especially page 115.
// <p>
// The pivot indices in ipvt are indexed starting from 1
// so that this is compatible with LAPACK (Fortran).
//
//!tparam: DenseMatrix - A dense MTL Matrix
//!tparam: Pvector - A Vector with integral element type
//!category: algorithms
//!component: function
//!example: lu_factorization.cc
template <class DenseMatrix, class Pvector>
int
lu_factor(DenseMatrix& A, Pvector& ipvt)
{
  typedef typename rows_type<DenseMatrix>::type RowMatrix;
  typedef typename columns_type<DenseMatrix>::type ColumnMatrix;
  typedef typename triangle_view<ColumnMatrix, lower>::type Lower;
  typedef typename triangle_view<RowMatrix, unit_upper>::type Unit_Upper;
  typedef typename triangle_view<ColumnMatrix, unit_lower>::type Unit_Lower;
  typedef typename DenseMatrix::value_type T;
  typedef typename DenseMatrix::size_type sizet;
  int info = 0;
  sizet j, jp, M = A.nrows(), N = A.ncols();

  Lower D(columns(A));
  Unit_Upper U(rows(A));
  Unit_Lower L(columns(A));
  dense1D<T> c(M), r(N);
  typename DenseMatrix::submatrix_type subA;
  
  typename Lower::iterator dcoli = D.begin();
  typename Unit_Upper::iterator rowi = U.begin();
  typename Unit_Lower::iterator columni = L.begin();
  
  for (j = 0; j < MTL_MIN(M - 1, N - 1); ++j, ++dcoli, ++rowi, ++columni) {

    jp = max_abs_index(*dcoli);		   /* find pivot */
    ipvt[j] = jp + 1;

    if ( A(jp, j) != T(0) ) {		   /* make sure pivot isn't zero */
      if (jp != j)
	mtl::swap(rows(A)[j], rows(A)[jp]); /* swap the rows */
      if (j < M - 1)
	scale(*columni, T(1) / A(j,j));    /* update column under the pivot */
    } else {
      info = j + 1;
      break;
    }

    if (j < MTL_MIN(M - 1, N - 1)) {
      subA = A.sub_matrix(j+1, M, j+1, N);

      /* TODO: Better to have an adaptor here -- A.L. */
      copy(*columni, c);  copy(*rowi, r);  /* translate to submatrix coords */
      rank_one_update(subA, scaled(c, T(-1)), r); /* update the submatrix */
    }
  }
  ipvt[j] = j + 1;

  return info;
}

/* For backward compatibility */
template <class DenseMatrix, class Pvector>
inline int
lu_factorize(DenseMatrix& A, Pvector& ipvt)
{
  return lu_factor(A, ipvt);
}

//: LU Solve
//
//  Solve equation Ax=b, given an LU factored matrix.
//
//  Usage:
//  <codeblock>
//      typedef matrix<double, rectangle<>, 
//                     dense<>, row_major>::type Matrix;
//      Matrix LU(A.nrows(), A.ncols());
//      dense1D<int> pvector(A.nrows());
//
//      copy(A, LU);
//      lu_factor(LU, pvector);
//
//      // call lu_solve with as many times for the same A as you want
//      lu_solve(LU, pvector, b, x);
//  </codeblock>
//
//  Thanks to Valient Gough for this routine!
//
//!tparam: DenseMatrix - A dense MTL Matrix which resulted from calling lu_factor
//!tparam: Pvector - A Vector with integral element type, the ipvt vector from lu_factor
//!category: algorithms
//!component: function
//!example: lu_solve.cc
template <class DenseMatrix, class VectorB, class VectorX, class Pvector>
void
lu_solve(const DenseMatrix &LU, const Pvector& pvector, 
	 const VectorB &b, VectorX &x)
{
  typedef typename Pvector::size_type p_int;

  copy(b, x);

  /* use the permutation vector to modify the starting vector 
   *  to account for the permutations in LU
   */
  for(p_int i=0; i < pvector.size(); i++) {
    p_int perm = pvector[i]-1;       // permutations stored in 1's offset
    if(i != perm)
      std::swap(x[i], x[perm]);
  }
  
  /* solve  Ax = b  ->  LUx = b  ->  Ux = L^-1 b 
   *  which we solve in two steps
   *  1) y = L^-1 b
   *  2) x = U^-1 y
   */
  typename triangle_view<DenseMatrix, unit_lower>::type L(LU);
  typename triangle_view<DenseMatrix, upper>::type U(LU);
  
  tri_solve(L, x);
  tri_solve(U, x);
}

//: LU Inverse
//
// Given an LU factored matrix, construct the inverse of the matrix.
//
//  Thanks to Valient Gough for this routine!
//
//!tparam: DenseMatrixLU - A dense MTL Matrix which resulted from calling lu_factor
//!tparam: DenseMatrix = The dense Matrix type used to store the inverse
//!tparam: Pvector - A Vector with integral element type, the ipvt vector from lu_factor
//!category: algorithms
//!component: function
template <class DenseMatrixLU, class DenseMatrix, class Pvector>
void
lu_inverse(const DenseMatrixLU& LU, const Pvector& pvector, DenseMatrix& AInv)
{
  typedef typename  matrix_traits<DenseMatrixLU>::value_type T;
  typedef typename Pvector::size_type p_int;
  dense1D<T> tmp(pvector.size());
  dense1D<T> result(pvector.size());
  mtl::set(tmp, 0.0);
  for(p_int i = 0; i < pvector.size(); i++) {
    tmp[i] = 1.0;
    lu_solve(LU, pvector, tmp, result);
    copy(result, columns(AInv)[i]);
    tmp[i] = 0.0;
  }
}



} /* namespace mtl */

#endif


