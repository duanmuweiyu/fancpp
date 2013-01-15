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

#include "FileStream.h"
#include <stdio.h>
#include <string.h>

CF_BEGIN

#define CF_PATH_MAX 255

typedef struct cf_File_ {
  char path[CF_PATH_MAX];
} cf_File;

inline void cf_File_make(cf_File *self, char *path) {
  strncpy(self->path, path, CF_PATH_MAX);
  self->path[CF_PATH_MAX] = 0;
}

cf_Error cf_File_inputStream(cf_File *self, cf_FileInputStream *in);

cf_Error cf_File_outputStream(cf_File *self, cf_FileOutputStream *out, bool append);

CF_END

#endif
