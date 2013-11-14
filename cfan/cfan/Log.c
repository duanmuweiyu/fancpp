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

#ifndef CF_NOLOG
  cf_LogLevel cf_Log_level = cf_LogLevel_debug;
#else
  cf_LogLevel cf_Log_level = cf_LogLevel_silent;
#endif

void cf_Log_print(cf_Log_Listener *self, const char *tag, const char *file, const char *func, const unsigned int line
                  , const cf_LogLevel level, const char *msg);

cf_Log_Listener cf_Log_defaultListener = { -1, cf_LogLevel_debug, cf_Log_print };
cf_Log_Listener *cf_Log_listenerList[256] = { &cf_Log_defaultListener };
int cf_Log_size = 1;
const char *cf_LogLevel_str[] = { "debug", "info", "warn", "error", "silent" };

mtx_t cf_Log_lock;
static bool cf_Log_inited = false;
void cf_Log_init() {
  if (cf_Log_inited) return;

  if (mtx_init(&cf_Log_lock, mtx_recursive) != thrd_success) {
    printf("ERROR: init lock error\n");
    return;
  }
  cf_Log_inited = true;
}


void cf_Log_setLogLevel(cf_LogLevel level) {
  cf_Log_init();
  mtx_lock(&cf_Log_lock);
  cf_Log_level = level;
  mtx_unlock(&cf_Log_lock);
}

bool cf_Log_isEnableLevel(cf_LogLevel level) {
  return level >= cf_Log_level;
}

void cf_Log_print(cf_Log_Listener *self, const char *tag, const char *file, const char *func, const unsigned int line
                  , const cf_LogLevel level, const char *msg) {
  printf("%s:%c(%s,%d) %s\n", tag, cf_LogLevel_str[level][0], func, line, msg);
  cf_unused(self);
  cf_unused(file);
}

void cf_Log_doLog(const char *tag, const char *file, const char *func, const unsigned int line
  , const cf_LogLevel level, const char *msg, ...) {
  va_list args;
  int i;
  cf_Log_Listener *listener;
  char buffer[1024];
  va_start(args, msg);

  if (level < cf_Log_level) {
    va_end(args);
    return;
  }

  vsnprintf(buffer, 1024, msg, args);

  cf_Log_init();
  mtx_lock(&cf_Log_lock);
  for (i=0; i<cf_Log_size; ++i) {
    listener = cf_Log_listenerList[i];
    if (level < listener->level) continue;
    (*(listener->func))(listener, tag, file, func, line, level, buffer);
  }
  mtx_unlock(&cf_Log_lock);

  va_end(args);
}

int cf_Log_addListener(cf_Log_Listener *listener) {
  if (cf_Log_size >= 256) return -1;
  cf_Log_init();
  mtx_lock(&cf_Log_lock);
  cf_Log_listenerList[cf_Log_size] = listener;
  listener->id = cf_Log_size;
  ++cf_Log_size;
  mtx_unlock(&cf_Log_lock);
  return listener->id;
}

cf_Log_Listener *cf_Log_removeListener(int id) {
  cf_Log_Listener *old;
  if (id < 0 || id > 255) {
    return NULL;
  }
  cf_Log_init();
  mtx_lock(&cf_Log_lock);
  old = cf_Log_listenerList[id];
  memmove(&cf_Log_listenerList[id], &cf_Log_listenerList[id+1], sizeof(cf_Log_Listener*));
  --cf_Log_size;
  mtx_unlock(&cf_Log_lock);
  return old;
}