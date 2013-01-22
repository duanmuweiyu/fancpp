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
#include <stdio.h>

CF_BEGIN

/**
 * Executor is a thread pool.
 * using producer/consumer model.
 *
 */
typedef struct cf_Executor_ {
  cf_BlockQueue taskQueue;
  thrd_t *threadList;
  size_t threadSize;
} cf_Executor;

/**
 * Task is a unexecuted job.
 */
typedef struct cf_ExecutorTask_ {
  void *(*func)(void*);
  void *args;
} cf_ExecutorTask;

/**
 * execute function.
 */
int *cf_Executor_thread_(void *arg);

/**
 * construcotr
 */
inline cf_Error cf_Executor_make(cf_Executor *self, size_t taskSize, size_t threadSize) {
  cf_Error err;
  int terr;
  size_t i;
  CF_ENTRY_FUNC

  err = cf_BlockQueue_make(&self->taskQueue, taskSize, sizeof(cf_ExecutorTask));
  if (err == cf_Error_ok) {
    self->threadList = (thrd_t *)cf_malloc(threadSize * sizeof(thrd_t *));
    //self->canceled = false;
    self->threadSize = threadSize;
    for (i = 0; i < threadSize; ++i)
    {
      terr = thrd_create(self->threadList+i, (thrd_start_t)cf_Executor_thread_, self);
      if (terr != thrd_success) {
        cf_BlockQueue_dispose(&self->taskQueue);
        //self->canceled = true;
        CF_EXIT_FUNC
        return cf_Error_thread;
      }
    }
  }
  CF_EXIT_FUNC
  return err;
}

/**
 * add a task
 */
inline cf_Error cf_Executor_addTask(cf_Executor *self, void *(*func)(void*), void *args) {
  cf_ExecutorTask task = { func, args };
  return cf_BlockQueue_add(&self->taskQueue, &task, true);
}

/**
 * Destroy executor.
 * wait until all threads return.
 */
inline void cf_Executor_dispose(cf_Executor *self) {
  size_t i;
  int rc;

  cf_BlockQueue_cancel(&self->taskQueue);

  for (i = 0; i < self->threadSize; ++i)
  {
    rc = thrd_join(self->threadList[i], NULL);
    if (rc != thrd_success) {
      printf("thread join error\n");
      fflush(stdout);
    }
  }

  cf_BlockQueue_dispose(&self->taskQueue);
}

CF_END

#endif
