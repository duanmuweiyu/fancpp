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

#include "cfan/macro.h"
#include "cfan/Error.h"
#include "cfan/Memory.h"

#include <string.h>
#include <stdio.h>

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

/*************************************************************************
 * Base method
 */

/**
 * constructor
 */
inline void cf_StrBuf_make(cf_StrBuf *self) {
  cf_assert(self);
  self->capacity = 256;
  self->buffer = self->array;
  self->size = 0;
}

/**
 * return string size
 *
 */
inline size_t cf_StrBuf_size(cf_StrBuf *self) {
  cf_assert(self);
  return self->size;
}

/**
 * Append a str to end.
 * @param str buf array
 * @param size buf array size, if -1 take the str until zero-termination
 */
cf_Error cf_StrBuf_add(cf_StrBuf *self, const char *str, long size);

/**
 * To c style string.
 */
inline char *cf_StrBuf_str(cf_StrBuf *self) {
  cf_assert(self);
  return self->buffer;
}

/**
 * free resource
 */
inline void cf_StrBuf_dispose(cf_StrBuf *self) {
  cf_assert(self);
  if (self->buffer == self->array) return;
  cf_free(self->buffer);
}

/*************************************************************************
 * Modify method
 */

/**
 * remove last char
 */
inline cf_Error cf_StrBuf_removeLast(cf_StrBuf *self) {
  cf_assert(self);
  if (self->size == 0) return cf_Error_eof;
  self->size--;
  return cf_Error_ok;
}

/**
 * remove char at index.
 */
inline void cf_StrBuf_remove(cf_StrBuf *self, size_t index) {
  cf_assert(self);
  cf_assert(index >= 0 && index < self->size);
  memmove(self->buffer+index, self->buffer+index+1, 1);
}

/**
 * set char at index.
 */
inline void cf_StrBuf_set(cf_StrBuf *self, size_t index, char c) {
  cf_assert(self);
  cf_assert(index >= 0 && index < self->size);
  self->buffer[index] = c;
}

/**
 * get char at index.
 */
inline void cf_StrBuf_get(cf_StrBuf *self, size_t index, char *c) {
  cf_assert(self);
  cf_assert(index >= 0 && index < self->size);
  *c = self->buffer[index];
}

/**
 * get sub string at range.
 */
inline void cf_StrBuf_sub(cf_StrBuf *self, size_t offset, int size, char *out) {
  cf_assert(self);
  cf_assert(offset >= 0 && offset+size <= self->size);
  memcpy(self->buffer + offset, out, size);
}

/**
 * clear the content, set size to zero.
 */
inline void cf_StrBuf_clear(cf_StrBuf *self) {
  cf_assert(self);
  self->size = 0;
}

/**
 * print format
 */
cf_Error cf_StrBuf_printf(cf_StrBuf *self, int size, char *format, ...);

CF_END

#endif

