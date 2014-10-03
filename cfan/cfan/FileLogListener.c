/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-9-14  Jed Young  Creation
 */

#include "cfan/FileLogListener.h"

#include <stdio.h>
#include <time.h>

static FILE *cf_FileLogListener_getFile(cf_FileLogListener *self, const char *name) {
  cf_Error err;
  FILE *file;
  char filePath[1024];
  err = cf_HashMapSP_get(&self->map, name, NULL, (void**)&file);
  if (err == cf_Error_ok) {
    return file;
  }

  strcpy(filePath, self->path);
  strcpy(filePath+strlen(self->path), name);
  file = fopen(filePath, "w");

  if (!file) {
    printf("ERROR: open file error: %s\n", filePath);
    return NULL;
  }

  cf_HashMapSP_set(&self->map, name, file, NULL, NULL);
  return file;
}

void cf_FileLogListener_print(cf_Log_Listener *self, const char *tag, const char *file, const char *func
                  , const unsigned int line, const cf_LogLevel level, const char *msg) {
  FILE *f;
  char *timeStr;
  time_t now;
  int len;

  f = cf_FileLogListener_getFile((cf_FileLogListener *)self, tag);
  if (!f) return;

  time(&now);
  timeStr = ctime(&now);
  len = strlen(timeStr);
  if (timeStr[len-1] == '\n') {
    timeStr[len-1] = 0;
  }

  fprintf(f, "[%s;%s:%c(%s,%d)] %s\n", timeStr, tag, cf_LogLevel_str[level][0], func, line, msg);
  CF_UNUSED(file);
}

void cf_FileLogListener_make(cf_FileLogListener *self, const char *path) {
  cf_HashMapSP_make(&self->map, 50);
  self->path = path;
  self->super.id = -1;
  self->super.level = cf_LogLevel_debug;
  self->super.func = cf_FileLogListener_print;
}

void cf_FileLogListener_dispose(cf_FileLogListener *self) {
  cf_HashMapSPIterator iter;

  const char *key2;
  FILE *value2;
  cf_Error err;

  cf_HashMapSP_createIterator(&self->map, &iter);
  while (!(err=cf_HashMapSPIterator_next(&iter))) {
    cf_HashMapSPIterator_get(&iter, &key2, (void**)&value2);
    fclose(value2);
  }

  cf_HashMapSP_dispose(&self->map);
}
