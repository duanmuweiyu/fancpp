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
#include "cfan/Error.h"
#include "cfan/Trace.h"
#include "tinyCThread/tinycthread.h"
#include <stdio.h>

#include <string.h>


#ifndef CF_NO_THREAD_SAFE
  mtx_t cf_Memory_mutex;
#endif

/*========================================================================
 * Memory manage define
 */

/**
 * A block of memory that alloced.
 */
typedef struct cf_MemChunk_ {
  const char *file;
  const char *func;
  const char *trace;
  unsigned int line;
  struct cf_MemChunk_ *next; //next chunk
  struct cf_MemChunk_ *prev; //previous chunk
  size_t size;
  //size_t refCount;
  int checkCode;
} cf_MemChunk;

/**
 * Momory manager contains a MemChunk linked list.
 */
typedef struct cf_MemManager_ {
  cf_MemChunk *first;
  cf_MemChunk *last;
} cf_MemManager;

cf_MemManager cf_Memory_memManager = { NULL, NULL };
/**
 * memory overflow check code
 */
#define cf_Memory_headCheckCode 0xABCD
#define cf_Memory_tailCheckCode 0xDCBA
#define cf_Memory_clearCode 0xCC

#define cf_Memory_getTailCheckCode(chunk) (*((int*)(((char*)(chunk + 1))+chunk->size)))
#define cf_Memory_setTailCheckCode(chunk, code) cf_Memory_getTailCheckCode(chunk) = code

/*========================================================================
 * methods
 */

void *cf_Memory_malloc(const char *file, const char *func, const unsigned int line, size_t size) {
  cf_MemChunk *chunk;

  // chunk + userData + tail checkCode
  chunk = (cf_MemChunk *)malloc(size + sizeof(cf_MemChunk) + sizeof(int));
  if (!chunk) return NULL;

#ifndef CF_NO_THREAD_SAFE
  mtx_lock(&cf_Memory_mutex);
#endif
  if (NULL == cf_Memory_memManager.last) {
    cf_Memory_memManager.first = chunk;
    cf_Memory_memManager.last = chunk;
    chunk->prev = NULL;
  } else {
    cf_Memory_memManager.last->next = chunk;
    chunk->prev = cf_Memory_memManager.last;
    cf_Memory_memManager.last = chunk;
  }
#ifndef CF_NO_THREAD_SAFE
  mtx_unlock(&cf_Memory_mutex);
#endif

  chunk->file = file;
  chunk->func = func;
  chunk->line = line;
  chunk->checkCode = cf_Memory_headCheckCode;
  //chunk->refCount = 0;
  chunk->next = NULL;
  chunk->size = size;
  chunk->trace = cf_Trace_getTraceString();

  //set last 4 byte as check code
  cf_Memory_setTailCheckCode(chunk, cf_Memory_tailCheckCode);

  //do default clear
  memset(chunk+1, cf_Memory_clearCode, size);
  //cf_Memory_doCheck(chunk);
  return chunk + 1;
}

void *cf_Memory_calloc(const char *file, const char *func, const unsigned int line, size_t nobj, size_t size) {
  void *temp;
  temp = cf_Memory_malloc(file, func, line, nobj * size);
  if (!temp) return NULL;
  memset(temp, 0, nobj * size);
  return temp;
}

static inline void cf_Memory_doCheck_(cf_MemChunk *chunk) {
  if (chunk->checkCode != cf_Memory_headCheckCode) {
    cf_abort("bad heap, front overflow.");
  }

  if (cf_Memory_getTailCheckCode(chunk) != cf_Memory_tailCheckCode  ) {
    printf("checkcode %d\n", cf_Memory_getTailCheckCode(chunk));
    cf_abort("bad heap, back overflow.");
  }
}

