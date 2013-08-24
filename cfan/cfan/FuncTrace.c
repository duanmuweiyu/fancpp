/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-2-6  Jed Young  Creation
 */

#include "cfan/FuncTrace.h"
#include "cfan/Array.h"
#include "cfan/HashMap.h"
#include "tinyCThread/tinycthread.h"
#include "cfan/StrBuf.h"

#include <signal.h>

/*========================================================================
 * time ticks
 */
#ifdef WIN32
  #include <windows.h>
  uint64_t cf_nowTicks() {
    LARGE_INTEGER m_nBeginTime;
    LARGE_INTEGER m_nFreq;
    QueryPerformanceFrequency(&m_nFreq);
    QueryPerformanceCounter(&m_nBeginTime);
    return (m_nBeginTime.QuadPart*1000000)/m_nFreq.QuadPart;
  }
#endif

/*========================================================================
 * define
 */

#undef CF_ENTRY_FUNC
#undef CF_EXIT_FUNC
#define CF_ENTRY_FUNC
#define CF_EXIT_FUNC

typedef struct cf_PerformanceItem_ {
  const char *name;
  unsigned long callTimes;
  unsigned long duration;
} cf_PerformanceItem;

cf_HashMapTemplate(cf_HashMapFuncTrace, const char*, cf_PerformanceItem)

#define cf_hashFunc(key) cf_HashMap_strHash(key)
#define cf_compFunc(v1, v2) strcmp((v1), (v2))

cf_HashMapTemplate_impl(cf_HashMapFuncTrace, const char*, cf_PerformanceItem)

#undef cf_hashFunc
#undef cf_compFunc

cf_ArrayTemplate(cf_ArrayFuncTrace, const char*)
cf_ArrayTemplate_impl(cf_ArrayFuncTrace, const char*)

#undef CF_ENTRY_FUNC
#undef CF_EXIT_FUNC
#define CF_ENTRY_FUNC CF_ENTRY_FUNC_
#define CF_EXIT_FUNC CF_EXIT_FUNC_

/*========================================================================
 * _Thread_local
 */

static tss_t callStack_;
static tss_t funcMap_;
static tss_t entryFuncTime_;
static tss_t inited_;
static volatile bool threadLocalVarInited = false;

static void deleteCallStack(void *arg) {
  cf_ArrayFuncTrace *f = (cf_ArrayFuncTrace*)arg;
  cf_ArrayFuncTrace_dispose(f);
  free(f);
  tss_delete(callStack_);
}

static void deleteFuncMap(void *arg) {
  cf_HashMapFuncTrace *f = (cf_HashMapFuncTrace*)arg;
  cf_HashMapFuncTrace_dispose(f);
  free(f);
  tss_delete(funcMap_);
}

static void deleteEntryFuncTime(void *arg) {
  uint64_t *f = (uint64_t*)arg;
  free(f);
  tss_delete(entryFuncTime_);
}

static void initThreadLocal() {
  tss_create(&callStack_, (tss_dtor_t)deleteCallStack);
  tss_create(&funcMap_, (tss_dtor_t)deleteFuncMap);
  tss_create(&entryFuncTime_, (tss_dtor_t)deleteEntryFuncTime);
  tss_create(&inited_, NULL);
}

static bool cf_FuncTrace_isInited() {
  void *t = tss_get(inited_);
  return t;
}

static void cf_FuncTrace_setInited(bool b) {
  tss_set(inited_, (void*)b);
}

static cf_ArrayFuncTrace *cf_FuncTrace_callStack() {
  cf_ArrayFuncTrace *t = (cf_ArrayFuncTrace*)(tss_get(callStack_));
  if (t == NULL) {
    t = (cf_ArrayFuncTrace*)malloc(sizeof(cf_ArrayFuncTrace));
    cf_ArrayFuncTrace_make(t, 0, 100);
    tss_set(callStack_, t);
  }
  return t;
}

