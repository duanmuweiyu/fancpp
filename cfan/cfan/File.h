/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-6  Jed Young  Creation
 */

#ifndef _CF_FILE_H_
#define _CF_FILE_H_

#include "cfan/FileStream.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

CF_BEGIN

#define CF_PATH_MAX 255

typedef struct cf_File_ {
  char path[CF_PATH_MAX];
  bool isDir;
  bool exists;
  size_t size;
  time_t mtime;//modified time
} cf_File;

inline void cf_File_make(cf_File *self, char *path) {
  strncpy(self->path, path, CF_PATH_MAX);
  self->exists = false;
  self->path[CF_PATH_MAX] = 0;
}

cf_Error cf_File_loadInfo(cf_File *self);

inline size_t cf_File_size(cf_File *self) {
  return self->size;
}

inline bool cf_File_isDir(cf_File *self) {
  return self->isDir;
}

cf_Error cf_File_createDir(cf_File *self);

cf_Error CF_File_delete(cf_File *self);

inline time_t cf_File_mtime(cf_File *self) {
  return self->mtime;
}

cf_Error cf_File_inputStream(cf_File *self, cf_FileInputStream *in);

cf_Error cf_File_outputStream(cf_File *self, cf_FileOutputStream *out, bool append);

/*************************************************************************
 * Dir
 */

typedef struct cf_DirIterator_ {
  void *first;
  void *second;
} cf_DirIterator;

cf_Error cf_DirIterator_make(cf_DirIterator *self, const char *path);
const char* cf_DirIterator_next(cf_DirIterator *self);
void cf_DirIterator_dispose(cf_DirIterator *self);

CF_END

#endif
