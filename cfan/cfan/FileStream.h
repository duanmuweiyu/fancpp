/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-6  Jed Young  Creation
 */

#ifndef _CF_FILESTREAM_H_
#define _CF_FILESTREAM_H_

#include "cfan/Stream.h"
#include <stdio.h>

CF_BEGIN

/*************************************************************************
 * OutputStream
 */

typedef struct cf_FileOutputStreamVTable_ {
  cf_OutputStreamVTable super;
} cf_FileOutputStreamVTable;

typedef struct cf_FileOutputStream_ {
  cf_OutputStream super;
  FILE *file;
} cf_FileOutputStream;

cf_Error cf_FileOutputStream_write(cf_OutputStream *self, char *buf, size_t size);

inline void cf_FileOutputStream_make(cf_FileOutputStream *self, FILE *file) {
  self->file = file;
  CF_VTABLE(cf_OutputStream, self)->write = cf_FileOutputStream_write;
}

/*************************************************************************
 * InputStream
 */
typedef struct cf_FileInputStreamVTable_ {
  cf_InputStreamVTable super;
} cf_FileInputStreamVTable;

typedef struct cf_FileInputStream_ {
  cf_InputStream super;
  FILE *file;
} cf_FileInputStream;

cf_Error cf_FileInputStream_read(cf_InputStream *self, char *buf, size_t size);

inline void cf_FileInputStream_make(cf_FileInputStream *self, FILE *file) {
  self->file = file;
  CF_VTABLE(cf_InputStream, self)->read = cf_FileInputStream_read;
}

CF_END

#endif
