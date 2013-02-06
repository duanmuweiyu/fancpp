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

#ifdef CF_DEBUG
  #define CF_FUNCTRACE
#else
  #undef CF_FUNCTRACE
#endif

#ifdef CF_FUNCTRACE
  #define CF_ENTRY_FUNC cf_FuncTrace_onEntry(__func__);
  #define CF_EXIT_FUNC  cf_FuncTrace_onLeave(__func__);
#else
  #define CF_ENTRY_FUNC
  #define CF_EXIT_FUNC
#endif


#ifdef WIN32
  #include <windows.h>
  static inline uint64_t cf_nowTicks() {
    LARGE_INTEGER m_nBeginTime;
    LARGE_INTEGER m_nFreq;
    QueryPerformanceFrequency(&m_nFreq);
    QueryPerformanceCounter(&m_nBeginTime);
    return (m_nBeginTime.QuadPart*1000000)/m_nFreq.QuadPart;
  }
#else
  #include <time.h>
  static inline uint64_t cf_nowTicks() {
    return clock();
  }
#endif

/**
 * entry a function
 */
void cf_FuncTrace_onEntry(const char *name);

/**
 * leave a function
 */
void cf_FuncTrace_onLeave(const char *name);

/**
 * dispose memory
 */
void cf_FuncTrace_dispose();

/**
 * print stack trace
 */
void cf_FuncTrace_printStackTrace();

/**
 * print performance profile count
 */
void cf_FuncTrace_printPerformance();

/**
 * get a formated trace string
 */
char *cf_FuncTrace_getTraceString();

#endif