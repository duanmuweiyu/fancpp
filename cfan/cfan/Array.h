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
typedef struct cf_Array_ {
  const unsigned int elemSize;
  size_t    size;        //current number of items
  size_t    capacity;    //The number of items can hold without allocating more memory.
  char *data;
} cf_Array;

/**
 * constructor
 *
 */
inline cf_Error cf_Array_make(cf_Array *self, size_t size, size_t capacity, const unsigned int elemSize) {
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
#define cf_Array_size(self) ((self)->size)

#define cf_Array_get_(self, index )\
  ((char*)(self)->data + ((index) * (self)->elemSize))
/**
 * get element pointer by index
 *
 */
inline void *cf_Array_get(cf_Array *self, size_t index) {
  cf_assert(self);
  cf_assert(index < self->size);

  return cf_Array_get_(self, index);
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
  cf_check(self->data);

  memcpy((char*)self->data + (self->size * self->elemSize), elem, self->elemSize);
  self->size++;

  CF_EXIT_FUNC
  return cf_Error_ok;
}

/**
 * Remove element at index.
 * Before removed must free the element resource by yourself.
 *
 */
inline void cf_Array_remove(cf_Array *self, const unsigned int index) {
  memmove(self->data + (index * self->elemSize)
          , self->data + ((index+1) * self->elemSize), self->elemSize);
  self->size--;
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

/**
 * change two element position
 * @param buffer is one element size temp space.
 *
 */
inline void cf_Array_swap(cf_Array *self, int i, int j, void *swapBuffer) {
  memcpy(swapBuffer, cf_Array_get_(self, i), self->elemSize);
  memcpy(cf_Array_get_(self, i), cf_Array_get_(self, j), self->elemSize);
  memcpy(cf_Array_get_(self, j), swapBuffer, self->elemSize);
}

/**
 * Array qsort and bsearch template macro.
 * need macro:
 *   cmpFunc
 */
#define cf_Array_sortTemplate(Array)\
\
void Array##_quickSort(cf_Array *self, int left, int right, void *swapBuffer);\
/**\
 * @param buffer is one element size temp space.\
 */\
inline void Array##_insertSort(cf_Array *self, int left, int right, void *buffer){\
  int j;\
  for (;left < right; left++) {\
    memcpy(buffer, cf_Array_get_(self, left+1), self->elemSize);\
    j = left;\
    while (j>-1 && (cmopFunc(buffer, cf_Array_get_(self, j)) < 0)) {\
      memcpy(cf_Array_get_(self, j+1), cf_Array_get_(self, j), self->elemSize);\
      --j;\
    }\
    memcpy(cf_Array_get_(self, j+1), buffer, self->elemSize);\
  }\
}\
\
inline cf_Error Array##_sort(cf_Array *self) {\
  void *swapBuffer;\
  swapBuffer = cf_malloc(self->elemSize);\
  if (swapBuffer == NULL) {\
    return cf_Error_alloc;\
  }\
  Array##_quickSort(self, 0, self->size-1, swapBuffer);\
  cf_free(swapBuffer);\
  return cf_Error_ok;\
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

/**
 * Array sort template implemention macro.
 * need macro:
 *   cmpFunc
 */
#define cf_Array_sortTemplate_impl(Array)\
\
void Array##_quickSort(cf_Array *self, int left, int right, void *swapBuffer) {\
  int i, last;\
  CF_ENTRY_FUNC\
  if (left >= right) {\
    return;\
  }\
  /*if too small using insert sort*/\
  if (self->size < 20) {\
    Array##_insertSort(self, left, right, swapBuffer);\
    return;\
  }\
  cf_Array_swap(self, left, (left + right) / 2, swapBuffer);\
  last = left;\
  for (i = left + 1; i <= right; i++)\
    if (cmopFunc(cf_Array_get(self, i), cf_Array_get(self, left)) < 0)\
      cf_Array_swap(self, ++last, i, swapBuffer);\
  cf_Array_swap(self, left, last, swapBuffer);\
  Array##_quickSort(self, left, last-1, swapBuffer);\
  Array##_quickSort(self, last+1, right, swapBuffer);\
  CF_EXIT_FUNC\
}\

/**
 * define int array
 */
#define cmopFunc(v1, v2) (*((int*)(v1)) - *((int*)(v2)))
cf_Array_sortTemplate(cf_ArrayI)
#undef cmopFunc

CF_END

#endif

