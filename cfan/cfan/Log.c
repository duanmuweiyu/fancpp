/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#include "cfan/Log.h"

#include <stdarg.h>
#include <stdio.h>

CF_API void cf_Log_doLog(const char *tag, const char *file, const char *func, const unsigned int line
  , const cf_LogLevel level, const char *msg, ...)
{
  va_list args;
  va_start(args, msg);

  printf("%s:%d(%s,%d) ", tag, level, func, line);
  vprintf(msg, args);
  printf("\n");

  va_end(args);
}
