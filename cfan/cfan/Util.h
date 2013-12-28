#ifndef _CF_UTIL_H_
#define _CF_UTIL_H_

#include "cfan/macro.h"
#include <string.h>

CF_BEGIN


/*========================================================================
 * Any
 */

typedef union cf_Value_ {
  void *pointer;
  char charV;
  short shortV;
  int intV;
  long longV;
  long long longlongV;
  float floatV;
  double doubleV;
} cf_Value;

#define cf_toValue(p) (*((cf_Value*)(&(p))))

/*========================================================================
 * copy
 */
/**
 * fast memory copy
 */
static inline void *cf_memcpy(void *dest, const void *src, size_t n) {
  switch(n) {
  case 0:
    return dest;
  case 1:
    *(int8_t*)dest = *(int8_t*)src;
    return dest;
  case 2:
    *(uint16_t*)dest = *(uint16_t*)src;
    return dest;
  case 4:
    *(int32_t*)dest = *(int32_t*)src;
    return dest;
  case 8:
    *(int64_t*)dest = *(int64_t*)src;
    return dest;
  }
  return memcpy(dest, src, n);
}


CF_END

#endif // _CF_UTIL_H_
