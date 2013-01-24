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
#include <string.h>

void cf_Log_print(const char *tag, const char *file, const char *func, const unsigned int line
                  , const cf_LogLevel level, const char *msg, va_list args);
cf_Log_listener cf_Log_listenerList[256] = { cf_Log_print };
int cf_Log_size = 1;

void cf_Log_print(const char *tag, const char *file, const char *func, const unsigned int line
                  , const cf_LogLevel level, const char *msg, va_list args) {
  printf("%s:%d(%s,%d) ", tag, level, func, line);
  vprintf(msg, args);
  printf("\n");
}

void cf_Log_doLog(const char *tag, const char *file, const char *func, const unsigned int line
  , const cf_LogLevel level, const char *msg, ...)
{
  va_list args;
  int i;
  va_start(args, msg);

  for (i=0; i<cf_Log_size; ++i) {
    (*(cf_Log_listenerList[i]))(tag, file, func, line, level, msg, args);
  }

  va_end(args);
}

int cf_Log_addListener(cf_Log_listener listener) {
  if (cf_Log_size >= 256) return -1;
  cf_Log_listenerList[cf_Log_size] = listener;
  ++cf_Log_size;
  return cf_Log_size - 1;
}

bool cf_Log_removeListener(int id) {
  if (id < 0 || id > 255) {
    return false;
  }
  memmove(&cf_Log_listenerList[id], &cf_Log_listenerList[id+1], sizeof(cf_Log_listener));
  --cf_Log_size;
  return true;
}
