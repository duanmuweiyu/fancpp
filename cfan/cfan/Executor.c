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


int *cf_Executor_thread_(void *arg) {
  cf_Executor *self;
  cf_ExecutorTask *task;
  self = (cf_Executor *)arg;

  //printf("thread %ul run\n", thrd_current());
  //printf("thread %d run\n", thrd_current());
  fflush(stdout);
  while (true) {
    task = (cf_ExecutorTask *)cf_BlockQueue_delete(&self->taskQueue);
    if (task != NULL) {
      (*task->func)(task->args);
    } else {
      break;
    }
  }

  //printf("thread %d exit\n", thrd_current());
  fflush(stdout);
  //thrd_exit(0);
  return 0;
}