static cf_HashMapFuncTrace *cf_FuncTrace_funcMap() {
  cf_HashMapFuncTrace *t = (cf_HashMapFuncTrace*)(tss_get(funcMap_));
  if (t == NULL) {
    t = (cf_HashMapFuncTrace*)malloc(sizeof(cf_HashMapFuncTrace));
    cf_HashMapFuncTrace_make(t, 1000);
    tss_set(funcMap_, t);
  }
  return t;
}

static uint64_t *cf_FuncTrace_entryFuncTime() {
  uint64_t *t = (uint64_t*)(tss_get(entryFuncTime_));
  if (t == NULL) {
    t = (uint64_t*)malloc(sizeof(uint64_t));
    *t = 0;
    tss_set(entryFuncTime_, t);
  }
  return t;
}

/*========================================================================
 * Stack trace
 */

void cf_FuncTrace_init() {
  if (!threadLocalVarInited) {
    initThreadLocal();
    threadLocalVarInited = true;
  }
}

void cf_FuncTrace_dispose() {
  cf_ArrayFuncTrace *f = cf_FuncTrace_callStack();
  cf_ArrayFuncTrace_dispose(f);
  free(f);
  tss_delete(callStack_);

  cf_HashMapFuncTrace *f2 = cf_FuncTrace_funcMap();
  cf_HashMapFuncTrace_dispose(f2);
  free(f2);
  tss_delete(funcMap_);

  uint64_t *f3 = cf_FuncTrace_entryFuncTime();
  free(f3);
  tss_delete(entryFuncTime_);

  tss_delete(inited_);
}

/**
 * entry a function
 */
void cf_FuncTrace_onEntry(const char *name) {
  if (!threadLocalVarInited) {
    initThreadLocal();
    threadLocalVarInited = true;
  }

  //do init
  cf_FuncTrace_funcMap();

  *cf_FuncTrace_entryFuncTime() = cf_nowTicks();
  cf_ArrayFuncTrace_addCopy(cf_FuncTrace_callStack(), (char*)name);

  cf_FuncTrace_setInited(true);

#ifdef CF_PRINT_FUNC
  cf_Log_log("func", cf_LogLevel_debug, "entry function: %s", name);
#endif
}

/**
 * leave a function
 */
void cf_FuncTrace_onLeave(const char *name) {
  const char *popName;
  uint64_t leaveFuncTime;
  cf_PerformanceItem item;
  cf_Error err;
  cf_HashMapFuncTrace *map;

  if (!cf_FuncTrace_isInited()) {
    return;
  }

#ifdef CF_PRINT_FUNC
  cf_Log_log("func", cf_LogLevel_debug, "leave function: %s", name);
#endif

  popName = *cf_ArrayFuncTrace_pop(cf_FuncTrace_callStack());
  if (strcmp(popName, name) != 0) {
    cf_abort("function trace mismatch");
  }

  map = cf_FuncTrace_funcMap();
  err = cf_HashMapFuncTrace_get(map, name, NULL, &item);
  leaveFuncTime = cf_nowTicks();

  if (err == cf_Error_ok) {
    item.callTimes++;
    item.duration += (leaveFuncTime - *cf_FuncTrace_entryFuncTime());
  } else {
    item.name = name;
    item.callTimes = 1;
    item.duration = (leaveFuncTime - *cf_FuncTrace_entryFuncTime());
  }
  cf_HashMapFuncTrace_set(map, name, item, NULL, NULL);
}

/**
 * print stack trace
 */
void cf_FuncTrace_printStackTrace() {
  int i;
  int size;
  const char *name;
  cf_ArrayFuncTrace *trace;

  if (!cf_FuncTrace_isInited()) {
    return;
  }

  printf("stack trace:\n");

  trace = cf_FuncTrace_callStack();
  size = cf_ArrayFuncTrace_size(trace);
  for (i=0; i<size; ++i) {
    name = cf_ArrayFuncTrace_getCopy(trace, i);
    printf("  %s\n", name);
  }

  printf("end stack trace\n");
}

/**
 * print performance profile count
 */
