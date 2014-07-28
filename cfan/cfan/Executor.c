/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-20  Jed Young  Creation
 */

#include "cfan/Executor.h"

cf_Error cf_Executor_make(cf_Executor *self, size_t taskSize, size_t threadSize) {
  cf_Error err;
  int terr;
  size_t i;
  CF_ENTRY_FUNC

  err = cf_BlockingQueue_make(&self->taskQueue, taskSize, sizeof(cf_ExecutorTask));
  if (err == cf_Error_ok) {
    self->threadList = (thrd_t *)cf_malloc(threadSize * sizeof(thrd_t));
    //self->canceled = false;
    self->threadSize = threadSize;
    for (i = 0; i < threadSize; ++i)
    {
      terr = thrd_create(self->threadList+i, (thrd_start_t)cf_Executor_thread_, self);
      if (terr != thrd_success) {
        cf_BlockingQueue_dispose(&self->taskQueue);
        //self->canceled = true;
        CF_EXIT_FUNC
        return cf_Error_thread;
      }
    }
  }
  CF_EXIT_FUNC
  return err;
}

int *cf_Executor_thread_(void *arg) {
  cf_Executor *self;
  cf_ExecutorTask *task;
  self = (cf_Executor *)arg;

  //printf("thread %d run\n", thrd_current());
  //fflush(stdout);
  while (true) {
    task = (cf_ExecutorTask *)cf_BlockingQueue_delete(&self->taskQueue);
    if (task != NULL) {
      (*task->func)(task->args);
    } else {
      break;
    }
  }

  //printf("thread %d exit\n", thrd_current());
  //fflush(stdout);
  //thrd_exit(0);
  return 0;
}

void cf_Executor_dispose(cf_Executor *self) {
  size_t i;
  int rc;

  cf_BlockingQueue_cancel(&self->taskQueue);

  for (i = 0; i < self->threadSize; ++i)
  {
    rc = thrd_join(self->threadList[i], NULL);
    if (rc != thrd_success) {
      printf("thread join error\n");
    }
  }

  cf_BlockingQueue_dispose(&self->taskQueue);
  cf_free(self->threadList);
}
