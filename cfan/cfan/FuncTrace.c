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

/*========================================================================
 * Performace counter
 */

typedef struct cf_PerformanceItem_ {
  const char *name;
  unsigned long callTimes;
  unsigned long duration;
} cf_PerformanceItem;

cf_HashMapTemplate(cf_HashMapSP, const char*, cf_PerformanceItem)

#define cf_hashFunc(key) cf_HashMap_strHash(key)
#define cf_compFunc(v1, v2) strcmp((v1), (v2))

cf_HashMapTemplate_impl(cf_HashMapSP, const char*, cf_PerformanceItem)

#undef cf_hashFunc
#undef cf_compFunc

/*========================================================================
 * Stack trace
 */
//_Thread_local
_Thread_local cf_ArrayS callStack;
_Thread_local cf_HashMapSP funcMap;
_Thread_local uint64_t entryFuncTime;
_Thread_local bool inited = false;

/**
 * entry a function
 */
void cf_FuncTrace_onEntry(const char *name) {
  if (!inited) {
    cf_ArrayS_make(&callStack, 0, 100);
    cf_HashMapSP_make(&funcMap, 1000);
    inited = true;
  }
  entryFuncTime = cf_nowTicks();
  cf_ArrayS_addCopy(&callStack, (char*)name);
  cf_Log_log("func", cf_LogLevel_debug, "entry function: %s", name);
}

/**
 * leave a function
 */
void cf_FuncTrace_onLeave(const char *name) {
  char *popName;
  uint64_t leaveFuncTime;
  cf_PerformanceItem item;
  cf_Error err;

  cf_Log_log("func", cf_LogLevel_debug, "leave function: %s", name);

  popName = *cf_ArrayS_pop(&callStack);
  if (strcmp(popName, name) != 0) {
    cf_abort("function trace mismatch");
  }

  err = cf_HashMapSP_get(&funcMap, name, NULL, &item);
  leaveFuncTime = cf_nowTicks();

  if (err == cf_Error_ok) {
    item.callTimes++;
    item.duration += (leaveFuncTime - entryFuncTime);
  } else {
    item.name = name;
    item.callTimes = 1;
    item.duration = (leaveFuncTime - entryFuncTime);
  }
  cf_HashMapSP_set(&funcMap, name, item, NULL, NULL);
}

/**
 * print stack trace
 */
void cf_FuncTrace_printStackTrace() {
  int i;
  int size;
  const char *name;

  if (!inited) return;
  printf("stack trace:\n");

  size = cf_ArrayS_size(&callStack);
  for (i=0; i<size; ++i) {
    name = cf_ArrayS_getCopy(&callStack, i);
    printf("  %s\n", name);
  }
}

/**
 * print performance profile count
 */
void cf_FuncTrace_printPerformance() {
  cf_HashMapSPIterator iter;
  cf_PerformanceItem item;
  cf_Error err;
  const char *name;

  if (!inited) return;
  printf("performance profile:\ntimes\tduration\tname\n");

  cf_HashMapSP_createIterator(&funcMap, &iter);
  while (!(err=cf_HashMapSPIterator_next(&iter))) {
    cf_HashMapSPIterator_get(&iter, &name, &item);
    printf("%ld\t%ld\t%s\n", item.callTimes, item.duration, item.name);
  }
}

void cf_FuncTrace_dispose() {
  if (inited) {
    cf_ArrayS_dispose(&callStack);
    cf_HashMapSP_dispose(&funcMap);
    inited = false;
  }
}

char *cf_FuncTrace_getTraceString() {
  cf_StrBuf str;
  int i;
  int size;
  const char *name;
  char *trace;

  cf_StrBuf_make(&str);

  size = cf_ArrayS_size(&callStack);
  for (i=0; i<size; ++i) {
    name = cf_ArrayS_getCopy(&callStack, i);
    cf_StrBuf_add(&str, name, -1);
    cf_StrBuf_add(&str, ",", -1);
  }
  if (str.size == 0) {
    trace = (char*)malloc(1);
    *trace = 0;
    return trace;
  }
  cf_StrBuf_removeLast(&str);
  trace = (char*)malloc(str.size+1);
  strcpy(trace, cf_StrBuf_str(&str));
  cf_StrBuf_dispose(&str);
  return trace;
}