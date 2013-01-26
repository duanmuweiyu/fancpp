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
inline cf_Error Array##_make(Array *self, size_t size, size_t capacity) {\
  CF_ENTRY_FUNC\
  self->size = size;\
  self->capacity = capacity;\
  self->data = (T*)cf_malloc(capacity * sizeof(T));\
  if (NULL == self->data) {\
    CF_EXIT_FUNC return cf_Error_alloc;\
  }\
  CF_EXIT_FUNC\
  return cf_Error_ok;\
}\
\
/**\
 * return array size\
 *\
 */\
inline size_t Array##_size(Array *self) {\
  return self->size;\
}\
/**\
 * get element pointer by index\
 *\
 */\
inline T *Array##_get(Array *self, size_t index) {\
  cf_assert(self);\
  cf_assert(index < self->size);\
\
  return self->data + index;\
}\
\
/**\
 * get element copy by index\
 *\
 */\
inline T Array##_getCopy(Array *self, size_t index) {\
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
inline cf_Error Array##_add(Array *self, T *elem) {\
  cf_Error err;\
\
  CF_ENTRY_FUNC\
\
  cf_assert(self);\
  cf_assert(elem);\
\
  if (self->size == self->capacity) {\
    err = Array##_reserver_(self);\
    if (err) { CF_EXIT_FUNC return err; }\
  }\
\
  self->data[self->size] = *elem;\
  self->size++;\
\
  CF_EXIT_FUNC\
  return cf_Error_ok;\
}\
inline cf_Error Array##_addCopy(Array *self, T elem) {\
  cf_Error err;\
  CF_ENTRY_FUNC\
  if (self->size == self->capacity) {\
    err = Array##_reserver_(self);\
    if (err) { CF_EXIT_FUNC return err; }\
  }\
\
  self->data[self->size] = elem;\
  self->size++;\
\
  CF_EXIT_FUNC\
  return cf_Error_ok;\
}\
\
/**\
 * Remove element at index.\
 * Before removed must free the element resource by yourself.\
 *\
 */\
inline void Array##_remove(Array *self, const unsigned int index) {\
  memmove(self->data + (index)\
          , self->data + ((index+1)), sizeof(T));\
  self->size--;\
}\
\
/**\
 * destroy content\
 *\
 */\
inline void Array##_dispose(Array *self) {\
  CF_ENTRY_FUNC\
  cf_assert(self);\
  cf_free(self->data);\
  CF_EXIT_FUNC\
}\

/*************************************************************************
 * Array sort template implemention macro.
 * need macro:
 *   cmpFunc
 */
#define cf_ArrayTemplate_impl(Array, T) \
\
cf_Error Array##_reserver_(Array *self) {\
  void *tmp;\
  size_t newCapacity;\
  CF_ENTRY_FUNC\
  cf_assert(self);\
  if (self->capacity > 1000000) {\
    newCapacity = ((self->capacity * 3) / 2 + 1);\
  } else {\
    newCapacity = (self->capacity * 2 + 1);\
  }\
\
  tmp = cf_realloc(self->data, newCapacity * sizeof(T));\
  if (!tmp) {\
    CF_EXIT_FUNC\
    return cf_Error_alloc;\
  }\
\
  self->data = (T*)tmp;\
  self->capacity = newCapacity;\
\
  CF_EXIT_FUNC\
  return cf_Error_ok;\
}\

/**
 * define int array
 */
cf_ArrayTemplate(cf_ArrayI, int)
cf_ArrayTemplate(cf_ArrayP, void*)

CF_END

#endif

