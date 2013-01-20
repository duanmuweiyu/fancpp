/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-20  Jed Young  Creation
 */

#ifndef _CF_EXECUTOR_H_
#define _CF_EXECUTOR_H_

#include "cfan/BlockQueue.h"

typedef struct cf_Executor_ {
  cf_BlockQueue taskQueue;
  thrd_t *threadList;
  size_t size;
  bool canceled;
} cf_Executor;

typedef struct cf_ExecutorTask_ {
  void *(*func)(void*);
  void *args;
} cf_ExecutorTask;

int *cf_Executor_thread_(void *arg);

inline cf_Error cf_Executor_make(cf_Executor *self, size_t taskSize, size_t threadSize) {
  cf_Error err;
  int terr;
  int i;

  err = cf_BlockQueue_make(&self->taskQueue, taskSize, sizeof(cf_ExecutorTask));
  if (err == cf_Error_ok) {
    self->threadList = (thrd_t *)cf_malloc(threadSize * sizeof(thrd_t *));
    self->canceled = false;
    for (i = 0; i < threadSize; ++ i)
    {
      terr = thrd_create(self->threadList+i, (thrd_start_t)cf_Executor_thread_, self);
      if (terr != thrd_success) {
        cf_BlockQueue_dispose(&self->taskQueue);
        self->canceled = true;
        return cf_Error_thread;
      }
    }
  }
  return err;
}

inline cf_Error cf_Executor_addTask(cf_Executor *self, void *(*func)(void*), void *args) {
  cf_ExecutorTask task = { func, args };
  return cf_BlockQueue_add(&self->taskQueue, &task);
}

inline void cf_Executor_dispose(cf_Executor *self) {
  cf_BlockQueue_dispose(&self->taskQueue);
  self->canceled = true;
}

#endif