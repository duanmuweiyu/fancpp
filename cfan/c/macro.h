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

/*************************************************************************
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

/*************************************************************************
 * Export API
 */

#undef  API
#ifdef  WIN32
  #ifdef CFAN_EXPORT
    #define API __declspec(dllexport)
  #else
    #define API __declspec(dllimport)
  #endif
#else
  #define  API
#endif

/*************************************************************************
 * Endian
 */

#ifndef ENDIAN
  const int __one = 1;
#else
  extern const int __one;
#endif

#define BIG_ENDIAN    (*(char *)(&__one)==0)
#define LITTLE_ENDIAN (*(char *)(&__one)==1)


/*************************************************************************
 * debug macro
 */
#ifdef _DEBUG
  #define CF_ENTRY_FUNC cf_Log_log("func", cf_LogLevel_debug, "entry function" )
  #define CF_EXIT_FUNC  cf_Log_log("func", cf_LogLevel_debug, "exit function" )
#else
  #define CF_ENTRY_FUNC
  #define CF_EXIT_FUNC
#endif

/*************************************************************************
 * misc
 */

#endif