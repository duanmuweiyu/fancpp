/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-20  Jed Young  Creation
 */

#ifndef _CF_BLOCKQUEUE_H_
#define _CF_BLOCKQUEUE_H_

#include "cfan/Queue.h"
#include "tinyCThread/tinycthread.h"
#include <stdio.h>

CF_BEGIN

/**
 * Blocking queue is a FIFO sequence.
 * Blocking when add to full queue or delete from empty queue.
 * It's like Golang channel.
 * This class is thread safe.
 *
 */
typedef struct cf_BlockingQueue_ {
  cf_Queue queue;
  mtx_t mutex;
  cnd_t addCond;
  cnd_t deleteCond;
  bool cancelAdd;
  bool cancelDelete;
} cf_BlockingQueue;

typedef enum cf_BlockingStrategy_ {
  cf_BlockingStrategy_donothing,
  cf_BlockingStrategy_blocking,
  cf_BlockingStrategy_removeLast,
  cf_BlockingStrategy_removeFirst
} cf_BlockingStrategy;

/**
 * constructor
 *
 */
cf_Error cf_BlockingQueue_make(cf_BlockingQueue *self, size_t capacity, unsigned int elemSize);

/**
 * current num of elements
 */
static inline size_t cf_BlockingQueue_size(cf_BlockingQueue *self) {
  register size_t size;
  mtx_lock(&self->mutex);
  size = cf_Queue_size(&self->queue);
  mtx_unlock(&self->mutex);
  return size;
}

/**
 * queue is empty
 */
static inline bool cf_BlockingQueue_isEmpty(cf_BlockingQueue *self) {
  register bool result;
  mtx_lock(&self->mutex);
  result = cf_Queue_isEmpty(&self->queue);
  mtx_unlock(&self->mutex);
  return result;
}

/**
 * push element to back.
 * If block arg is true and queue is full will be blocked.
 */
cf_Error cf_BlockingQueue_add(cf_BlockingQueue *self, void *elem, cf_BlockingStrategy strate);

/**
 * Release all blocked thread.
 */
static inline void cf_BlockingQueue_cancel(cf_BlockingQueue *self) {
  mtx_lock(&self->mutex);
  self->cancelAdd = true;
  self->cancelDelete = true;
  cnd_broadcast(&self->addCond);
  cnd_broadcast(&self->deleteCond);
  mtx_unlock(&self->mutex);
}

/**
 * pop out a front element.
 * If queue is empty will be blocked.
 */
void *cf_BlockingQueue_delete(cf_BlockingQueue *self);

/**
 * get first element but pop out.
 */
static inline void *cf_BlockingQueue_peek(cf_BlockingQueue *self) {
  register void *result;
  mtx_lock(&self->mutex);
  result = cf_Queue_peek(&self->queue);
  mtx_unlock(&self->mutex);
  return result;
}

/**
 * Destroy queue.
 * Must guarantee call cf_BlockingQueue_cancel before.
 */
static inline void cf_BlockingQueue_dispose(cf_BlockingQueue *self) {
  //cf_BlockingQueue_cancel(self);
  mtx_destroy(&self->mutex);
  cnd_destroy(&self->addCond);
  cnd_destroy(&self->deleteCond);
  cf_Queue_dispose(&self->queue);
  //printf("queue disposed\n");
}

CF_END

#endif
