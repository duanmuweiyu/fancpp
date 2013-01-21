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

typedef struct cf_Executor_ {
  cf_BlockQueue taskQueue;
  thrd_t *threadList;
  size_t threadSize;
  //bool canceled;
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

inline cf_Error cf_Executor_addTask(cf_Executor *self, void *(*func)(void*), void *args) {
  cf_ExecutorTask task = { func, args };
  return cf_BlockQueue_add(&self->taskQueue, &task);
}

inline void cf_Executor_dispose(cf_Executor *self) {
  int i;
  int rc;
  //self->canceled = true;


  {
    struct timespec ts;
    clock_gettime(TIME_UTC, &ts);
    ts.tv_sec += 2;

    /* Sleep... */
    printf("thread sleepping\n");
    fflush(stdout);
    thrd_sleep(&ts, NULL);
    printf("thread sleeped\n");
    fflush(stdout);
  }

  cf_BlockQueue_cancel(&self->taskQueue);

  for (i = 0; i < self->threadSize; ++i)
  {
    rc = thrd_join(self->threadList[i], NULL);
    if (rc != thrd_success) {
      printf("thread join error\n");
      fflush(stdout);
    }
  }
  printf("will cf_BlockQueue_dispose\n");
  fflush(stdout);
  //cf_BlockQueue_dispose(&self->taskQueue);
}

#endif
