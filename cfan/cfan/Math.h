#ifndef _CF_MATH_H_
#define _CF_MATH_H_

#include "cfan/macro.h"

/*========================================================================
 * Math
 */

#define cf_Math_pi       3.14159265358979323846

#define cf_Math_max(a, b)  (((a) > (b)) ? (a) : (b))

#define cf_Math_min(a, b)  (((a) < (b)) ? (a) : (b))

#define cf_Math_abs(a)     (((a) < 0) ? -(a) : (a))

#define cf_Math_clamp(a, min, max) (cf_Math_min(cf_Math_max((a), (min)), (max)))

#define cf_Math_round(f) ((int)(f+0.5))

#define cf_Math_toRadians(f) ((f)/180.0*cf_Math_pi)

#define cf_Math_toDegrees(f) ((f)/cf_Math_pi*180.0)

static inline bool cf_Math_isPowerOf2(uint32_t x) {
  return !(x & (x-1));
}

static inline uint32_t cf_Math_nextPowerOf2(uint32_t x) {
  if ( cf_Math_isPowerOf2(x) ) return x;
  x |= x>>1;
  x |= x>>2;
  x |= x>>4;
  x |= x>>8;
  x |= x>>16;
  return x+1;
}

#endif // _CF_MATH_H_
