/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#include "cfan/Memory.h"
#include "Error.h"
#include <stdio.h>

#include <string.h>

cf_MemManager memManager = { NULL, NULL };

void *cf_doMalloc(const char *file, const char *func, const unsigned int line, size_t size) {
  cf_MemChunk *chunk;
  chunk = (cf_MemChunk *)malloc(size + sizeof(cf_MemChunk));
  if (!chunk) return NULL;
  if (NULL == memManager.last) {
    memManager.first = chunk;
    memManager.last = chunk;
    chunk->prev = NULL;
  } else {
    memManager.last->next = chunk;
    chunk->prev = memManager.last;
    memManager.last = chunk;
  }
  chunk->file = file;
  chunk->func = func;
  chunk->line = line;
  chunk->checkCode = cf_Memory_checkCode;
  chunk->refCount = 0;
  chunk->next = NULL;
  chunk->size = size;
  return chunk + 1;
}

void *cf_doCalloc(const char *file, const char *func, const unsigned int line, size_t nobj, size_t size) {
  void *temp;
  temp = cf_doMalloc(file, func, line, nobj * size);
  if (!temp) return NULL;
  memset(temp, 0, nobj * size);
  return temp;
}

void *cf_realloc(void *p, size_t size) {
  cf_MemChunk *chunk;
  chunk = (cf_MemChunk *)((char*)p - sizeof(cf_MemChunk));
  chunk = (cf_MemChunk *)realloc(chunk, size + sizeof(cf_MemChunk));
  if (!chunk) return NULL;
  chunk->size = size;
  if (chunk->prev) {
    chunk->prev->next = chunk;
  } else {
    memManager.first = chunk;
  }

  if (chunk->next == NULL) {
    memManager.last = chunk;
  }
  return chunk + 1;
}

void cf_free(void *p) {
  cf_MemChunk *chunk;
  cf_assert(p);

  chunk = (cf_MemChunk *)((char*)p - sizeof(cf_MemChunk));
  if (chunk->checkCode != cf_Memory_checkCode) {
    cf_Log_log(cf_Log_tag, cf_LogLevel_err, "bad free");
    exit(2);
  }

  if (chunk->next) {
    if (chunk->prev) {
      chunk->prev->next = chunk->next;
      chunk->next->prev = chunk->prev;
    } else {
      memManager.first = chunk->next;
      chunk->next->prev = NULL;
    }
  } else if (chunk->prev) {
    chunk->prev->next = NULL;
    memManager.last = chunk->prev;
  } else {
    memManager.first = NULL;
    memManager.last = NULL;
  }

  chunk->checkCode = 0;
  free(chunk);
}

void cf_dumpMem() {
  cf_MemChunk *chunk;
  for (chunk = memManager.first; chunk != NULL; chunk = chunk->next) {
    cf_Log_cfDebug("func:%s, line:%d, size:%d, refCount:%d"
      , chunk->func, chunk->line, chunk->size, chunk->refCount);
  }
}
