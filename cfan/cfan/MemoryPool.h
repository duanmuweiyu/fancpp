/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-2-18  Jed Young  Creation
 */

#ifndef _CF_MEMORYPOOL_H_
#define _CF_MEMORYPOOL_H_


#include "cfan/Error.h"
#include "cfan/Memory.h"

CF_BEGIN

/**
 * Memory Pool Item
 *
 */
typedef struct cf_MemoryPoolElem_ {
  const char *name;
  bool isBuffered;
  struct cf_MemoryPoolElem_ *next;
  int checkCode;
} cf_MemoryPoolElem;

/**
 * Memory Pool is a Fixed size alloctor, support fast memory alloc.
 *
 */
typedef struct cf_MemoryPool_ {
  size_t    size;
  size_t    itemSize;
  size_t    allocCount;
  cf_MemoryPoolElem *list;
  char *buffer;
} cf_MemoryPool;

/*========================================================================
 * Base method
 */

/**
 * constructor
 */
void cf_MemoryPool_make(cf_MemoryPool *self, size_t objSize, size_t objCount);

/**
 * alloc from pool, please using cf_MemoryPool_alloc replace.
 */
void *cf_MemoryPool_alloc_(cf_MemoryPool *self, const char *name);

/**
 * free memory which alloc from this pool.
 */
void cf_MemoryPool_free(cf_MemoryPool *self, void *p);

/**
 * free this pool.
 */
void cf_MemoryPool_dispose(cf_MemoryPool *self);

/**
 * alloc from pool.
 */
#define cf_MemoryPool_alloc(self) cf_MemoryPool_alloc_(self, __func__)

CF_END

#endif

