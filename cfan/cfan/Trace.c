/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#include "cfan/Trace.h"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (*cf_Trace_sighandler) (int);
cf_Trace_sighandler cf_Trace_SIGABRT;
cf_Trace_sighandler cf_Trace_SIGFPE;
cf_Trace_sighandler cf_Trace_SIGILL;
cf_Trace_sighandler cf_Trace_SIGINT;
cf_Trace_sighandler cf_Trace_SIGSEGV;
cf_Trace_sighandler cf_Trace_SIGTERM;

static void printTrace(int i) {
  printf("received signal: %d\n", i);
  cf_Trace_printTrace();

  switch (i) {
  case SIGABRT:
    //if (cf_Trace_SIGABRT != NULL) cf_Trace_SIGABRT(i);
    //abort();
    break;
  case SIGFPE:
    if (cf_Trace_SIGFPE != NULL) cf_Trace_SIGFPE(i);
    else abort();
    break;
  case SIGILL:
    if (cf_Trace_SIGILL != NULL) cf_Trace_SIGILL(i);
    else abort();
    break;
  case SIGINT:
    if (cf_Trace_SIGINT != NULL) cf_Trace_SIGINT(i);
    else abort();
    break;
  case SIGSEGV:
    if (cf_Trace_SIGSEGV != NULL) cf_Trace_SIGSEGV(i);
    else abort();
    break;
  case SIGTERM:
    if (cf_Trace_SIGTERM != NULL) cf_Trace_SIGTERM(i);
    else abort();
    break;
  default:
    abort();
    break;
  }
}

void cf_Trace_traceOnExit() {
  atexit(cf_Trace_printTrace);
  cf_Trace_SIGABRT = signal(SIGABRT, printTrace);
  cf_Trace_SIGFPE = signal(SIGFPE, printTrace);
  cf_Trace_SIGILL = signal(SIGILL, printTrace);
  cf_Trace_SIGINT = signal(SIGINT, printTrace);
  cf_Trace_SIGSEGV = signal(SIGSEGV, printTrace);
  cf_Trace_SIGTERM = signal(SIGTERM, printTrace);
}
