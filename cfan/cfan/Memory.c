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

#define cf_Memory_getTailCheckCode(chunk) (*((int*)(((char*)(chunk + 1))+chunk->size)))
#define cf_Memory_setTailCheckCode(chunk, code) cf_Memory_getTailCheckCode(chunk) = code

void *cf_doMalloc(const char *file, const char *func, const unsigned int line, size_t size) {
  cf_MemChunk *chunk;

  // chunk + userData + tail checkCode
  chunk = (cf_MemChunk *)malloc(size + sizeof(cf_MemChunk) + sizeof(int));
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
  //set last 4 byte as check code
  cf_Memory_setTailCheckCode(chunk, cf_Memory_checkCode);
  //cf_Memory_doCheck(chunk);
  return chunk + 1;
}

void *cf_doCalloc(const char *file, const char *func, const unsigned int line, size_t nobj, size_t size) {
  void *temp;
  temp = cf_doMalloc(file, func, line, nobj * size);
  if (!temp) return NULL;
  memset(temp, 0, nobj * size);
  return temp;
}

inline void cf_Memory_doCheck(cf_MemChunk *chunk) {
  if (chunk->checkCode != cf_Memory_checkCode) {
    cf_Log_log(cf_Log_tag, cf_LogLevel_err, "bad heap, front overflow.");
    exit(2);
  }

  if (cf_Memory_getTailCheckCode(chunk) != cf_Memory_checkCode  ) {
    printf("checkcode %d\n", cf_Memory_getTailCheckCode(chunk));
    cf_Log_log(cf_Log_tag, cf_LogLevel_err, "bad heap, back overflow.");
    exit(2);
  }
}

void cf_Memory_check(const char *file, const char *func, const unsigned int line, void *p) {
  cf_MemChunk *chunk;
  if (p == NULL) return;
  chunk = (cf_MemChunk *)((char*)p - sizeof(cf_MemChunk));
  if (chunk->checkCode != cf_Memory_checkCode) {
    cf_Log_doLog(cf_Log_tag, file, func, line, cf_LogLevel_err, "bad heap, front overflow.");
    exit(2);
  }

  if (cf_Memory_getTailCheckCode(chunk) != cf_Memory_checkCode  ) {
    printf("checkcode %d\n", cf_Memory_getTailCheckCode(chunk));
    cf_Log_doLog(cf_Log_tag, file, func, line, cf_LogLevel_err, "bad heap, back overflow.");
    exit(2);
  }
}

void *cf_realloc(void *p, size_t size) {
  cf_MemChunk *chunk;
  cf_assert(p);
  chunk = (cf_MemChunk *)((char*)p - sizeof(cf_MemChunk));
  cf_Memory_doCheck(chunk);
  chunk = (cf_MemChunk *)realloc(chunk, size + sizeof(cf_MemChunk) + sizeof(int));
  if (!chunk) return NULL;
  chunk->size = size;
  cf_Memory_setTailCheckCode(chunk, cf_Memory_checkCode);
  cf_Memory_doCheck(chunk);
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
  cf_Memory_doCheck(chunk);

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

void cf_checkMem() {
  cf_MemChunk *chunk;

  for (chunk = memManager.first; chunk != NULL; chunk = chunk->next) {
    cf_Memory_doCheck(chunk);
  }
}

void cf_dumpMem() {
  cf_MemChunk *chunk;

  for (chunk = memManager.first; chunk != NULL; chunk = chunk->next) {
    cf_Memory_doCheck(chunk);
    cf_Log_cfDebug("func:%s, line:%d, size:%d, refCount:%d"
      , chunk->func, chunk->line, chunk->size, chunk->refCount);
  }
}