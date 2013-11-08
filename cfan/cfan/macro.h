/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CF_MACRO_H_
#define _CF_MACRO_H_

/*========================================================================
 * extern "C"
 */

#ifdef  __cplusplus
  #define CF_BEGIN extern  "C" {
#else
  #define CF_BEGIN
#endif

#ifdef  __cplusplus
  #define CF_END }
#else
  #define CF_END
#endif

/*========================================================================
 * Export API
 */

#undef  CF_API
#ifdef  WIN32
  #ifdef CF_EXPORT
    #define CF_API __declspec(dllexport)
  #else
    #define CF_API __declspec(dllimport)
  #endif
#else
  #define CF_API
#endif

/*========================================================================
 * Endian
 */

static inline int cf_Endian_isBigEndian(){
  int __one = 1;
  return (*(char *)(&__one)==0);
}

//#if !defined(CF_BIG_ENDIAN) && !defined(CF_LITTLE_ENDIAN)
//  #if (('1234' >> 24) == '1')
//    #define CF_LITTLE_ENDIAN
//  #elif (('4321' >> 24) == '1')
//    #define CF_BIG_ENDIAN
//  #else
//    #error "Couldn't determine the endianness!"
//  #endif
//#endif

/*========================================================================
 * Math
 */

#define cf_Math_pi       3.14159265358979323846

#define cf_Math_max(a, b)  (((a) > (b)) ? (a) : (b))

#define cf_Math_min(a, b)  (((a) < (b)) ? (a) : (b))

#define cf_Math_abs(a)     (((a) < 0) ? -(a) : (a))

#define cf_Math_clamp(a, min, max) (cf_Math_min(cf_Math_max((a), (min)), (max)))

#define cf_Math_round(f) ((int)(f+0.5))


/*========================================================================
 * debug macro
 */

#ifdef _DEBUG
  #undef NDEBUG
#else
  #undef NDEBUG
  #define NDEBUG
#endif

#ifdef NDEBUG
  #undef CF_DEBUG
#else
  #define CF_DEBUG
#endif

/*========================================================================
 * misc
 */

/**
 * suppress unused warning
 */
#define cf_unused(p) ((void)p)

/*========================================================================
 * align
 */
#define CF_ALIGN_SIZE 4
#define CF_ALIGNN(size,n) ((size) + ((~((size)&((n)-1))+1)&((n)-1)))
#define CF_ALIGN(size) CF_ALIGNN(size, CF_ALIGN_SIZE)


#endif
