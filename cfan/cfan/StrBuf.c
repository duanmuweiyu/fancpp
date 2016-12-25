/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#include "cfan/StrBuf.h"
#include "cfan/Trace.h"
#include "cfan/Util.h"

#include <stdarg.h>
#include <stdio.h>


cf_VTable cf_StrBuf_vtable = { 0 , "cf_StrBuf", (cf_VTableDestructor)cf_StrBuf_dispose };

static inline void initVTable(cf_StrBuf *self) {
  self->super.vtable = &cf_StrBuf_vtable;
}

void cf_StrBuf_makeDefault(cf_StrBuf *self) {
  cf_assert(self);

  self->type = cf_StrBufType_ownMem;
  self->pointer = self->array;
  self->size = 0;
  self->capacity = cf_Str_defaultBufferCapacity;
  self->super.refCount = 0;
  self->array[0] = 0;
  initVTable(self);
}

cf_StrBuf *cf_StrBuf_makeSize(size_t strCapacity) {
  int capacity;
  int allocSize;
  cf_StrBuf *self;

  //min length is cf_Str_defaultBufferCapacity
  if (strCapacity >= cf_Str_defaultBufferCapacity) {
    capacity = strCapacity+1;
    allocSize = sizeof(cf_StrBuf) + (capacity-cf_Str_defaultBufferCapacity);
  } else {
    capacity = cf_Str_defaultBufferCapacity;
    allocSize = sizeof(cf_StrBuf);
  }

  self = (cf_StrBuf *)cf_malloc(allocSize);
  if (self == NULL) {
    return NULL;
  }

  self->array[0] = 0;
  self->type = cf_StrBufType_ownMem;
  self->size = 0;
  self->capacity = capacity;
  self->super.refCount = 0;
  self->pointer = self->array;
  initVTable(self);
  return self;
}

cf_StrBuf *cf_StrBuf_makeCopy(const char *s) {
  int n = strlen(s);
  cf_StrBuf *self = cf_StrBuf_makeSize(n);
  if (self == NULL) {
    return NULL;
  }
  strcpy(self->array, s);
  return self;
}

void cf_StrBuf_makeConstRef(cf_StrBuf *self, const char *s) {
  self->type = cf_StrBufType_constRef;
  self->pointer = (char *)s;
  self->size = strlen(s);
  self->capacity = self->size+1;
  self->super.refCount = 0;
  self->array[0] = 0;
  initVTable(self);
}

/*========================================================================
 * Base method
 */

char *cf_StrBuf_detach(cf_StrBuf *self) {
  char * str;
  cf_assert(self);
  if (self->type == cf_StrBufType_constRef) return self->pointer;
  str = (char*)cf_malloc(self->size+1);
  if (str == NULL) return NULL;
  strcpy(str, self->array);
  return str;
}

/**
 * increase capacity
 */
cf_Error cf_StrBuf_reserver_(cf_StrBuf *self, const long addSize) {
  void *tmp;
  size_t newCapacity;
  CF_ENTRY_FUNC
  cf_assert(self);

  //increase 1.5 if large than 1M.
  if (self->capacity > 1000000) {
    newCapacity = (self->capacity * 3) / 2 + addSize;
  } else {
    newCapacity = self->capacity * 2 + addSize;
  }

  if (self->type == cf_StrBufType_ownMem || self->type == cf_StrBufType_constRef) {
    tmp = cf_malloc(newCapacity);
    self->type = cf_StrBufType_dynamicRef;
    memcpy(tmp, self->pointer, self->size+1);
  } else {
    tmp = cf_realloc(self->pointer, newCapacity);
  }
  if (!tmp) {
    CF_EXIT_FUNC
    return cf_Error_alloc;
  }

  self->pointer = (char*)tmp;
  self->capacity = newCapacity;

  CF_EXIT_FUNC
  return cf_Error_ok;
}

cf_Error cf_StrBuf_add(cf_StrBuf *self, const char *str, long size) {
  cf_Error err;
  CF_ENTRY_FUNC
  cf_assert(self);
  cf_assert(str);

  if (size < 0) {
    size = strlen(str);
  }
  if (self->size + size >= self->capacity) {
    err = cf_StrBuf_reserver_(self, size);
    if (err) { CF_EXIT_FUNC return err; }
  }

  cf_memcpy(self->pointer + self->size, str, size);
  self->size += size;
  *(self->pointer + self->size) = '\0';

  CF_EXIT_FUNC
  return cf_Error_ok;
}

cf_Error cf_StrBuf_printf(cf_StrBuf *self, size_t hintBuffersize, const char *format, ...) {
  va_list args;
  cf_Error err;
  int rc;
  size_t size = hintBuffersize;
  CF_ENTRY_FUNC
  va_start(args, format);

  if (self->capacity - self->size <= size) {
    err = cf_StrBuf_reserver_(self, size);
    if (err) { va_end(args); CF_EXIT_FUNC return err; }
  } else {
    size = self->capacity - self->size;
  }

  rc = vsnprintf(self->pointer, size, format, args);

//windows return -1, unix return expected size
#ifdef CF_WIN
  while (rc < 0) {
    size *= 2;
    if (size > 1E9) break;
    err = cf_StrBuf_reserver_(self, size);
    if (err) { va_end(args); CF_EXIT_FUNC return err; }
    va_end(args);
    va_start(args, format);
    rc = vsnprintf(self->pointer, size, format, args);
  }
#else
  if (rc > (int)(size-1)) {
    size = rc + 1;
    err = cf_StrBuf_reserver_(self, size);
    if (err) { va_end(args); CF_EXIT_FUNC return err; }
    va_end(args);
    va_start(args, format);
    rc = vsnprintf(self->pointer, size, format, args);
  }
#endif

  va_end(args);
  if (rc <0) { CF_EXIT_FUNC return cf_Error_unknow; }

  self->size += rc;
  CF_EXIT_FUNC
  return cf_Error_ok;
}
