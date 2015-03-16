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

#ifndef MTL_STRIDED_ITERATOR_H
#define MTL_STRIDED_ITERATOR_H

#include "mtl/mtl_iterator.h"
#include "mtl/mtl_config.h"
#include "mtl/meta_if.h"

namespace mtl {


//: strided iterator
//
// This iterator moves a constant stride for each increment or
// decrement operator invoked. The strided iterator is used
// to implement a row-view to column oriented matrices, or
// column-views to row oriented matrices.
//
//!category: iterators, adaptors
//!component: type
//!defined: strided_iterator.h
//!models: RandomAccessIterator
//!tparams: RandomAccessIterator - the base iterator type
template <class RandomAccessIterator, int isConst>
class strided_iterator {
  typedef strided_iterator self;
public:
#if !defined( _MSVCPP_ )
  //: The type for the difference between two iterators
  typedef typename std::iterator_traits<RandomAccessIterator>::difference_type
                      difference_type;
#else
  typedef typename std::iterator_traits<RandomAccessIterator>::distance_type
                      difference_type;
  typedef difference_type distance_type;
#endif
  //: The value type pointed to by this iterator type
  typedef typename std::iterator_traits<RandomAccessIterator>::value_type
                      value_type;
  //: The iterator category for this iterator
  typedef typename std::iterator_traits<RandomAccessIterator>::iterator_category
                      iterator_category;
#if !defined( _MSVCPP_ )
  //: The type for references to the value type
  typedef typename std::iterator_traits<RandomAccessIterator>::reference
                      reference;
  //: The type for pointers to the value type
  typedef typename std::iterator_traits<RandomAccessIterator>::pointer
                      pointer;
#else
  typedef typename IF<isConst, const value_type&, value_type&>::RET reference;
  typedef typename IF<isConst, const value_type*, value_type*>::RET pointer;
#endif

  typedef difference_type      Distance;
  //: The underlying iterator type
  typedef RandomAccessIterator iterator_type;

  //: Default Constructor
  inline strided_iterator() : stride(0), pos(0) { }

  //: Construct from the underlying iterator
  inline strided_iterator(const RandomAccessIterator& x, int s, int p) 
    : iter(x), stride(s), pos(p) { }

  //: Copy Constructor
  inline strided_iterator(const self& x)
    : iter(x.iter), stride(x.stride), pos(x.pos) { }

  //: Assignment Operator
  inline self& operator=(const self& x) {
    iter = x.iter; stride = x.stride; pos = x.pos; return *this;
  }

  //: Return the index of the element this iterator points to
  //!wheredef: IndexedIterator
  inline int index() const { return iter.index() / stride; }
  
  //: Convert to the underlying iterator
  inline operator RandomAccessIterator () const { return iter; }

  inline RandomAccessIterator base() const { return iter; }

  //: Dereference, return the element currently pointed to
  inline reference operator*() const { return *iter; }

  //: Pre-increment operator
  inline self& operator++ (){
    ++pos; iter += stride; return *this;
  }
  //: Post-increment operator
  inline self operator++ (int){ 
    self tmp = *this; ++pos; iter += stride; return tmp; 
  }
  //: Pre-decrement operator
  inline self& operator-- (){ 
    --pos; iter -= stride; return *this; 
  }
  //: Post-decrement operator
  inline self operator-- (int){ 
    self tmp = *this; --pos; iter -= stride; return tmp; 
  }
  //: Add this iterator and n  
  inline self operator+ (Distance n) const {
    return self (iter + n*stride, stride, pos + n);
  }
  //: Add distance n to this iterator
  inline self& operator+= (Distance n) { 
    iter += n*stride; pos += n; return *this; 
  }
  //: Subtract this iterator and distance n
  inline self operator- (Distance n) const {
    return self (iter - n*stride, stride, pos - n);
  }
  //: Subtract distance n from this iterator
  inline self& operator-= (Distance n) {
    iter -= n*stride; pos -= n; return *this; 
  }

  //: Add this iterator and iterator x
  inline self operator+ (const self& x) const {
    return self(iter + x.iter, stride, pos + x.pos); 
  }
  //: Return this distance between this iterator and iterator x
  inline Distance operator- (const self& x) const { return iter - x.iter; }
  //: Return *(i + n)  
  inline reference operator[] (Distance n) const { return *(*this+n*stride); }
  //: Return whether this iterator is equal to iterator x
  inline bool operator==(const self& x) const { return pos == x.pos; }
  //: Return whether this iterator is not equal to iterator x
  inline bool operator!=(const self& x) const { return pos != x.pos; }
  //: Return whether this iterator is less than iterator x
  inline bool operator<(const self& x) const { return pos < x.pos; }  
protected:
  RandomAccessIterator iter;
  int stride;
  int pos;
};

#if 0
template <class RandomAccessIterator>
inline strided_iterator<RandomAccessIterator,1>
str(const RandomAccessIterator& x, int stride = 1) {
  return strided_iterator<RandomAccessIterator,1>(x, stride);
}
#endif

} /* namespace mtl */

#endif







