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
#ifdef  CF_WIN
  #ifdef CF_EXPORT
    #define CF_API __declspec(dllexport)
  #else
    #define CF_API __declspec(dllimport)
  #endif
#else
  #define CF_API
#endif


/*========================================================================
 * debug macro
 */

#ifdef DEBUG
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
#define CF_UNUSED(p) ((void)p)

/*========================================================================
 * align
 */
#define CF_ALIGN_SIZE 4
#define CF_ALIGNN(size, align) (((size)+((align)-1))&~((align)-1))
#define CF_ALIGN(size) CF_ALIGNN(size, CF_ALIGN_SIZE)

/*========================================================================
 * cast
 */
#define CF_STATIC_CAST(type, dst) ((type)(dst))
#define CF_REINTERPRET_CAST(type, dst) (*(((type) *)(&(dst))))

#endif //_CF_MACRO_H_
