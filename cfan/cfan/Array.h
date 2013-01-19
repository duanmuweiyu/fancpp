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

#include "cfan/macro.h"
#include "cfan/Error.h"
#include "cfan/Memory.h"

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
inline void *cf_Array_get(cf_Array *self, const unsigned int index) {
  cf_assert(self);
  cf_assert(index >= 0 && index < self->size);

  return (char*)self->data + (index * self->elemSize);
}

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
  cf_assert(self);
  cf_free(self->data);
  CF_EXIT_FUNC
}

/*************************************************************************
 * sort
 */

inline void cf_Array_swap(cf_Array *self, int i, int j) {
  int n = self->elemSize - 1;
  char *s1 = (char *)cf_Array_get(self, i);
  char *s2 = (char *)cf_Array_get(self, j);
  char t;
  for (; n != -1; --n) {
    t = s1[n];
    s1[n] = s2[n];
    s2[n] = t;
  }
}

/*
* macro:
* cmpFunc
*/

#define cf_Array_sortTemplate(Array)\
\
void Array##_quickSort(cf_Array *self, int left, int right);\
\
inline void Array##_qsort(cf_Array *self) {\
  Array##_quickSort(self, 0, self->size-1);\
}\
\
inline long Array##_bsearch(cf_Array *self, void *elem) {\
  int cond;\
  int low, high, mid;\
  int n;\
  CF_ENTRY_FUNC\
  n = self->size;\
  low = 0;\
  high = n - 1;\
  while (low <= high) {\
    mid = (low+high) / 2;\
    if ((cond = cmopFunc(elem, cf_Array_get(self, mid))) < 0) {\
      high = mid - 1;\
    } else if (cond > 0) {\
      low = mid + 1;\
    } else {\
      CF_EXIT_FUNC\
      return mid;\
    }\
  }\
  CF_EXIT_FUNC\
  return -1;\
}\

#define cf_Array_sortTemplate_impl(Array)\
\
void Array##_quickSort(cf_Array *self, int left, int right) {\
  int i, last;\
  CF_ENTRY_FUNC\
  if (left >= right) {\
    return;\
  }\
  cf_Array_swap(self, left, (left + right) / 2);\
  last = left;\
  for (i = left + 1; i <= right; i++)\
    if (cmopFunc(cf_Array_get(self, i), cf_Array_get(self, left)) < 0)\
      cf_Array_swap(self, ++last, i);\
  cf_Array_swap(self, left, last);\
  Array##_quickSort(self, left, last-1);\
  Array##_quickSort(self, last+1, right);\
  CF_EXIT_FUNC\
}\

#define cmopFunc(v1, v2) (*((int*)(v1)) - *((int*)(v2)))
cf_Array_sortTemplate(cf_ArrayI)
#undef cmopFunc

CF_END

#endif

