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

#include <stdarg.h>
#include <stdio.h>

/**
 * increase capacity
 */
cf_Error cf_StrBuf_reserver_(cf_StrBuf *self, const long addSize) {
  void *tmp;
  size_t newCapacity = self->capacity * 2 + addSize;

  CF_ENTRY_FUNC
  cf_assert(self);

  if (self->buffer == self->array) {
    tmp = cf_malloc(newCapacity);
  } else {
    tmp = cf_realloc(self->buffer, newCapacity);
  }
  if (!tmp) {
    CF_EXIT_FUNC
    return cf_Error_alloc;
  }

  self->buffer = (char*)tmp;
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

  memcpy(self->buffer + self->size, str, size);
  self->size += size;
  *(self->buffer + self->size) = '\0';

  CF_EXIT_FUNC
  return cf_Error_ok;
}

cf_Error cf_StrBuf_printf(cf_StrBuf *self, size_t size, char *format, ...) {
  va_list args;
  cf_Error err;
  int rc;
  CF_ENTRY_FUNC
  va_start(args, format);

  if (self->capacity <= size) {
    err = cf_StrBuf_reserver_(self, size);
    if (err) { va_end(args); CF_EXIT_FUNC return err; }
  }

  rc = vsnprintf(self->buffer, size, format, args);

  va_end(args);
  if (rc <0) { CF_EXIT_FUNC return cf_Error_unknow; }

  self->size += rc;
  CF_EXIT_FUNC
  return cf_Error_ok;
}
