/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CF_TYPE_H_
#define _CF_TYPE_H_

/*************************************************************************
 * Fixed int type
 */

#ifdef  __STDC__
  #include <stdint.h>
#else
  typedef signed char       int8_t;
  typedef signed short      int16_t;
  typedef signed long       int32_t;
  typedef unsigned char     uint8_t;
  typedef unsigned short    uint16_t;
  typedef unsigned long     uint32_t;

  #if defined(_MSC_VER) || defined(__BORLANDC__)
    typedef signed __int64      int64_t;
    typedef unsigned __int64    uint64_t;
  #else
    typedef signed long long    int64_t;
    typedef unsigned long long  uint64_t;
  #endif
#endif

/*************************************************************************
 * Boolean
 */

#ifndef __cplusplus
  #define bool    _Bool
  #define true    1
  #define false   0
#endif

#endif