void cf_FuncTrace_printPerformance() {
  cf_HashMapFuncTraceIterator iter;
  cf_PerformanceItem item;
  cf_Error err;
  const char *name;

  if (!cf_FuncTrace_isInited()) {
    return;
  }

  printf("performance profile:\ntimes\tduration\tname\n");

  cf_HashMapFuncTrace_createIterator(cf_FuncTrace_funcMap(), &iter);
  while (!(err=cf_HashMapFuncTraceIterator_next(&iter))) {
    cf_HashMapFuncTraceIterator_get(&iter, &name, &item);
    printf("%ld\t%ld\t%s\n", item.callTimes, item.duration, item.name);
  }
}

char *cf_FuncTrace_getTraceString_() {
  char *str;
  int strSize = 0;
  int strAlloc;
  int nameSize;
  int i;
  int size;
  const char *name;
  cf_ArrayFuncTrace *trace;

  if (!cf_FuncTrace_isInited()) {
    return NULL;
  }

  str = (char*)malloc(256);
  if (str == NULL) {
    cf_abort("bad alloc");
  }
  strAlloc = 255;

  trace = cf_FuncTrace_callStack();
  size = cf_ArrayFuncTrace_size(trace);
  for (i=0; i<size; ++i) {
    name = cf_ArrayFuncTrace_getCopy(trace, i);
    //+1 for comma
    nameSize = strlen(name)+1;
    if (strSize + nameSize > strAlloc) {
      strAlloc = (strSize + nameSize)*3/2;
      str = (char*)realloc(str, strAlloc+1);
      if (str == NULL) {
        cf_abort("bad alloc");
      }
    }
    strcpy(str+strSize, name);
    strSize += nameSize;
    str[strSize-1] = ',';
    str[strSize] = '\0';
  }

  if (strSize > 0) {
    str[strSize-1] = '\0';
  }
  return str;
}

/*========================================================================
 * signal
 */

typedef void (*cf_FuncTrace_sighandler) (int);
cf_FuncTrace_sighandler cf_FuncTrace_SIGABRT;
cf_FuncTrace_sighandler cf_FuncTrace_SIGFPE;
cf_FuncTrace_sighandler cf_FuncTrace_SIGILL;
cf_FuncTrace_sighandler cf_FuncTrace_SIGINT;
cf_FuncTrace_sighandler cf_FuncTrace_SIGSEGV;
cf_FuncTrace_sighandler cf_FuncTrace_SIGTERM;

static void printTrace(int i) {
  printf("received signal: %d\n", i);
  cf_FuncTrace_printStackTrace();

  switch (i) {
  case SIGABRT:
    //if (cf_FuncTrace_SIGABRT != NULL) cf_FuncTrace_SIGABRT(i);
    //abort();
    break;
  case SIGFPE:
    if (cf_FuncTrace_SIGFPE != NULL) cf_FuncTrace_SIGFPE(i);
    else abort();
    break;
  case SIGILL:
    if (cf_FuncTrace_SIGILL != NULL) cf_FuncTrace_SIGILL(i);
    else abort();
    break;
  case SIGINT:
    if (cf_FuncTrace_SIGINT != NULL) cf_FuncTrace_SIGINT(i);
    else abort();
    break;
  case SIGSEGV:
    if (cf_FuncTrace_SIGSEGV != NULL) cf_FuncTrace_SIGSEGV(i);
    else abort();
    break;
  case SIGTERM:
    if (cf_FuncTrace_SIGTERM != NULL) cf_FuncTrace_SIGTERM(i);
    else abort();
    break;
  default:
    abort();
    break;
  }
}

void cf_FuncTrace_traceOnExit() {
  atexit(cf_FuncTrace_printStackTrace);
  cf_FuncTrace_SIGABRT = signal(SIGABRT, printTrace);
  cf_FuncTrace_SIGFPE = signal(SIGFPE, printTrace);
  cf_FuncTrace_SIGILL = signal(SIGILL, printTrace);
  cf_FuncTrace_SIGINT = signal(SIGINT, printTrace);
  cf_FuncTrace_SIGSEGV = signal(SIGSEGV, printTrace);
  cf_FuncTrace_SIGTERM = signal(SIGTERM, printTrace);
}
