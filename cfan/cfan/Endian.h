/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-6  Jed Young  Creation
 */

#ifndef _CF_ENDIAN_H_
#define _CF_ENDIAN_H_

#include "cfan/Error.h"

CF_BEGIN

enum cf_Endian {
  cf_Endian_big,
  cf_Endian_little
};

static inline bool cf_Endian_isBig(){
  static const int __one = 1;
  return (*(char *)(&__one)==0);
}

static inline void cf_Endian_swap16p(void *lpMem) {
  char * p = (char*)lpMem;
  p[0] = p[0] ^ p[1];
  p[1] = p[0] ^ p[1];
  p[0] = p[0] ^ p[1];
}

static inline void cf_Endian_swap32p(void *lpMem) {
  char * p = (char*)lpMem;
  p[0] = p[0] ^ p[3];
  p[3] = p[0] ^ p[3];
  p[0] = p[0] ^ p[3];
  p[1] = p[1] ^ p[2];
  p[2] = p[1] ^ p[2];
  p[1] = p[1] ^ p[2];
}

static inline void cf_Endian_swap64p(void *lpMem) {
  char * p = (char*)lpMem;
  p[0] = p[0] ^ p[7];
  p[7] = p[0] ^ p[7];
  p[0] = p[0] ^ p[7];
  p[1] = p[1] ^ p[6];
  p[6] = p[1] ^ p[6];
  p[1] = p[1] ^ p[6];
  p[2] = p[2] ^ p[5];
  p[5] = p[2] ^ p[5];
  p[2] = p[2] ^ p[5];
  p[3] = p[3] ^ p[4];
  p[4] = p[3] ^ p[4];
  p[3] = p[3] ^ p[4];
}

#define cf_Endian_swap16(s) ((((s) & 0xff) << 8) | (((s) >> 8) & 0xff))

#define cf_Endian_swap32(A)  ((((uint32)(A) & 0xff000000) >> 24) | \
                          (((uint32)(A) & 0x00ff0000) >> 8) | \
                          (((uint32)(A) & 0x0000ff00) << 8) | \
                          (((uint32)(A) & 0x000000ff) << 24))

#define cf_Endian_swap64(ll)  (((ll) >> 56) |\
            (((ll) & 0x00ff000000000000) >> 40) |\
            (((ll) & 0x0000ff0000000000) >> 24) |\
            (((ll) & 0x000000ff00000000) >> 8)	|\
            (((ll) & 0x00000000ff000000) << 8)	|\
            (((ll) & 0x0000000000ff0000) << 24) |\
            (((ll) & 0x000000000000ff00) << 40) |\
            (((ll) << 56)))

CF_END

#endif // _CF_ENDIAN_H_
