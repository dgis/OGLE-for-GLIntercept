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

#ifndef MTL_BARE_BONES_ARRAY_H
#define MTL_BARE_BONES_ARRAY_H

#include "light1D.h"
#include <algorithm>

namespace mtl {

template <class T>
class bare_bones_array : public light1D<T> {
public:
  inline bare_bones_array(T* data, int n, int start = 0)
    : light1D<T>(data, n, start), owns_data(false) { }
  inline bare_bones_array(int n) : light1D<T>(new T[n], n), owns_data(true) { }
  inline bare_bones_array(const bare_bones_array& x) 
    : light1D<T>(new T[x.size()], x.size()), owns_data(false) {
      std::copy(x.begin(), x.end(), begin());
  }
  template <class Container>
  inline bare_bones_array& operator=(const Container& x) {
    // JGS assert sizes are the same
    MTL_ASSERT(size() == x.size(), "bare__bones_array::operator=");
    std::copy(x.begin(), x.end(), begin());    
  }
  inline ~bare_bones_array() { if (rep && owns_data) delete rep; }
protected:
  bool owns_data;
};

} /* namespace mtl */

#endif /* MTL_BARE_BONES_ARRAY_H */
