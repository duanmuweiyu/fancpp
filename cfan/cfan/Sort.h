/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CF_SORT_H_
#define _CF_SORT_H_

#include "cfan/Error.h"
#include "cfan/Memory.h"

#include <string.h>

CF_BEGIN

/*************************************************************************
 * sort
 */
#define cf_SortTemplate(Array, T) \
\
/**\
 * Array qsort and bsearch template macro.\
 * need macro:\
 *   cmpFunc\
 */\
\
void Array##_quickSort(T *self, int left, int right, T *swapBuffer);\
/**\
 * @param swapBuffer is one element size temp space.\
 */\
static inline void Array##_insertSort(T *self, int left, int right){\
  int j;\
  T swapBuffer;\
  for (;left < right; left++) {\
    swapBuffer = self[left+1];\
    j = left;\
    while (j>-1 && (cf_cmopFunc(swapBuffer, self[j]) < 0)) {\
      self[j+1] = self[j];\
      --j;\
    }\
    self[j+1] = swapBuffer;\
  }\
}\
\
static inline void Array##_sort(T *self, size_t size) {\
  T swapBuffer;\
  if (size < 2) return;\
  Array##_quickSort(self, 0, size-1, &swapBuffer);\
}\
\
static inline long Array##_bsearch(T *self, size_t size, T *elem) {\
  int cond;\
  int low, high, mid;\
  int n;\
  CF_ENTRY_FUNC\
  n = size;\
  low = 0;\
  high = n - 1;\
  while (low <= high) {\
    mid = (low+high) / 2;\
    if ((cond = cf_cmopFunc(*elem, self[mid])) < 0) {\
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

/*************************************************************************
 * Array sort template implemention macro.
 * need macro:
 *   cmpFunc
 */
#define cf_SortTemplate_impl(Array, T) \
\
void Array##_quickSort(T *self, int low, int high, T *temp) {\
  int i = low, j = high;\
  CF_ENTRY_FUNC\
  cf_assert(low < high);\
  \
  /*if too small using insert sort*/\
  if (high - low < 30) {\
    Array##_insertSort(self, low, high);\
    CF_EXIT_FUNC\
    return;\
  }\
  *temp = self[low];\
\
  while (i < j) {\
    while (i<j && cf_cmopFunc(*temp, self[j]) <= 0 ) --j;\
    if (i < j) {\
      self[i] = self[j];\
      ++i;\
    }\
\
    while (i<j && cf_cmopFunc(self[i], *temp) < 0) ++i;\
    if (i < j) {\
      self[j] = self[i];\
      --j;\
    }\
  }\
  self[i] = *temp;\
  if (low < i-1) {\
    Array##_quickSort(self, low, i-1, temp);\
  }\
  if (i+1 < high) {\
    Array##_quickSort(self, j+1, high, temp);\
  }\
  CF_EXIT_FUNC\
}\

/**
 * define int array
 */
#define cf_cmopFunc(v1, v2) ((v1) - (v2))
cf_SortTemplate(cf_ArrayI, int)
#undef cf_cmopFunc

CF_END

#endif

