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

#ifndef MTL_NOT_AT_H
#define MTL_NOT_AT_H

#include "mtl/mtl_iterator.h"

#include "mtl/mtl_config.h"


namespace mtl {

using std::random_access_iterator_tag;
using std::input_iterator_tag;

template <class Ran1, class Ran2>
inline bool not_at(const Ran1& a, const Ran2& b, random_access_iterator_tag) {
  return a < b;
}

template <class Iter1, class Iter2>
inline bool not_at(const Iter1& a, const Iter2& b, input_iterator_tag) {
  return a != b;
}

template <class Iter1, class Iter2>
inline bool not_at(const Iter1& a, const Iter2& b) {
#if !defined ( _MSVCPP_ )
	typedef typename std::iterator_traits<Iter1>::iterator_category Category;
  return mtl::not_at(a, b, Category());
#else
  return a != b;
#endif
}

inline bool not_at(int a, int b) {
  return a < b;
}

} /* namespace mtl */

#endif // MTL_NOT_AT_H
