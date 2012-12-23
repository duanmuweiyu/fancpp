/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CF_STRBUF_H_
#define _CF_STRBUF_H_

#include "macro.h"
#include "Error.h"
#include "Memory.h"

#include <string.h>

CF_BEGIN

/**
 * String Buffer
 *
 */
typedef struct cf_StrBuf_ {
  size_t    size;
  size_t    capacity;
  char array[256];
  char *buffer;
} cf_StrBuf;

inline void cf_StrBuf_make(cf_StrBuf *self) {
  cf_assert(self);
  self->capacity = 256;
  self->buffer = self->array;
  self->size = 0;
}

cf_Error cf_StrBuf_add(cf_StrBuf *self, const char *str, long size);

inline char *cf_StrBuf_str(cf_StrBuf *self) {
  cf_assert(self);
  return self->buffer;
}

inline void cf_StrBuf_destroy(cf_StrBuf *self) {
  cf_assert(self);
  if (self->buffer == self->array) return;
  cf_free(self);
}

CF_END

#endif

