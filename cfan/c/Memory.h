/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CF_MEMORY_H_
#define _CF_MEMORY_H_


#include "macro.h"

#include <stdlib.h>

CF_BEGIN

typedef struct cf_MemChunk_ {
  const char *file;
  const char *func;
  unsigned int line;
  struct cf_MemChunk_ *next; //next chunk
  struct cf_MemChunk_ *prev; //previous chunk
  size_t size;
  size_t refCount;
} cf_MemChunk;

typedef struct cf_MemManager_ {
  cf_MemChunk *first;
  cf_MemChunk *last;
} cf_MemManager;

extern cf_MemManager memManager;

void *cf_doMalloc(const char *file, const char *func, const unsigned int line, size_t size);

void *cf_doCalloc(const char *file, const char *func, const unsigned int line, size_t nobj, size_t size);

void *cf_realloc(void *p, size_t size);

void cf_free(void *p);

void cf_dumpMem();

#define cf_malloc(size) cf_doMalloc(__FILE__, __func__, __LINE__, size)
#define cf_calloc(nobj, size) cf_doCalloc(__FILE__, __func__, __LINE__, nobj, size)

CF_END

#endif