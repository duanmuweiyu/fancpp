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
 * Stack trace
 */
//_Thread_local
_Thread_local cf_ArrayFuncTrace cf_FuncTrace_callStack;
_Thread_local cf_HashMapFuncTrace cf_FuncTrace_funcMap;
_Thread_local uint64_t cf_FuncTrace_entryFuncTime;
_Thread_local bool cf_FuncTrace_inited = false;

/**
 * entry a function
 */
void cf_FuncTrace_onEntry(const char *name) {
  if (!cf_FuncTrace_inited) {
    cf_ArrayFuncTrace_make(&cf_FuncTrace_callStack, 0, 100);
    cf_HashMapFuncTrace_make(&cf_FuncTrace_funcMap, 1000);
    cf_FuncTrace_inited = true;
  }
  cf_FuncTrace_entryFuncTime = cf_nowTicks();
  cf_ArrayFuncTrace_addCopy(&cf_FuncTrace_callStack, (char*)name);

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

#ifdef CF_PRINT_FUNC
  cf_Log_log("func", cf_LogLevel_debug, "leave function: %s", name);
#endif

  if (!cf_FuncTrace_inited) {
    cf_abort("func trace not init");
  }

  popName = *cf_ArrayFuncTrace_pop(&cf_FuncTrace_callStack);
  if (strcmp(popName, name) != 0) {
    cf_abort("function trace mismatch");
  }

  err = cf_HashMapFuncTrace_get(&cf_FuncTrace_funcMap, name, NULL, &item);
  leaveFuncTime = cf_nowTicks();

  if (err == cf_Error_ok) {
    item.callTimes++;
    item.duration += (leaveFuncTime - cf_FuncTrace_entryFuncTime);
  } else {
    item.name = name;
    item.callTimes = 1;
    item.duration = (leaveFuncTime - cf_FuncTrace_entryFuncTime);
  }
  cf_HashMapFuncTrace_set(&cf_FuncTrace_funcMap, name, item, NULL, NULL);
}

/**
 * print stack trace
 */
void cf_FuncTrace_printStackTrace() {
  int i;
  int size;
  const char *name;

  if (!cf_FuncTrace_inited) return;
  printf("stack trace:\n");

  size = cf_ArrayFuncTrace_size(&cf_FuncTrace_callStack);
  for (i=0; i<size; ++i) {
    name = cf_ArrayFuncTrace_getCopy(&cf_FuncTrace_callStack, i);
    printf("  %s\n", name);
  }
}

/**
 * print performance profile count
 */
void cf_FuncTrace_printPerformance() {
  cf_HashMapFuncTraceIterator iter;
  cf_PerformanceItem item;
  cf_Error err;
  const char *name;

  if (!cf_FuncTrace_inited) return;
  printf("performance profile:\ntimes\tduration\tname\n");

  cf_HashMapFuncTrace_createIterator(&cf_FuncTrace_funcMap, &iter);
  while (!(err=cf_HashMapFuncTraceIterator_next(&iter))) {
    cf_HashMapFuncTraceIterator_get(&iter, &name, &item);
    printf("%ld\t%ld\t%s\n", item.callTimes, item.duration, item.name);
  }
}

void cf_FuncTrace_dispose() {
  if (cf_FuncTrace_inited) {
    cf_ArrayFuncTrace_dispose(&cf_FuncTrace_callStack);
    cf_HashMapFuncTrace_dispose(&cf_FuncTrace_funcMap);
    cf_FuncTrace_inited = false;
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

  if (!cf_FuncTrace_inited) return NULL;

  str = (char*)malloc(256);
  if (str == NULL) {
    cf_abort("bad alloc");
  }
  strAlloc = 255;

  size = cf_ArrayFuncTrace_size(&cf_FuncTrace_callStack);
  for (i=0; i<size; ++i) {
    name = cf_ArrayFuncTrace_getCopy(&cf_FuncTrace_callStack, i);
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