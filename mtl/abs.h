#ifndef MTL_STD_ABS_H
#define MTL_STD_ABS_H

#include "mtl/mtl_config.h"

#ifndef HAVE_STD_ABS
namespace std {
  inline double abs(double a) {
    return a > 0 ? a : -a;
  }
  inline float abs(float a) {
    return a > 0 ? a : -a;
  }
  inline int abs(int a) {
    return a > 0 ? a : -a;
  }
}
#endif


#endif
