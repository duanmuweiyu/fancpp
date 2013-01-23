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
 * Block queue is a FIFO sequence.
 * Blocked when add to full queue or delete from empty queue.
 * It's like Golang channel.
 *
 */
typedef struct cf_BlockQueue_ {
  cf_Queue queue;
  mtx_t mutex;
  cnd_t addCond;
  cnd_t deleteCond;
  bool cancelAdd;
  bool cancelDelete;
} cf_BlockQueue;

/**
 * constructor
 *
 */
inline cf_Error cf_BlockQueue_make(cf_BlockQueue *self, size_t capacity, unsigned int elemSize) {
  cf_Error err;
  int terr;
  err = cf_Queue_make(&self->queue, capacity, elemSize);
  if (err == cf_Error_ok) {
    terr = mtx_init(&self->mutex, mtx_plain);
    if (terr != thrd_success) {
      return cf_Error_thread;
    }
    terr = cnd_init(&self->addCond);
    if (terr != thrd_success) {
      return cf_Error_thread;
    }
    terr = cnd_init(&self->deleteCond);
    if (terr != thrd_success) {
      return cf_Error_thread;
    }
  }
  self->cancelAdd = false;
  self->cancelDelete = false;
  return err;
}

/**
 * current num of elements
 */
inline size_t cf_BlockQueue_size(cf_BlockQueue *self) {
  register size_t size;
  mtx_lock(&self->mutex);
  size = cf_Queue_size(&self->queue);
  mtx_unlock(&self->mutex);
  return size;
}

/**
 * queue is empty
 */
inline bool cf_BlockQueue_isEmpty(cf_BlockQueue *self) {
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
inline cf_Error cf_BlockQueue_add(cf_BlockQueue *self, void *elem, bool block) {
  register cf_Error result;
  mtx_lock(&self->mutex);
  while (!self->cancelAdd) {
    result = cf_Queue_add(&self->queue, elem);
    if (result != cf_Error_ok) {
      if (block) {
        cnd_wait(&self->addCond, &self->mutex);
      } else {
        mtx_unlock(&self->mutex);
        return cf_Error_overflow;
      }
    } else {
      cnd_signal(&self->deleteCond);
      mtx_unlock(&self->mutex);
      return cf_Error_ok;
    }
  }
  mtx_unlock(&self->mutex);
  return cf_Error_error;
}

/**
 * Release all blocked thread.
 */
inline void cf_BlockQueue_cancel(cf_BlockQueue *self) {
  mtx_lock(&self->mutex);
  self->cancelAdd = true;
  self->cancelDelete = true;
  //printf("queue canceled\n");
  fflush(stdout);
  cnd_broadcast(&self->addCond);
  cnd_broadcast(&self->deleteCond);
  mtx_unlock(&self->mutex);
}

/**
 * pop out a front element.
 * If queue is empty will be blocked.
 */
inline void *cf_BlockQueue_delete(cf_BlockQueue *self) {
  register void *result;
  int rc;
  rc = mtx_lock(&self->mutex);
  if (rc != thrd_success) {
    printf("mtx_lock error\n");
  }
  while (!self->cancelDelete) {
    result = cf_Queue_delete(&self->queue);
    if (result == NULL) {
      rc = cnd_wait(&self->deleteCond, &self->mutex);
      if (rc != thrd_success) {
        printf("cnd_wait error\n");
      }
    } else {
      rc = cnd_signal(&self->addCond);
      if (rc != thrd_success) {
        printf("cnd_signal error\n");
      }
      rc = mtx_unlock(&self->mutex);
      if (rc != thrd_success) {
        printf("mtx_unlock error\n");
      }
      return result;
    }
  }

  //unlock
  rc = mtx_unlock(&self->mutex);
  if (rc != thrd_success) {
    printf("mtx_unlock error\n");
  }
  return NULL;
}

/**
 * get first element but pop out.
 */
inline void *cf_BlockQueue_peek(cf_BlockQueue *self) {
  register void *result;
  mtx_lock(&self->mutex);
  result = cf_Queue_peek(&self->queue);
  mtx_unlock(&self->mutex);
  return result;
}

/**
 * Destroy queue.
 * Must guarantee call cf_BlockQueue_cancel before.
 */
inline void cf_BlockQueue_dispose(cf_BlockQueue *self) {
  //cf_BlockQueue_cancel(self);
  mtx_destroy(&self->mutex);
  cnd_destroy(&self->addCond);
  cnd_destroy(&self->deleteCond);
  cf_Queue_dispose(&self->queue);
  //printf("queue disposed\n");
}

CF_END

#endif
