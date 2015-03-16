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

#ifndef MTL_INITIALIZE_H
#define MTL_INITIALIZE_H

#include "mtl/matrix_stream.h"

#include "mtl/entry.h"
#include "mtl/mtl_complex.h"
#include "mtl/conj.h"
#include "mtl/mtl_set.h"

namespace mtl {

using std::complex;
using std::conj;

class symmetric_tag;

//need add mmio.c on using
template <class Matrix, class T>
void
__initialize(Matrix& A, matrix_market_stream<T>& s, 
	     symmetric_tag)
{
  typedef typename Matrix::value_type VT;
  entry2<VT> e;

  if ( s.is_symmetric() ) {
    while( ! s.eof() ) {
      s >> e;
      int row = e.row;
      int col = e.col;
      A(row, col) = e.value;
    }
  } else {
    cout << " matrix type is symmetric but the matrix in the file is not" << endl;
    assert(0);
  }
}

template <class Matrix, class ANY_TAG, class T>
void
__initialize(Matrix& A, matrix_market_stream<T>& s,
	     ANY_TAG)
{
  typedef typename Matrix::value_type VT;
  entry2<VT> e;

  if ( s.is_symmetric() ) {
    while( ! s.eof() ) {
      s >> e;
      int row = e.row;		// g++ internal compiler error
      int col = e.col;		//      A(e.row, e.col) = e.value;
      A(row, col) = e.value;
      if ( s.is_hermitian() )  
	A(e.col, e.row) = std::conj(e.value);
      else
	A(e.col, e.row) = e.value;
    }
  } else {
    while( ! s.eof() ) {
      s >> e;
      int row = e.row;
      int col = e.col;
      A(row, col) = e.value;
    }
  }
}

template <class Matrix, class TT>
void
initialize(Matrix& A, matrix_market_stream<TT>& s)
{
  typedef typename Matrix::value_type T;
  typedef typename Matrix::shape Shape;
  mtl::set_value(A, T(0));
  mtl::__initialize(A, s, Shape());
}

//need add iohb.c on using
template <class Matrix, class TT>
void
initialize(Matrix& A, harwell_boeing_stream<TT>& s)
{ //for hbs, shape doesn't matter
  typedef typename Matrix::size_type Int;
  typedef typename Matrix::value_type T;

  mtl::set_value(A, T(0));
  entry2<T> e;

  while( ! s.eof() ) {
    s >> e;
    Int row = e.row;
    Int col = e.col;
    A(row, col) = e.value;
  }
}

} /* namespace mtl */

#endif
