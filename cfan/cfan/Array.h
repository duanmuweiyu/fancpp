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

#include "cfan/Error.h"
#include "cfan/Memory.h"
#include "cfan/Trace.h"

#include <string.h>

CF_BEGIN

/**
 * Array represents an liner sequence indexed by an int.
 * is a dynamic c array lick C++ vector.
 * supports fast random access to the elements.
 *
 */
#define cf_ArrayTemplate(Array, T) \
typedef struct Array##_ {\
  size_t    size;        /*current number of items*/\
  size_t    capacity;    /*The number of items can hold without allocating more memory.*/\
  T *data;\
} Array;\
\
/**\
 * constructor\
 *\
 */\
static inline cf_Error Array##_make(Array *self, size_t size, size_t capacity) {\
  self->size = size;\
  self->capacity = capacity;\
  self->data = (T*)cf_malloc(capacity * sizeof(T));\
  if (NULL == self->data) {\
    return cf_Error_alloc;\
  }\
  return cf_Error_ok;\
}\
\
/**\
 * return array size\
 *\
 */\
static inline size_t Array##_size(Array *self) {\
  return self->size;\
}\
/**\
 * get element pointer by index\
 *\
 */\
static inline T *Array##_get(Array *self, size_t index) {\
  cf_assert(self);\
  cf_assert(index < self->size);\
\
  return self->data + index;\
}\
/**\
 * remove last element and return it.\
 * if empty return NULL;\
 */\
static inline T *Array##_pop(Array *self) {\
  if (self->size == 0) {\
    return NULL;\
  }\
  self->size--;\
  return self->data + self->size;\
}\
\
/**\
 * get element copy by index\
 *\
 */\
static inline T Array##_getCopy(Array *self, size_t index) {\
  cf_assert(self);\
  cf_assert(index < self->size);\
\
  return self->data[index];\
}\
\
/**\
 * increase capacity\
 */\
cf_Error Array##_reserver_(Array *self);\
\
/**\
 * add to back\
 *\
 */\
static inline cf_Error Array##_add(Array *self, T *elem) {\
  cf_Error err;\
\
\
  cf_assert(self);\
  cf_assert(elem);\
\
  if (self->size == self->capacity) {\
    err = Array##_reserver_(self);\
    if (err) { return err; }\
  }\
\
  self->data[self->size] = *elem;\
  self->size++;\
\
  return cf_Error_ok;\
}\
static inline cf_Error Array##_addCopy(Array *self, T elem) {\
  cf_Error err;\
  if (self->size == self->capacity) {\
    err = Array##_reserver_(self);\
    if (err) { return err; }\
  }\
\
  self->data[self->size] = elem;\
  self->size++;\
\
  return cf_Error_ok;\
}\
\
/**\
 * Remove element at index.\
 * Before removed must free the element resource by yourself.\
 *\
 */\
static inline void Array##_remove(Array *self, const unsigned int index) {\
  memmove(self->data + (index)\
          , self->data + ((index+1)), sizeof(T));\
  self->size--;\
}\
\
/**\
 * destroy content\
 *\
 */\
static inline void Array##_dispose(Array *self) {\
  CF_ENTRY_FUNC\
  cf_assert(self);\
  cf_free(self->data);\
  CF_EXIT_FUNC\
}\

/*======================================================================*/
/**
 * Array template implemention macro.
 * need macro:
 *   cmpFunc
 */
#define cf_ArrayTemplate_impl(Array, T) \
\
cf_Error Array##_reserver_(Array *self) {\
  void *tmp;\
  size_t newCapacity;\
  cf_assert(self);\
  if (self->capacity > 1000000) {\
    newCapacity = ((self->capacity * 3) / 2 + 1);\
  } else {\
    newCapacity = (self->capacity * 2 + 1);\
  }\
\
  tmp = cf_realloc(self->data, newCapacity * sizeof(T));\
  if (!tmp) {\
    return cf_Error_alloc;\
  }\
\
  self->data = (T*)tmp;\
  self->capacity = newCapacity;\
\
  return cf_Error_ok;\
}\

/**
 * define int array
 */
cf_ArrayTemplate(cf_ArrayI, int)
cf_ArrayTemplate(cf_ArrayP, void*)
cf_ArrayTemplate(cf_ArrayS, char*)

CF_END

#endif

