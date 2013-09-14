/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-9-14  Jed Young  Creation
 */

#include "cfan/Cache.h"

cf_Error cf_Cache_make(cf_Cache *self, unsigned long capacity, cf_CacheOnRemove onRemove) {
  cf_Error err;
  err = cf_HashMapSP_make(&self->map, capacity);
  if (err) return err;

  cf_CacheList_make(&self->list, capacity);
  self->capacity = capacity;
  self->onRemove = onRemove;
  self->size = 0;

  return cf_Error_ok;
}

void cf_Cache_dispose(cf_Cache *self) {
  cf_CacheElem *elem;
  elem = self->list.head;
  while (elem) {
    self->onRemove(elem->key, elem->value);
    elem = elem->next;
  }

  cf_HashMapSP_dispose(&self->map);
  cf_CacheList_dispose(&self->list);
}

void cf_Cache_keepClear_(cf_Cache *self) {
  cf_CacheElem *elem;
  while (self->size > self->capacity) {
    elem = self->list.head;
    if (elem == NULL) return;

    cf_CacheList_remove(&self->list, elem);
    cf_HashMapSP_remove(&self->map, elem->key, NULL, NULL);
    self->size--;

    self->onRemove(elem->key, elem->value);
    cf_MemoryPool_free(&self->list.allocator, elem);
  }
}

cf_Error cf_Cache_get(cf_Cache *self, const char *key, const char **oldKey, void **val) {
  cf_Error err;
  cf_CacheElem *elem;

  err = cf_HashMapSP_get(&self->map, key, oldKey, (void**)&elem);
  if (err) return err;

  cf_assert(elem);
  elem->key = key;
  cf_CacheList_remove(&self->list, elem);
  cf_CacheList_add(&self->list, elem);

  *val = elem->value;

  return err;
}

cf_Error cf_Cache_set(cf_Cache *self, const char *key, void *val) {
  cf_Error err;
  cf_CacheElem *elem;

  elem = (cf_CacheElem*)cf_MemoryPool_alloc(&self->list.allocator);
  elem->key = key;
  elem->value = val;
  cf_CacheList_add(&self->list, elem);

  err = cf_HashMapSP_set(&self->map, key, elem, NULL, NULL);
  self->size++;

  cf_Cache_keepClear_(self);

  return err;
}
