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

#define CF_PATH_MAX 256

/**
 * Represents File or Dir
 *
 */
typedef struct cf_File_ {
  char path[CF_PATH_MAX];
  bool isDir;
  bool exists;
  size_t size;
  time_t mtime;//modified time
} cf_File;

/**
 * constructor
 *
 */
static inline void cf_File_make(cf_File *self, char *path) {
  strncpy(self->path, path, CF_PATH_MAX);
  self->exists = false;
  self->path[CF_PATH_MAX-1] = 0;
}

/**
 * load file infomation.
 * Must call this before get the file info.
 *
 */
cf_Error cf_File_loadInfo(cf_File *self);

/**
 * return file size.
 *
 */
static inline size_t cf_File_size(cf_File *self) {
  return self->size;
}

/**
 * is directory
 *
 */
static inline bool cf_File_isDir(cf_File *self) {
  return self->isDir;
}

/**
 * create directory
 *
 */
cf_Error cf_File_createDir(cf_File *self);

/**
 * remove file
 *
 */
cf_Error CF_File_delete(cf_File *self);

/**
 * get file modify time
 *
 */
static inline time_t cf_File_mtime(cf_File *self) {
  return self->mtime;
}

/**
 * get file input stream
 *
 */
cf_Error cf_File_inputStream(cf_File *self, cf_FileInputStream *in);

/**
 * get file output stream
 *
 */
cf_Error cf_File_outputStream(cf_File *self, cf_FileOutputStream *out, bool append);

/*========================================================================
 * Dir
 */

typedef struct cf_DirIterator_ {
  void *first;
  void *second;
} cf_DirIterator;

/**
 * make directory iterator
 *
 */
cf_Error cf_DirIterator_make(cf_DirIterator *self, const char *path);

/**
 * find next file
 *
 */
const char* cf_DirIterator_next(cf_DirIterator *self);

/**
 * destroy direcotry iterator
 *
 */
void cf_DirIterator_dispose(cf_DirIterator *self);

CF_END

#endif
