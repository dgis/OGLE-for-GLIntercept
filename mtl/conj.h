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

#ifndef MTL_CONJ_H
#define MTL_CONJ_H

#include "mtl/mtl_config.h"

namespace std {

// dummy conj function for real numbers
inline double conj(double a) {
  return a;
}
inline float conj(float a) {
  return a;
}
inline int conj(int a) {
  return a;
}
inline bool conj(bool a) {
  return a;
}

// dummy real and imag function for real numbers
inline double real(double a) {
  return a;
}
inline double imag(double) {
  return 0.0;
}

inline float real(float a) {
  return a;
}
inline float imag(float) {
  return 0.0;
}

// JGS need to add conj() adapters for vectors and matrices
// used in application of householder transform

} /* namespace mtl */

#endif
