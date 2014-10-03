/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-20  Jed Young  Creation
 */

#ifndef _CF_TIMER_H_
#define _CF_TIMER_H_

#include "tinyCThread/tinycthread.h"
#include "cfan/LinkedList.h"

typedef void (*cf_TimerEventFunc)(void *);

/**
 * all time unit is nanosecond
 * @param repeat -1 repeat forever
 */
typedef struct cf_TimerEvent_ {
  cf_LinkedListElem super;
  uint64_t delay;
  uint64_t interval;
  uint64_t time_;
  int repeat;
  void *arg;
  cf_TimerEventFunc func;
} cf_TimerEvent;

/**
 * Timer be schedule to execute some task.
 */
typedef struct cf_Timer_ {
  cf_LinkedList list;
  mtx_t mutex;
  cnd_t condition;
  thrd_t thread;
  bool cancel;
} cf_Timer;

cf_Error cf_Timer_make(cf_Timer *self);

void cf_Timer_add(cf_Timer *self, cf_TimerEvent *event);

void cf_Timer_dispose(cf_Timer *self);

#endif // _CF_TIMER_H_
