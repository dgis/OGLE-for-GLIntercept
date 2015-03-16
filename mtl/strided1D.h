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
//
//===========================================================================


#ifndef MTL_STRIDED1D_H
#define MTL_STRIDED1D_H

#include "mtl/strided_iterator.h"
#include "mtl/reverse_iter.h"
#include "mtl/scaled1D.h"
#include "mtl/abs.h"

namespace mtl {


  //:  Strided Vector Adaptor
  //!category: containers, adaptors
  //!component: type
  //!definition: strided1D.h
  //!models: RandomAccessContainerRef
  //!tparam: RandomAccessContainerRef - base container type
  //
  //  This class in not meant to be used directly. Instead it is
  //  created automatically when the stride(x,inc) function
  //  is invoked. See the documentation for "Shortcut for Creating
  //  A Strided Vector".

template <class RandomAccessContainerRef>
class strided1D {
  typedef RandomAccessContainerRef Vector;
  typedef strided1D<Vector> self;
public:

  enum { N = RandomAccessContainerRef::N };

  /**@name Type Definitions */
  //: The value type
  //!wheredef: Container
  typedef typename Vector::value_type value_type;
  //: The type for references to the value type
  //!wheredef: Container
  typedef typename Vector::reference reference;
  //: The type for const references to the value type
  //!wheredef: Container
  typedef typename Vector::const_reference const_reference;
  //: The iterator type
  //!wheredef: Container
  typedef strided_iterator<typename Vector::iterator,0> iterator;
  //: The const iterator type
  //!wheredef: Container
  typedef strided_iterator<typename Vector::const_iterator,1> const_iterator;
  //: The reverse iterator type
  //!wheredef: Reversible Container
  typedef reverse_iter<iterator> reverse_iterator;
  //: The const reverse iterator type
  //!wheredef: Reversible Container
  typedef reverse_iter<const_iterator> const_reverse_iterator;
  //: The scaled vector type
  //!wheredef: Scalable
  typedef scaled1D<self> scaled_type;
  //: Whether the vector is sparse or dense
  typedef typename Vector::sparsity sparsity;
  //: The type for references to the index array
  typedef typename Vector::IndexArrayRef IndexArrayRef;
  //: The type for the index array
  typedef typename Vector::IndexArray IndexArray;
  //: This is a 1D container
  typedef typename Vector::dimension dimension;

  //: The unsigned integral type for dimensions and indices
  //!wheredef: Container
  typedef typename Vector::size_type size_type;
  //: The integral type for differences between iterators
  //!wheredef: Container
  typedef typename Vector::difference_type difference_type;
  //: The type for pointers to the value type
  typedef typename Vector::pointer pointer;
  //: The subrange vector type
  typedef strided1D<typename Vector::subrange_type> subrange_type;

  /**@name Constructors */
  
  //: Normal Constructor
  inline strided1D(const Vector& r, int stride_) 
    : rep(r), stride(stride_), 
      start(stride_ < 0 ? (r.size() + stride_) : 0),
      n(rep.size()/ MTL_ABS(stride_))
  { }

  //: Copy Constructor
  inline strided1D(const self& x)
    : rep(x.rep), stride(x.stride), start(x.start), n(x.n) { }

  inline self& operator=(const self& x) {
    rep = x.rep; stride = x.stride; start = x.start; n = x.n;
    return *this;
  }

  inline operator Vector&() { return rep; }
  /**@name Access Methods */
  
  //: Return an iterator pointing to the beginning of the vector
  //!wheredef: Container
  inline iterator begin() { 
    typename Vector::iterator i = rep.begin();
    i += start; /* VC++ workaround */
    return iterator(i, stride, 0); 
  }
  //: Return an iterator pointing past the end of the vector
  //!wheredef: Container
  inline iterator end() { 
    typename Vector::iterator i = rep.end();
    i -= start; /* VC++ workaround */
    return iterator(i, stride, n); 
  }
  //: Return a const iterator pointing to the begining of the vector
  //!wheredef: Container
  inline const_iterator begin() const {
    typename Vector::const_iterator i = rep.begin();
    i += start; /* VC++ workaround */
    return const_iterator(i, stride, 0); 
  }
  //: Return a const iterator pointing past the end of the vector
  //!wheredef: Container
  inline const_iterator end() const {
    typename Vector::const_iterator i = rep.end();
    i -= start; /* VC++ workaround */
    return const_iterator(i, stride, n); 
  }
  /* reverse iterators */
  //: Return a reverse iterator pointing to the last element of the vector
  //!wheredef: Reversible Container
  inline reverse_iterator rbegin() {
    return reverse_iterator(end());
  }
  //: Return a reverse iterator pointing past the end of the vector
  //!wheredef: Reversible Container
  inline reverse_iterator rend() { 
    return reverse_iterator(begin());
  }
  //: Return a const reverse iterator pointing to the last element of the vector
  //!wheredef: Reversible Container
  inline const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end());
  }
  //: Return a const reverse iterator pointing past the end of the vector
  //!wheredef: Reversible Container
  inline const_reverse_iterator rend() const {
    return const_reverse_iterator(begin());
  }

  //: Return a reference to the element with index i
  //!wheredef: Random Access Container
  inline reference operator[](size_type i) { return *(begin() + i); }
  //: Return a const reference to the element with index i
  //!wheredef: Random Access Container
  inline const_reference operator[](size_type i) const { 
    return rep[i * stride]; 
  }
  //: Return the size of the vector
  //!wheredef: Container  
  inline size_type size() const { return rep.size() / stride; }
  //: Return the number of non-zeroes
  //!wheredef: Vector  
  inline size_type nnz() const { return rep.nnz(); }

  //: Return a subrange vector containing the elements from index s to f
  inline subrange_type operator()(size_type s, size_type f) const {
    return subrange_type(rep(s*stride, f*stride), stride);
  }

  inline self& adjust_index(size_type i) { 
    rep.adjust_index(i);  // JGS??
    return *this; 
  }

protected:
  Vector rep;
  int stride;
  size_type start, n;
};

//: Shortcut for Creating a Strided Argument
//!category: containers
//!component: function
//!definition: strided1D.h
//!typereqs: Distance must be compatible with RandomAccessContainerRef's Distance
//!complexity: compile time
template <class RandomAccessContainerRef, class Distance>
inline strided1D<RandomAccessContainerRef>
strided(RandomAccessContainerRef v, Distance stride_)
{
  return strided1D<RandomAccessContainerRef>(v,stride_);
}


} /* namespace mtl */

#endif
