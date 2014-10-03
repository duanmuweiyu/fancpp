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
#include "cfan/Object.h"

#include <string.h>
#include <stdio.h>

CF_BEGIN

#define cf_Str_defaultBufferCapacity 32

typedef enum cf_StrBufType_ {
  cf_StrBufType_constRef, //pointer to string const
  cf_StrBufType_dynamicRef, //pointer to heap string
  cf_StrBufType_ownMem //pointer to self buffer
} cf_StrBufType;

/**
 * String Buffer
 *
 */
typedef struct cf_StrBuf_ {
  cf_Object super;
  cf_StrBufType type;
  size_t    size;
  size_t    capacity;
  char *pointer;
  char array[cf_Str_defaultBufferCapacity];
  //...more buffer at here
} cf_StrBuf;

/*========================================================================
 * constructor
 */
/**
 * make a default own memory Str in stack
 */
void cf_StrBuf_makeDefault(cf_StrBuf *self);

/**
 * make own memory Str with capcity is strCapacity+1
 */
cf_StrBuf *cf_StrBuf_makeSize(size_t strCapacity);

/**
 * convenience for cf_StrBuf_makeSize and init by s
 */
cf_StrBuf *cf_StrBuf_makeCopy(const char *s);

/**
 * make static Str ref to s
 */
void cf_StrBuf_makeConstRef(cf_StrBuf *self, const char *s);

/*========================================================================
 * Base method
 */
/**
 * return string size
 *
 */
static inline size_t cf_StrBuf_size(cf_StrBuf *self) {
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
static inline char *cf_StrBuf_str(cf_StrBuf *self) {
  cf_assert(self);
  return self->pointer;
}

/**
 * return a copyed free str
 */
char *cf_StrBuf_detach(cf_StrBuf *self);

/**
 * free resource
 */
static inline void cf_StrBuf_dispose(cf_StrBuf *self) {
  cf_assert(self);
  if (self->type != cf_StrBufType_dynamicRef) return;
  cf_free(self->pointer);
}


/*========================================================================
 * Modify method
 */

/**
 * remove last char
 */
static inline cf_Error cf_StrBuf_removeLast(cf_StrBuf *self) {
  cf_assert(self);
  if (self->size == 0) return cf_Error_eof;
  self->size--;
  *(self->pointer + self->size) = '\0';
  return cf_Error_ok;
}

/**
 * remove char at index.
 */
static inline void cf_StrBuf_remove(cf_StrBuf *self, size_t index) {
  cf_assert(self);
  cf_assert(index < self->size);
  memmove(self->pointer+index, self->pointer+index+1, 1);
}

/**
 * set char at index.
 */
static inline void cf_StrBuf_set(cf_StrBuf *self, size_t index, char c) {
  cf_assert(self);
  cf_assert(index < self->size);
  self->pointer[index] = c;
}

/**
 * get char at index.
 */
static inline void cf_StrBuf_get(cf_StrBuf *self, size_t index, char *c) {
  cf_assert(self);
  cf_assert(index < self->size);
  *c = self->pointer[index];
}

/**
 * get sub string at range.
 */
static inline void cf_StrBuf_sub(cf_StrBuf *self, size_t offset, int size, char *out) {
  cf_assert(self);
  cf_assert(offset+size <= self->size);
  memcpy(self->pointer + offset, out, size);
}

/**
 * clear the content, set size to zero.
 */
static inline void cf_StrBuf_clear(cf_StrBuf *self) {
  cf_assert(self);
  self->size = 0;
}

/**
 * print format
 */
cf_Error cf_StrBuf_printf(cf_StrBuf *self, size_t hintBuffersize, const char *format, ...);

CF_END

#endif

