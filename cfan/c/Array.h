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

#include "macro.h"
#include "Error.h"
#include "Memory.h"

#include <string.h>

CF_BEGIN

/**
 * Array
 *
 */
typedef struct cf_Array_ {
  size_t    size;        //current number of items
  size_t    capacity;    //The number of items can hold without allocating more memory.
  unsigned int elemSize;
  void *data;
} cf_Array;

/**
 * constructor
 *
 */
inline cf_Error cf_Array_make(cf_Array *self, size_t size, size_t capacity, const unsigned int elemSize) {
  CF_ENTRY_FUNC
  self->size = size;
  self->capacity = capacity;
  self->elemSize = elemSize;
  self->data = cf_malloc(capacity * elemSize);
  if (NULL == self->data) {
    CF_EXIT_FUNC return cf_Error_alloc;
  }
  CF_EXIT_FUNC
  return cf_Error_ok;
}

/**
 * return array size
 *
 */
inline size_t cf_Array_size(cf_Array *self) {
  cf_assert(self);
  return self->size;
}

/**
 * get by index
 *
 */
inline void cf_Array_get(cf_Array *self, const unsigned int index, void **elem) {
  cf_assert(self);
  cf_assert(index >= 0 && index < self->size);
  cf_assert(elem);

  CF_ENTRY_FUNC

  *elem = (char*)self->data + (index * self->elemSize);

  CF_EXIT_FUNC
};

/**
 * increase capacity
 */
cf_Error cf_Array_reserver_(cf_Array *self);

/**
 * add to back
 *
 */
inline cf_Error cf_Array_add(cf_Array *self, void *elem) {
  cf_Error err;

  CF_ENTRY_FUNC

  cf_assert(self);
  cf_assert(elem);

  if (self->size == self->capacity) {
    err = cf_Array_reserver_(self);
    if (err) { CF_EXIT_FUNC return err; }
  }

  memcpy((char*)self->data + (self->size * self->elemSize), elem, self->elemSize);
  self->size++;

  CF_EXIT_FUNC
  return cf_Error_ok;
}

/**
 * destroy content
 *
 */
inline void cf_Array_dispose(cf_Array *self) {
  CF_ENTRY_FUNC
  cf_free(self->data);
  CF_EXIT_FUNC
}


CF_END

#endif

