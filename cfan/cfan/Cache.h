/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-9-14  Jed Young  Creation
 */

#ifndef _CF_CACHE_H_
#define _CF_CACHE_H_

#include "cfan/LinkedList.h"
#include "cfan/HashMap.h"
#include "cfan/MemoryPool.h"

CF_BEGIN

/*========================================================================
 * Cache Element
 */

/**
 * LinkedList element
 */
typedef struct cf_CacheElem_ {
  cf_LinkedListElem super;
  const void *key;
  void *value;
} cf_CacheElem;

/**
 * LinkedList parent
 */
typedef struct cf_CacheList_ {
  cf_LinkedList super;
  cf_MemoryPool allocator;
} cf_CacheList;

/**
 * default ctor
 */
static inline void cf_CacheList_make(cf_CacheList *self, size_t objCount) {
  cf_LinkedList_make(&self->super);
  cf_MemoryPool_make(&self->allocator, sizeof(cf_CacheElem), objCount);
}

static inline void cf_CacheList_dispose(cf_CacheList *self) {
  cf_LinkedList_freeLinkedElem(&self->super, &self->allocator);
  cf_MemoryPool_dispose(&self->allocator);
}

/*========================================================================
 * Cache
 */

typedef void (*cf_CacheOnRemove)(const void *, void *);

/**
 * Least Recently Used Cache
 */
typedef struct cf_Cache_ {
  unsigned long capacity;
  unsigned long size;
  cf_HashMapPP map;
  cf_CacheList list;
  cf_CacheOnRemove onRemove;
} cf_Cache;

cf_Error cf_Cache_make(cf_Cache *self, unsigned long capacity, cf_CacheOnRemove onRemove
    , cf_HashMapPPHashFunc hashFunc, cf_HashMapPPCompFunc compFunc);
void cf_Cache_dispose(cf_Cache *self);

static inline unsigned long cf_Cahce_size(cf_Cache *self) { return self->size; }

cf_Error cf_Cache_get(cf_Cache *self, const void *key, const void **oldKey, void **val);
cf_Error cf_Cache_set(cf_Cache *self, const void *key, void *val);

CF_END

#endif
