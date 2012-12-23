/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#include "StrBuf.h"

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
  if (self->size + size > self->capacity) {
    err = cf_StrBuf_reserver_(self, size);
    if (err) { CF_EXIT_FUNC return err; }
  }

  memcpy(self->buffer + self->size, str, size);
  self->size += size;
  *(self->buffer + self->size) = '\0';

  CF_EXIT_FUNC
  return cf_Error_ok;
}