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

typedef struct cf_BlockQueue_ {
  cf_Queue queue;
  mtx_t mutex;
  cnd_t addCond;
  cnd_t deleteCond;
  bool cancelAdd;
  bool cancelDelete;
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
  while (!self->cancelAdd) {
    result = cf_Queue_add(&self->queue, elem);
    if (result != cf_Error_ok) {
      cnd_wait(&self->addCond, &self->mutex);
    } else {
      cnd_signal(&self->deleteCond);
      mtx_unlock(&self->mutex);
      return cf_Error_ok;
    }
  }
  cnd_broadcast(&self->addCond);
  mtx_unlock(&self->mutex);
  return cf_Error_error;
}

inline void cf_BlockQueue_cancel(cf_BlockQueue *self) {
  mtx_lock(&self->mutex);
  self->cancelAdd = true;
  self->cancelDelete = true;
  printf("queue canceled\n");
  fflush(stdout);
  cnd_broadcast(&self->addCond);
  cnd_broadcast(&self->deleteCond);
  mtx_unlock(&self->mutex);
}

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

  //broadcast other cancel delete
  rc = cnd_broadcast(&self->deleteCond);
  if (rc != thrd_success) {
    printf("cnd_broadcast error\n");
  }

  //unlock
  rc = mtx_unlock(&self->mutex);
  if (rc != thrd_success) {
    printf("mtx_unlock error\n");
  }
  return NULL;
}

inline void *cf_BlockQueue_peek(cf_BlockQueue *self) {
  register void *result;
  mtx_lock(&self->mutex);
  result = cf_Queue_peek(&self->queue);
  mtx_unlock(&self->mutex);
  return result;
}

inline void cf_BlockQueue_dispose(cf_BlockQueue *self) {
  //cf_BlockQueue_cancel(self);
  mtx_destroy(&self->mutex);
  cnd_destroy(&self->addCond);
  cnd_destroy(&self->deleteCond);
  cf_Queue_dispose(&self->queue);
  printf("queue disposed\n");
}

#endif
