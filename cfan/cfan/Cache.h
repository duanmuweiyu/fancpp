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

CF_BEGIN

/*========================================================================
 * Cache Element
 */

/**
 * LinkedList element
 */
typedef struct cf_CacheElem_ {
  const char *key;
  void *value;
  struct cf_CacheElem_ *previous;
  struct cf_CacheElem_ *next;
} cf_CacheElem;

/**
 * LinkedList parent
 */
typedef struct cf_CacheList_ {
  cf_CacheElem *head;
  cf_CacheElem *tail;
  cf_MemoryPool allocator;
} cf_CacheList;

/**
 * default ctor
 */
static inline void cf_CacheList_make(cf_CacheList *self, size_t objCount) {
  self->head = NULL;
  self->tail = NULL;
  cf_MemoryPool_make(&self->allocator, sizeof(cf_CacheElem), objCount);
}

/**
 * define methods
 */
cf_LinkedListTemplate(cf_CacheList, cf_CacheElem)

static inline void cf_CacheList_dispose(cf_CacheList *self) {
  cf_CacheList_dispose_(self, &self->allocator);
  cf_MemoryPool_dispose(&self->allocator);
}

/*========================================================================
 * Cache
 */

typedef void (*cf_CacheOnRemove)(const char*, void *);

/**
 * Cache
 */
typedef struct cf_Cache_ {
  unsigned long capacity;
  unsigned long size;
  cf_HashMapSP map;
  cf_CacheList list;
  cf_CacheOnRemove onRemove;
} cf_Cache;

cf_Error cf_Cache_make(cf_Cache *self, unsigned long capacity, cf_CacheOnRemove onRemove);
void cf_Cache_dispose(cf_Cache *self);

static inline unsigned long cf_Cahce_size(cf_Cache *self) { return self->size; }

cf_Error cf_Cache_get(cf_Cache *self, const char *key, const char **oldKey, void **val);
cf_Error cf_Cache_set(cf_Cache *self, const char *key, void *val);

CF_END

#endif
