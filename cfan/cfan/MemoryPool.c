/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-2-18  Jed Young  Creation
 */

#include "cfan/MemoryPool.h"
#include "cfan/Trace.h"
#include <stdio.h>

/**
 * a magic code.
 */
#define cf_MemoryPool_headCheckCode 0xA9C5

void cf_MemoryPool_make(cf_MemoryPool *self, size_t objSize, size_t objCount) {
  size_t i;
  cf_MemoryPoolElem *last;
  cf_MemoryPoolElem *item;
  CF_ENTRY_FUNC
  cf_assert(self);

  self->itemSize = CF_ALIGN(objSize+sizeof(cf_MemoryPoolElem));
  self->buffer = (char *)cf_checkedMalloc((self->itemSize) * objCount);
  self->size = objCount;
  last = (cf_MemoryPoolElem *)self->buffer;
  last->isBuffered = true;
  last->next = NULL;
  last->name = NULL;
  last->checkCode = cf_MemoryPool_headCheckCode;
  self->list = last;
  for (i=1; i<objCount; ++i) {
    item = (cf_MemoryPoolElem *)(self->buffer + i*self->itemSize);
    item->isBuffered = true;
    item->next = NULL;
    item->name = NULL;
    item->checkCode = cf_MemoryPool_headCheckCode;
    last->next = item;
    last = item;
  }
  self->allocCount = 0;
  CF_EXIT_FUNC
}

static inline void cf_MemoryPool_doCheck_(cf_MemoryPoolElem *item) {
  if (item->checkCode != cf_MemoryPool_headCheckCode) {
    cf_abort("bad heap, front overflow.");
  }
}

void *cf_MemoryPool_alloc_(cf_MemoryPool *self, const char *name) {
  cf_MemoryPoolElem *item;
  CF_ENTRY_FUNC
  cf_assert(self);
  self->allocCount++;
  if (self->list != NULL) {
    item = self->list;
    cf_MemoryPool_doCheck_(item);
    self->list = self->list->next;
    item->name = name;
    CF_EXIT_FUNC
    return item + 1;
  } else {
    item = (cf_MemoryPoolElem *)cf_checkedMalloc(self->itemSize);
    item->isBuffered = false;
    item->name = name;
    item->next = NULL;
    item->checkCode = cf_MemoryPool_headCheckCode;
    CF_EXIT_FUNC
    return item + 1;
  }
}

void cf_MemoryPool_free(cf_MemoryPool *self, void *p) {
  cf_MemoryPoolElem *item;
  cf_MemoryPoolElem *current;
  CF_ENTRY_FUNC
  cf_assert(self);
  cf_assert(p);
  self->allocCount--;
  item = ((cf_MemoryPoolElem *)p) - 1;
  item->next = NULL;
  item->name = NULL;
  cf_MemoryPool_doCheck_(item);
  if (self->list == NULL) {
    self->list = item;
    CF_EXIT_FUNC
    return;
  }
  current = self->list;
  while (current->next != NULL) {
    current = current->next;
  }
  current->next = item;
  item->next = NULL;
  CF_EXIT_FUNC
}

void cf_MemoryPool_dispose(cf_MemoryPool *self) {
  cf_MemoryPoolElem *current;
  cf_MemoryPoolElem *next;
  CF_ENTRY_FUNC
  cf_assert(self);

  if (self->allocCount != 0) {
    cf_abort("some memory not release");
    CF_EXIT_FUNC
    return;
  }

  current = self->list;
  while (current != NULL) {
    cf_MemoryPool_doCheck_(current);
    next = current->next;
    if (!current->isBuffered) {
      cf_free(current);
    }
    current = next;
  }
  cf_free(self->buffer);
  CF_EXIT_FUNC
}
