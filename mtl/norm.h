//===========================================================================
//  CVS Information:                                                         
//                                                                           
//     $RCSfile: norm.h,v $  $Revision: 1.1.1.1 $  $State: Exp $ 
//     $Author: jsiek $  $Date: 2000/07/12 21:51:46 $ 
//     $Locker:  $ 
//---------------------------------------------------------------------------
//                                                                           
// DESCRIPTION                                                               
//                                                                           
//---------------------------------------------------------------------------
//                                                                           
// LICENSE AGREEMENT                                                         
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
//---------------------------------------------------------------------------
//                                                                           
// REVISION HISTORY:                                                         
//                                                                           
// $Log: norm.h,v $
// Revision 1.1.1.1  2000/07/12 21:51:46  jsiek
// stable mtl version
//
// Revision 1.4  1999/08/30 17:49:12  jsiek
// changed config.h to mtl_config.h
//
// Revision 1.3  1999/06/10 15:29:04  jsiek
// bunch of changes
//
// Revision 1.1.1.1  1999/06/04 16:58:31  jeremys
// Import MTL
//
// Revision 1.2  1999/03/05 16:13:36  jsiek
// config
//
// Revision 1.1  1999/01/16 14:33:51  jsiek
// *** empty log message ***
//
// Revision 1.4  1998/10/07 16:13:17  jsiek
// asdf
//
// Revision 1.3  1998/08/21 19:51:21  llee1
// it was horrible that I forget to make it inlining
//
// Revision 1.2  1998/08/13 00:35:35  jsiek
// use MTL_NAMESPACE
//
// Revision 1.1  1998/08/12 20:55:54  llee1
// for real type
//
//                                                                           
//===========================================================================
#ifndef REAL_NORM_H
#define REAL_NORM_H

#include "mtl/mtl_config.h"

namespace std {

  template <class T>
  inline T norm(const T& a)
  {
    return a * a;
  }

} /* namespace std */

#endif
