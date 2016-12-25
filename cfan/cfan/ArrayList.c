/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#include "cfan/ArrayList.h"

cf_Error cf_ArrayList_add(cf_ArrayList *self, void *elem) {
  cf_Error err;

  CF_ENTRY_FUNC

  cf_assert(self);
  cf_assert(elem);

  if (self->size == self->capacity) {
    err = cf_ArrayList_reserver_(self);
    if (err) { CF_EXIT_FUNC return err; }
  }
  cf_checkMem(self->data);

  cf_memcpy((char*)self->data + (self->size * self->elemSize), elem, self->elemSize);
  self->size++;

  CF_EXIT_FUNC
  return cf_Error_ok;
}

cf_Error cf_ArrayList_reserver_(cf_ArrayList *self) {
  void *tmp;
  size_t newCapacity = self->capacity * 2 + 4;

  CF_ENTRY_FUNC
  cf_assert(self);

  cf_checkMem(self->data);
  tmp = cf_realloc(self->data, newCapacity * self->elemSize);
  if (!tmp) {
    CF_EXIT_FUNC
    return cf_Error_alloc;
  }
  cf_checkMem(tmp);
  self->data = (char*)tmp;
  self->capacity = newCapacity;

  CF_EXIT_FUNC
  return cf_Error_ok;
}

