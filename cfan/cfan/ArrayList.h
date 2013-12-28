/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _cf_ArrayListLIST_H_
#define _cf_ArrayListLIST_H_

#include "cfan/Error.h"
#include "cfan/Memory.h"
#include "cfan/Trace.h"
#include "cfan/Util.h"

#include <string.h>

CF_BEGIN

/**
 * Array represents an liner sequence indexed by an int.
 * is a dynamic c array lick C++ vector.
 * supports fast random access to the elements.
 *
 */
typedef struct cf_ArrayList_ {
  unsigned int elemSize;
  size_t    size;        //current number of items
  size_t    capacity;    //The number of items can hold without allocating more memory.
  char *data;
} cf_ArrayList;

/**
 * constructor
 *
 */
static inline cf_Error cf_ArrayList_make(cf_ArrayList *self, size_t size, size_t capacity, const unsigned int elemSize) {
  CF_ENTRY_FUNC
  *((unsigned int*)(&self->elemSize)) = elemSize;
  self->size = size;
  self->capacity = capacity;
  self->data = (char*)cf_malloc(capacity * elemSize);
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
#define cf_ArrayList_size(self) ((self)->size)

#define cf_ArrayList_get_(self, index )\
  ((char*)(self)->data + ((index) * (self)->elemSize))
/**
 * get element pointer by index
 *
 */
static inline void *cf_ArrayList_get(cf_ArrayList *self, size_t index) {
  cf_assert(self);
  cf_assert(index < self->size);

  return cf_ArrayList_get_(self, index);
}

/**
 * increase capacity
 */
cf_Error cf_ArrayList_reserver_(cf_ArrayList *self);

/**
 * add to back
 *
 */
static inline cf_Error cf_ArrayList_add(cf_ArrayList *self, void *elem) {
  cf_Error err;

  CF_ENTRY_FUNC

  cf_assert(self);
  cf_assert(elem);

  if (self->size == self->capacity) {
    err = cf_ArrayList_reserver_(self);
    if (err) { CF_EXIT_FUNC return err; }
  }
  cf_check(self->data);

  cf_memcpy((char*)self->data + (self->size * self->elemSize), elem, self->elemSize);
  self->size++;

  CF_EXIT_FUNC
  return cf_Error_ok;
}

/**
 * Remove element at index.
 * Before removed must free the element resource by yourself.
 *
 */
static inline void cf_ArrayList_remove(cf_ArrayList *self, const unsigned int index) {
  memmove(self->data + (index * self->elemSize)
          , self->data + ((index+1) * self->elemSize), self->elemSize);
  self->size--;
}

/**
 * destroy content
 *
 */
static inline void cf_ArrayList_dispose(cf_ArrayList *self) {
  CF_ENTRY_FUNC
  cf_assert(self);
  cf_free(self->data);
  CF_EXIT_FUNC
}

CF_END

#endif

