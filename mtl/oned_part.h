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

#ifndef _MTL_ONED_PART_
#define _MTL_ONED_PART_

/*
  A class template that allows for flexible indexing of a 1-D part of
  a matrix, depending on what shape and storage the matrix is in.
  The Indexer class is responsible for filling in how indexing is
  suppose to work.
*/

/* #include "mtl/iterator_adaptor.h" no longer used */
#include "mtl/reverse_iter.h"
#include "mtl/scaled1D.h"

namespace mtl {


template <class Vector, class VecRef, class Indexer>
class oned_part {
public:
  typedef typename Vector::iterator base_iterator;
  typedef typename Vector::const_iterator const_base_iterator;
  typedef oned_part self;
  enum { N = Vector::N };

  /***** associated types ******/
  typedef typename Vector::value_type value_type;
  typedef typename Vector::reference reference;
  typedef typename Vector::const_reference const_reference;
  typedef typename Vector::pointer pointer;
  typedef typename Vector::size_type size_type;
  typedef typename Vector::difference_type difference_type;
  typedef typename Vector::subrange_type inner_subrange;
  typedef oned_part<inner_subrange, inner_subrange, Indexer> subrange_type;

  typedef typename Vector::sparsity sparsity;
  typedef typename Vector::IndexArrayRef IndexArrayRef;
  typedef typename Vector::IndexArray IndexArray;
  typedef typename Vector::dimension dimension;
  typedef scaled1D< self > scaled_type;

  /***** iterator classes *****/  

  template <int isConst>
  class __iterator { 
    typedef __iterator self;
    typedef typename IF<isConst,const_base_iterator,base_iterator>::RET Iterator;
  public:
#if !defined( _MSVCPP_ )
    typedef typename std::iterator_traits<Iterator>::difference_type
                               difference_type;
#else
	typedef typename std::iterator_traits<Iterator>::distance_type
                               difference_type;
	typedef difference_type distance_type;
#endif
    typedef typename std::iterator_traits<Iterator>::value_type value_type;
    typedef typename std::iterator_traits<Iterator>::iterator_category 
                               iterator_category;
#if !defined (_MSVCPP_)
    typedef typename std::iterator_traits<Iterator>::pointer pointer; 
    typedef typename std::iterator_traits<Iterator>::reference reference;
#else
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
#endif

    typedef difference_type Distance;
    typedef Iterator iterator_type;

    inline __iterator() { }

    inline __iterator(const Iterator& x, Indexer ind)
      : iter(x), indexer(ind) { }

    inline __iterator(const self& x)
      : iter(x.iter), indexer(x.indexer) { }

    inline self& operator=(const self& x) { 
      iter = x.iter; indexer = x.indexer; return *this; 
    }

    inline operator Iterator() { return iter; }

    inline Iterator base() const { return iter; }

    inline reference operator*() const { return *iter; }

    inline self& operator++() { ++iter; return *this; }

    inline self operator++(int) { 
      self tmp = (*this);
      ++(*this); 
      return tmp; 
    }
    
    inline self& operator--() { --iter; return *this; }

    inline self operator--(int) { 
      self tmp = (*this);
      --(*this); 
      return tmp; 
    }
    
    inline self operator+(Distance n) const {
      self tmp = (*this);
      tmp += n;
      return tmp;
    }

    inline self& operator+=(Distance n) {
      iter += n; return (*this); 
    }

    inline self operator-(Distance n) const {
      self tmp = (*this); 
      tmp -= n;
      return tmp;
    }

    inline self& operator-=(Distance n) { 
      iter -= n; return (*this); 
    }

    inline value_type operator[](Distance n) const {
      self tmp = (*this);
      return *(tmp += n);
    }
    
    inline Distance operator-(const self& y) const {
      return iter - y.iter;
    }
    
    inline bool operator==(const self& y) const {
      return iter == y.iter; 
    }
    
    inline bool operator!=(const self& y) const {
      return iter != y.iter; 
    }
    
    inline bool operator<(const self& y) const {
      return iter < y.iter; 
    }  
    
    inline size_type row() const { return indexer.row(iter); }
    inline size_type column() const { return indexer.column(iter); }
    inline size_type index() const { return indexer.minor(iter); }
  protected:
    Iterator iter;
    Indexer indexer;
  };

  typedef __iterator<0> iterator;
  typedef __iterator<1> const_iterator;

  typedef reverse_iter< iterator > reverse_iterator;
  typedef reverse_iter< const_iterator > const_reverse_iterator;

  /***** oned_part methods *****/

  inline oned_part(VecRef v, Indexer ind) : vec(v), indexer(ind) { }

  inline oned_part(const oned_part& x) : vec(x.vec), indexer(x.indexer) { }

#if !defined(_MSVCPP_)
  template <class OtherOneD>
  inline oned_part(const OtherOneD& x) : vec(x.vec), indexer(x.indexer) { }
#endif

  inline oned_part& operator=(const oned_part& x) {
    vec = x.vec; indexer = x.indexer; return *this;
  }

  template <class OtherOneD>
  inline oned_part& operator=(const OtherOneD& x) {
    vec = x.vec; indexer = x.indexer; return *this;
  }
  
  inline iterator begin() { /* JGS possibly don't need indexer::begin() */
    return iterator(indexer.begin(vec.begin()), indexer);
  }
  inline iterator end() {
    return iterator(vec.end(), indexer);
  }
  inline const_iterator begin() const {
    return const_iterator(indexer.begin(vec.begin()), indexer);
  }
  inline const_iterator end() const {
    return const_iterator(vec.end(), indexer);
  }

  inline reverse_iterator rbegin() { return reverse_iterator(end()); }
  inline reverse_iterator rend() { return reverse_iterator(begin()); }

  inline const_reverse_iterator rbegin() const {
    return const_reverse_iterator(end()); 
  }
  inline const_reverse_iterator rend() const {
    return const_reverse_iterator(begin()); 
  }
  
  inline reference operator[](size_type n) {
    return vec[indexer.at(n)]; 
  }
  inline const_reference operator[](size_type n) const { 
    return vec[indexer.at(n)]; 
  }

  //added by Rich
  inline value_type& get_ref(size_type j) {
    return vec.get_ref(j);
  }

  inline subrange_type operator()(size_type i, size_type j) const {
    return subrange_type(vec(indexer.at(i), indexer.at(j)), indexer); 
  }

  inline size_type size() const { return vec.size(); }

  //add by Rich
  inline void reserve(size_type i) {
    vec.reserve(i);
  }
  //add by Rich
  inline void resize(size_type i) {
    vec.resize(i);
  }

  inline void clear() { vec.clear(); }
  inline void push_back(size_type i, const value_type& t) {
    vec.push_back(i, t);
  }

  inline size_type nnz() const { return vec.nnz(); }

  inline IndexArrayRef nz_struct() const { return vec.nz_struct(); }

  /*
    inline void adjust_index(size_type i) { vec.adjust_index(i); }
    */

  /* protected:*/
  VecRef vec;
  Indexer indexer;
};

} /* namespace mtl */
#endif
