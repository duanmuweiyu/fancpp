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

cf_Error cf_Cache_make(cf_Cache *self, unsigned long capacity, cf_CacheOnRemove onRemove
    , cf_HashMapPPHashFunc hashFunc, cf_HashMapPPCompFunc compFunc) {
  cf_Error err;
  err = cf_HashMapPP_make(&self->map, capacity);
  if (err) return err;
  self->map.hashFunc = hashFunc;
  self->map.compFunc = compFunc;

  cf_CacheList_make(&self->list, capacity);
  self->capacity = capacity;
  self->onRemove = onRemove;
  self->size = 0;

  return cf_Error_ok;
}

void cf_Cache_dispose(cf_Cache *self) {
  cf_CacheElem *elem;
  elem = (cf_CacheElem *)cf_LinkedList_first(&self->list.super);
  while (elem != (cf_CacheElem *)cf_LinkedList_end(&self->list.super)) {
    self->onRemove(elem->key, elem->value);
    elem = (cf_CacheElem *)elem->super.next;
  }

  cf_HashMapPP_dispose(&self->map);
  cf_CacheList_dispose(&self->list);
}

void cf_Cache_keepClear_(cf_Cache *self) {
  cf_CacheElem *elem;
  while (self->size > self->capacity) {
    elem = (cf_CacheElem *)cf_LinkedList_first(&self->list.super);
    if (elem == (cf_CacheElem *)cf_LinkedList_end(&self->list.super)) return;

    cf_LinkedList_remove(&self->list.super, &elem->super);
    cf_HashMapPP_remove(&self->map, elem->key, NULL, NULL);
    self->size--;

    self->onRemove(elem->key, elem->value);
    cf_MemoryPool_free(&self->list.allocator, elem);
  }
}

cf_Error cf_Cache_get(cf_Cache *self, const void *key, const void **oldKey, void **val) {
  cf_Error err;
  cf_CacheElem *elem;

  err = cf_HashMapPP_get(&self->map, key, oldKey, (void**)&elem);
  if (err) return err;

  cf_assert(elem);
  //elem->key = key;
  cf_LinkedList_remove(&self->list.super, &elem->super);
  cf_LinkedList_add(&self->list.super, &elem->super);

  *val = elem->value;

  return err;
}

cf_Error cf_Cache_set(cf_Cache *self, const void *key, void *val) {
  cf_Error err;
  cf_CacheElem *elem;

  elem = (cf_CacheElem*)cf_MemoryPool_alloc(&self->list.allocator);
  elem->key = key;
  elem->value = val;
  cf_LinkedList_add(&self->list.super, &elem->super);

  err = cf_HashMapPP_set(&self->map, key, elem, NULL, NULL);
  self->size++;

  cf_Cache_keepClear_(self);

  return err;
}
