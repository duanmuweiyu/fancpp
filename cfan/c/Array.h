/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CF_ARRAY_H_
#define _CF_ARRAY_H_

#include "error.h"
#include "memory.h"

#include <string.h>

CF_BEGIN

/**
 * Array
 *
 */
typedef struct _cf_Array {
  size_t    size;
  size_t    capacity;
  const unsigned int elemSize;
  void *data;
} cf_Array;

/**
 * get by index
 *
 */
inline cf_Error cf_Array_get(cf_Array *self, const unsigned int index, void **elem) {
  cf_returnErrorIf(!self, cf_Error_null);
  cf_returnErrorIf(index < 0 || index >= self->size, cf_Error_index);
  cf_returnErrorIf(!elem, cf_Error_null);

  CF_ENTRY_FUNC

  *elem = (char*)self->data + (index * self->elemSize);

  CF_EXIT_FUNC
  return cf_Error_ok;
};

cf_Error cf_Array_reserver_(cf_Array *self);

/**
 * add to back
 *
 */
inline cf_Error cf_Array_add(cf_Array *self, void *elem) {
  cf_Error err;

  CF_ENTRY_FUNC

  cf_returnErrorIf(!self, cf_Error_null);
  cf_returnErrorIf(!elem, cf_Error_null);

  if (self->size == self->capacity) {
    err = cf_Array_reserver_(self);
    if (err) { CF_EXIT_FUNC return err; }
  }

  memcpy((char*)self->data + (self->size * self->elemSize), elem, self->elemSize);

  CF_EXIT_FUNC
  return cf_Error_ok;
}

/**
 * destroy content
 *
 */
inline void cf_Array_destroy(cf_Array *self) {
  CF_ENTRY_FUNC
  cf_free(self->data);
  CF_EXIT_FUNC
}


CF_END

#endif

