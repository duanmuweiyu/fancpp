/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CF_TRACE_H_
#define _CF_TRACE_H_

#define CF_FUNCTRACE

#include "cfan/FuncTrace.h"
#include "cfan/StackTrace.h"

CF_BEGIN

/**
 * print stack trace
 */
static inline void cf_Trace_printTrace(void) {
#ifdef CF_FUNCTRACE
  cf_FuncTrace_printTrace();
#elif defined(CF_STACKTRACE)
  cf_StackTrace_printTrace();
#endif
}

static inline void cf_Trace_printPerformance(void) {
#ifdef CF_FUNCTRACE
  cf_FuncTrace_printPerformance();
#endif
}

/**
 * get stack trace string
 */
static inline char *cf_Trace_getTraceString(void) {
#ifdef CF_FUNCTRACE
  return cf_FuncTrace_getTraceString();
#elif defined(CF_STACKTRACE)
  return cf_StackTrace_getTraceString();
#else
  return NULL;
#endif
}

/**
 * init threadlocal
 */
static inline void cf_Trace_init() {
#ifdef CF_FUNCTRACE
  cf_FuncTrace_init();
#endif
}

/**
 * release current thread memory
 */
static inline void cf_Trace_dispose() {
#ifdef CF_FUNCTRACE
  cf_FuncTrace_dispose();
#endif
}

/**
 * print stack trace on system exit.
 */
void cf_Trace_traceOnExit();

CF_END

#endif
