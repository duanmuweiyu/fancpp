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


#include "cfan/macro.h"

#include <stdlib.h>

CF_BEGIN

/*========================================================================
 * Memory manage
 */

/**
 * memory overflow check code
 */
#define cf_Memory_checkCode 0xABCD

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
  size_t refCount;
  int checkCode;
} cf_MemChunk;

/**
 * Momory manager contains a MemChunk linked list.
 */
typedef struct cf_MemManager_ {
  cf_MemChunk *first;
  cf_MemChunk *last;
} cf_MemManager;


/**
 * global memory manager object.
 */
extern cf_MemManager cf_Memory_memManager;

/**
 * actually do memory alloc.
 */
void *cf_Memory_malloc(const char *file, const char *func, const unsigned int line, size_t size);

/**
 * actually do clear memory alloc.
 */
void *cf_Memory_calloc(const char *file, const char *func, const unsigned int line, size_t nobj, size_t size);

/**
 * re alloc memory.
 */
void *cf_Memory_realloc(void *p, size_t size);

/**
 * free memory.
 */
void cf_Memory_free(const char *file, const char *func, const unsigned int line, void *p);

/**
 * dump memory leak.
 */
void cf_Memory_dumpMem();

/**
 * check memory overflow.
 */
void cf_Memory_checkMem();

/**
 * actually do memory check.
 */
void cf_Memory_check(const char *file, const char *func, const unsigned int line, void *p);


/*========================================================================
 * Memory manage macro define
 */

#ifdef CF_DEBUG

  #define cf_malloc(size) cf_Memory_malloc(__FILE__, __func__, __LINE__, size)
  #define cf_calloc(nobj, size) cf_Memory_calloc(__FILE__, __func__, __LINE__, nobj, size)
  #define cf_realloc(p, size) cf_Memory_realloc(p, size)
  #define cf_free(p) cf_Memory_free(__FILE__, __func__, __LINE__, p)

  #define cf_check(p) cf_Memory_check(__FILE__, __func__, __LINE__, p)
  #define cf_dumpMem() cf_Memory_dumpMem()
  #define cf_checkMem() cf_Memory_checkMem()

#else

  #define cf_malloc(size) malloc(size)
  #define cf_calloc(nobj, size) calloc(nobj, size)
  #define cf_realloc(nobj, size) realloc(nobj, size)
  #define cf_free(p) free(p)

  #define cf_check(p)
  #define cf_dumpMem()
  #define cf_checkMem()

#endif

#ifdef __cplusplus
  #include <new>

  #define NEW(Type, ...) (new ((void*)cf_malloc(sizeof(Type))) Type(## __VA_ARGS__))
  #define DELETE(Type, p) {p->~Type(); cf_free(p);}
#endif

CF_END

#endif //_CF_MEMORY_H_
