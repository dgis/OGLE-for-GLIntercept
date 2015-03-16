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

#ifndef MTL_REVERSE_ITER_H
#define MTL_REVERSE_ITER_H


#include "mtl/mtl_iterator.h"

#include "mtl/mtl_config.h"

namespace mtl {


#if STD_REVERSE_ITER
template <class Iter>
class reverse_iter : public std::reverse_iterator<Iter> {
	typedef std::reverse_iterator<Iter> super;
#else
template <class Iter>
class reverse_iter : public std::reverse_iterator<Iter,typename Iter::value_type,
                             typename Iter::reference, typename Iter::pointer> {
  typedef std::reverse_iterator<Iter, typename Iter::value_type,
	  typename Iter::reference, typename Iter::pointer> super;
#endif
public:
  typedef typename super::value_type value_type;

#if defined(_MSVCPP_)
  typedef typename super::distance_type difference_type;
  typedef difference_type distance_type;
  typedef typename super::reference_type reference;
#else
  typedef typename super::difference_type difference_type;
  typedef typename super::reference reference;
#endif


  typedef typename super::iterator_category iterator_category;

  inline reverse_iter() {}

  inline reverse_iter(const reverse_iter& x) : super(x) { }    

  inline explicit
  reverse_iter(Iter x) : super(x) {}

  inline difference_type index() const {
    Iter tmp = current;
    return (--tmp).index(); 
  }
  inline difference_type row() const {
    Iter tmp = current;
    return (--tmp).row(); 
  }
  inline difference_type column() const {
    Iter tmp = current;
    return (--tmp).column(); 
  }

};

} /* namespace mtl */

#endif
