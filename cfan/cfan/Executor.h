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

#include "cfan/BlockingQueue.h"
#include "cfan/Trace.h"
#include <stdio.h>

CF_BEGIN

/**
 * Executor is a thread pool.
 * using producer/consumer model.
 *
 */
typedef struct cf_Executor_ {
  cf_BlockingQueue taskQueue;
  thrd_t *threadList;
  size_t threadSize;
  bool onlyRunLatest;
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
cf_Error cf_Executor_make(cf_Executor *self, size_t taskSize, size_t threadSize);


/**
 * add a task with bloking startegy option
 */
static inline cf_Error cf_Executor_addTaskWithStrate(cf_Executor *self
                                                , void *(*func)(void*), void *args
                                                , cf_BlockingStrategy strate) {
  cf_ExecutorTask task = { func, args };
  return cf_BlockingQueue_add(&self->taskQueue, &task, strate);
}

/**
 * add a task
 */
static inline cf_Error cf_Executor_addTask(cf_Executor *self, void *(*func)(void*), void *args) {
  return cf_Executor_addTaskWithStrate(self, func, args, cf_BlockingStrategy_blocking);
}

/**
 * Destroy executor.
 * wait until all threads return.
 */
void cf_Executor_dispose(cf_Executor *self);

CF_END

#endif
