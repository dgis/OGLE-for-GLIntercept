// -*- c++ -*-
//
// $Id: refcnt_ptr.h,v 1.1.1.1 2000/07/12 21:51:46 jsiek Exp $
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

#ifndef MTL_REFCNT_PTR_H
#define MTL_REFCNT_PTR_H

#include "mtl/mtl_config.h"
#include "mtl/mtl_exception.h"

namespace mtl {


template <class Object>
class refcnt_ptr {
  typedef refcnt_ptr<Object> self;
public:

  inline refcnt_ptr() : object(0), count(0) { }

  inline refcnt_ptr(Object* c)
    : object(c), count(new int(1)) { }

  inline refcnt_ptr(const self& x) : object(x.object), count(x.count) {
    inc();
  }

  inline ~refcnt_ptr() { dec(); }

  inline self& operator=(Object* c) {
    if (object) dec();
    object = c;
    count = new int(1);
    return *this;
  }

  inline self& operator=(const self& x) {
    if (object) dec();
    object = x.object;
    count = x.count;
    inc();
    return *this;
  }

  inline Object& operator*() MTL_THROW_ASSERTION {
    MTL_ASSERT(count != 0, "refcnt_ptr::operator*()");
    MTL_ASSERT(*count >= 0, "refcnt_ptr::operator*()");
    return *object; 
  }

  inline const Object& operator*() const MTL_THROW_ASSERTION {
    MTL_ASSERT(count != 0, "const refcnt_ptr::operator*()");
    MTL_ASSERT(*count >= 0, "const refcnt_ptr::operator*()");
    return *object; 
  }

  inline Object* operator->() MTL_THROW_ASSERTION { 
    MTL_ASSERT(count != 0, "refcnt_ptr::operator->()");
    MTL_ASSERT(*count >= 0, "refcnt_ptr::operator->()");
    return object; 
  }

  inline const Object* operator->() const MTL_THROW_ASSERTION {
    MTL_ASSERT(count != 0, "const refcnt_ptr::operator->()");
    MTL_ASSERT(*count >= 0, "const refcnt_ptr::operator->()");
    return object; 
  }

  inline void inc() { if (count) (*count)++; }

  inline void dec() {
    if (count) {
      (*count)--; 
      if (*count <= 0) {
	delete object;
	delete count;
      }
    }
  }

protected:
  
  Object* object;
  int* count;
};

} /* namespace mtl */

#endif