void cf_Memory_check(const char *file, const char *func, const unsigned int line, void *p) {
  cf_MemChunk *chunk;
  if (p == NULL) return;
  chunk = (cf_MemChunk *)((char*)p - sizeof(cf_MemChunk));
  if (chunk->checkCode != cf_Memory_headCheckCode) {
    cf_Log_doLog(cf_Log_tag, file, func, line, cf_LogLevel_err, "bad heap, front overflow.");
    abort();
  }

  if (cf_Memory_getTailCheckCode(chunk) != cf_Memory_tailCheckCode  ) {
    printf("checkcode %d\n", cf_Memory_getTailCheckCode(chunk));
    cf_Log_doLog(cf_Log_tag, file, func, line, cf_LogLevel_err, "bad heap, back overflow.");
    abort();
  }
}

void *cf_Memory_realloc(void *p, size_t size) {
  cf_MemChunk *chunk;
  cf_assert(p);
  chunk = (cf_MemChunk *)((char*)p - sizeof(cf_MemChunk));
  cf_Memory_doCheck_(chunk);

#ifndef CF_NO_THREAD_SAFE
  mtx_lock(&cf_Memory_mutex);
#endif
  chunk = (cf_MemChunk *)realloc(chunk, size + sizeof(cf_MemChunk) + sizeof(int));
  if (!chunk) return NULL;

  //if size > old size clear bit.
  if (size > chunk->size) {
    memset(((char*)(chunk + 1))+chunk->size, cf_Memory_clearCode, size - chunk->size);
  }

  chunk->size = size;
  cf_Memory_setTailCheckCode(chunk, cf_Memory_tailCheckCode);
  cf_Memory_doCheck_(chunk);
  if (chunk->prev) {
    chunk->prev->next = chunk;
  } else {
    cf_Memory_memManager.first = chunk;
  }
  if (chunk->next == NULL) {
    cf_Memory_memManager.last = chunk;
  }
#ifndef CF_NO_THREAD_SAFE
  mtx_unlock(&cf_Memory_mutex);
#endif
  return chunk + 1;
}

void cf_Memory_free(const char *file, const char *func, const unsigned int line, void *p) {
  cf_MemChunk *chunk;
  cf_assert(p);

  chunk = (cf_MemChunk *)((char*)p - sizeof(cf_MemChunk));
  cf_Memory_check(file, func, line, p);

#ifndef CF_NO_THREAD_SAFE
  mtx_lock(&cf_Memory_mutex);
#endif
  if (chunk->next) {
    if (chunk->prev) {
      chunk->prev->next = chunk->next;
      chunk->next->prev = chunk->prev;
    } else {
      cf_Memory_memManager.first = chunk->next;
      chunk->next->prev = NULL;
    }
  } else if (chunk->prev) {
    chunk->prev->next = NULL;
    cf_Memory_memManager.last = chunk->prev;
  } else {
    cf_Memory_memManager.first = NULL;
    cf_Memory_memManager.last = NULL;
  }

  chunk->checkCode = 0;
  cf_Memory_setTailCheckCode(chunk, 0);
  memset(chunk, cf_Memory_clearCode, chunk->size + sizeof(cf_MemChunk) + sizeof(int));
  free(chunk);
#ifndef CF_NO_THREAD_SAFE
  mtx_unlock(&cf_Memory_mutex);
#endif
}

void cf_Memory_checkMem() {
  cf_MemChunk *chunk;
#ifndef CF_NO_THREAD_SAFE
  mtx_lock(&cf_Memory_mutex);
#endif
  chunk = cf_Memory_memManager.first;
  for (; chunk != NULL; chunk = chunk->next) {
    cf_Memory_doCheck_(chunk);
  }
#ifndef CF_NO_THREAD_SAFE
  mtx_unlock(&cf_Memory_mutex);
#endif
}

void cf_Memory_dumpMem() {
  cf_MemChunk *chunk;
#ifndef CF_NO_THREAD_SAFE
  mtx_lock(&cf_Memory_mutex);
#endif
  chunk = cf_Memory_memManager.first;
  printf("memory dump:\n");
  for (; chunk != NULL; chunk = chunk->next) {
    cf_Memory_doCheck_(chunk);
    printf("func:%s(%s), line:%d, size:%ld\n"
      , chunk->func, chunk->trace, chunk->line, chunk->size);
  }
  printf("end memory dump\n");
#ifndef CF_NO_THREAD_SAFE
  mtx_unlock(&cf_Memory_mutex);
#endif
}
