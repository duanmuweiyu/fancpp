/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CF_MISS_H_
#define _CF_MISS_H_

#ifdef  _MSC_VER
  #define inline      __inline
  #define __func__    __FUNCTION__
  #define snprintf    _snprintf
  #define strcasecmp  _stricmp
  #define strtoll     _strtoi64
  #define tzset       _tzset
  #define isnan(x)   _isnan(x)
#endif

#ifndef offsetof
#define offsetof(STRUCTURE,FIELD) ((int)((char*)&((STRUCTURE*)0)->FIELD))
#endif

#endif