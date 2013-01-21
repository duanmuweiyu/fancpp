/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#include "cfan/Array.h"

cf_Error cf_Array_reserver_(cf_Array *self) {
  void *tmp;
  size_t newCapacity = self->capacity * 2 + 4;

  CF_ENTRY_FUNC
  cf_assert(self);

  cf_check(self->data);
  tmp = cf_realloc(self->data, newCapacity * self->elemSize);
  cf_check(self->data);
  if (!tmp) {
    CF_EXIT_FUNC
    return cf_Error_alloc;
  }

  self->data = tmp;
  self->capacity = newCapacity;

  CF_EXIT_FUNC
  return cf_Error_ok;
}

#define cmopFunc(v1, v2) (*((int*)(v1)) - *((int*)(v2)))
cf_Array_sortTemplate_impl(cf_ArrayI)
#undef cmopFunc
