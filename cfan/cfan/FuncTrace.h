/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-2-6  Jed Young  Creation
 */

#ifndef _CF_FUNCTRACE_H_
#define _CF_FUNCTRACE_H_

#include "cfan/macro.h"
#include "cfan/miss.h"

CF_BEGIN

#ifdef CF_DEBUG
  #define CF_FUNCTRACE
#endif

/*========================================================================
 * stack trace
 */

/**
 * entry a function
 */
void cf_FuncTrace_onEntry(const char *name);

/**
 * leave a function
 */
void cf_FuncTrace_onLeave(const char *name);

/**
 * init threadlocal
 */
void cf_FuncTrace_init();

/**
 * release current thread memory
 */
void cf_FuncTrace_dispose();

/**
 * print stack trace
 */
void cf_FuncTrace_printTrace();

/**
 * print performance profile count
 */
void cf_FuncTrace_printPerformance();

/**
 * get a formated trace string.
 * alloc by malloc not cf_malloc.
 */
char *cf_FuncTrace_getTraceString();


/*========================================================================
 * stack trace macro
 */
#ifdef CF_FUNCTRACE
  #define CF_ENTRY_FUNC_ cf_FuncTrace_onEntry(__func__);
  #define CF_EXIT_FUNC_  cf_FuncTrace_onLeave(__func__);
#else
  #define CF_ENTRY_FUNC_
  #define CF_EXIT_FUNC_
#endif

#define CF_ENTRY_FUNC CF_ENTRY_FUNC_
#define CF_EXIT_FUNC CF_EXIT_FUNC_

/*========================================================================
 * define CF_CPP_FUNCTRACE for C++ stack trace
 */
#ifdef __cplusplus
  class cf_FuncTraceClass {
    const char *funcName;
  public:
    cf_FuncTraceClass(const char *name) : funcName(name) {
      cf_FuncTrace_onEntry(name);
    }
    ~cf_FuncTraceClass() {
      cf_FuncTrace_onLeave(funcName);
    }
  };
  #ifdef CF_FUNCTRACE
    #define CF_CPP_FUNCTRACE_ cf_FuncTraceClass __cf_funcTraceObj(__func__);
  #else
    #define CF_CPP_FUNCTRACE_
  #endif
  #define CF_CPP_FUNCTRACE CF_CPP_FUNCTRACE_
#endif


CF_END
#endif
