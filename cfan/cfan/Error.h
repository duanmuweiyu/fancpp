/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CF_ERROR_H_
#define _CF_ERROR_H_

#include "cfan/Log.h"

/**
 * Error code list.
 * The 0 represents no error.
 *
 */
typedef enum cf_Error_ {
  cf_Error_ok,
  cf_Error_error,
  cf_Error_arg,
  cf_Error_io,
  cf_Error_index,
  cf_Error_null,
  cf_Error_alloc,
  cf_Error_unknow,
  cf_Error_notfound,
  cf_Error_parse,
  cf_Error_unsupported,
  cf_Error_eof,
  cf_Error_overflow,
  cf_Error_thread,
  cf_Error_file
} cf_Error;

/**
 * Bomb at condition fail.
 * Only effective at CF_DEBUG macro define.
 */
#ifdef CF_DEBUG
  #define cf_assert(exp) do {\
      if (!(exp)) {\
        cf_Log_log(cf_Log_tag, cf_LogLevel_err, "error: %s", #exp);\
        exit(2);\
      }\
    } while(0);
#else
  #define cf_assert(exp)
#endif

#define cf_abort(msg) {\
  cf_Log_log(cf_Log_tag, cf_LogLevel_err, "error: %s", msg);\
  abort();\
}

/**
 * exit function if condition is true.
 */
#define cf_returnErrorIf(exp, val) do {\
    if (!(exp)) {\
      cf_Log_log(cf_Log_tag, cf_LogLevel_err, "error: %s", #exp);\
      CF_EXIT_FUNC\
      return val;\
    }\
  } while(0);

/**
 * goto error label if condition is true.
 */
#define cf_gotoErrorIf(exp) do {\
    if (!(exp)) {\
      cf_Log_log(cf_Log_tag, cf_LogLevel_err, "error: %s", #exp);\
      goto error;\
    }\
  } while(0);

#endif