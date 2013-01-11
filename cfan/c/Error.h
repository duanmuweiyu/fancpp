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

#include "Log.h"

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
  cf_Error_overflow
} cf_Error;

#define cf_assert(exp) do {\
    if (!(exp)) {\
      cf_Log_log(cf_Log_tag, cf_LogLevel_err, "error: %s", #exp);\
      exit(2);\
    }\
  } while(0);

#define cf_verify(exp) do {\
    if (!(exp)) {\
      cf_Log_log(cf_Log_tag, cf_LogLevel_err, "verify fail: %s", #exp);\
      exit(2);\
    }\
  } while(0);

#define cf_returnErrorIf(exp, val) do {\
    if (!(exp)) {\
      cf_Log_log(cf_Log_tag, cf_LogLevel_err, "error: %s", #exp);\
      CF_EXIT_FUNC\
      return val;\
    }\
  } while(0);

#define cf_gotoErrorIf(exp) do {\
    if (!(exp)) {\
      cf_Log_log(cf_Log_tag, cf_LogLevel_err, "error: %s", #exp);\
      goto error;\
    }\
  } while(0);

#endif