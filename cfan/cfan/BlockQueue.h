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

typedef struct cf_BlockQueue_ {
  cf_Queue queue;
  mtx_t mutex;
  cnd_t cond;
} cf_BlockQueue;

inline cf_Error cf_BlockQueue_make(cf_BlockQueue *self, size_t capacity, unsigned int elemSize) {
  cf_Error err;
  int terr;
  err = cf_Queue_make(&self->queue, capacity, elemSize);
  if (err == cf_Error_ok) {
    terr = mtx_init(&self->mutex, mtx_plain);
    if (terr != thrd_success) {
      return cf_Error_thread;
    }
    terr = cnd_init(&self->cond);
    if (terr != thrd_success) {
      return cf_Error_thread;
    }
  }
  return err;
}

inline size_t cf_BlockQueue_size(cf_BlockQueue *self) {
  register size_t size;
  mtx_lock(&self->mutex);
  size = cf_Queue_size(&self->queue);
  mtx_unlock(&self->mutex);
  return size;
}

inline bool cf_BlockQueue_isEmpty(cf_BlockQueue *self) {
  register bool result;
  mtx_lock(&self->mutex);
  result = cf_Queue_isEmpty(&self->queue);
  mtx_unlock(&self->mutex);
  return result;
}

inline cf_Error cf_BlockQueue_add(cf_BlockQueue *self, void *elem) {
  register cf_Error result;
  mtx_lock(&self->mutex);
  do {
    result = cf_Queue_add(&self->queue, elem);
    if (result != cf_Error_ok) {
      cnd_wait(&self->cond, &self->mutex);
    } else {
      cnd_broadcast(&self->cond);
      mtx_unlock(&self->mutex);
      return cf_Error_ok;
    }
  } while (0);
}

inline void *cf_BlockQueue_delete(cf_BlockQueue *self) {
  register void *result;
  mtx_lock(&self->mutex);
  do {
    result = cf_Queue_delete(&self->queue);
    if (result == NULL) {
      cnd_wait(&self->cond, &self->mutex);
    } else {
      cnd_broadcast(&self->cond);
      mtx_unlock(&self->mutex);
      return result;
    }
  } while (0);
}

inline void *cf_BlockQueue_peek(cf_BlockQueue *self) {
  register void *result;
  mtx_lock(&self->mutex);
  result = cf_Queue_peek(&self->queue);
  mtx_unlock(&self->mutex);
  return result;
}

inline void cf_BlockQueue_dispose(cf_BlockQueue *self) {
  cf_Queue_dispose(&self->queue);
  mtx_destroy(&self->mutex);
  cnd_destroy(&self->cond);
}

#endif