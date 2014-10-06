/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-20  Jed Young  Creation
 */

#include "cfan/BlockingQueue.h"

cf_Error cf_BlockingQueue_make(cf_BlockingQueue *self, size_t capacity, unsigned int elemSize) {
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
  self->onRemove = NULL;
  return err;
}

cf_Error cf_BlockingQueue_add(cf_BlockingQueue *self, void *elem, cf_BlockingStrategy strate) {
  register cf_Error result;
  void *tempElem;
  mtx_lock(&self->mutex);
  while (!self->cancelAdd) {
    result = cf_Queue_add(&self->queue, elem);
    if (result != cf_Error_ok) {
      switch(strate) {
      case cf_BlockingStrategy_donothing:
        mtx_unlock(&self->mutex);
        return cf_Error_overflow;
      case cf_BlockingStrategy_blocking:
        cnd_wait(&self->addCond, &self->mutex);
        break;
      case cf_BlockingStrategy_removeLast:
        tempElem = cf_Queue_removeLast(&self->queue);
        if (self->onRemove) {
          (*self->onRemove)(tempElem);
        }
        break;
      case cf_BlockingStrategy_removeFirst:
        tempElem = cf_Queue_delete(&self->queue);
        if (self->onRemove) {
          (*self->onRemove)(tempElem);
        }
        break;
      default:
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

void *cf_BlockingQueue_getAndClear(cf_BlockingQueue *self) {
  void *result;
  result = cf_BlockingQueue_delete(self);
  while (!cf_BlockingQueue_isEmpty(self)) {
    if (self->onRemove) {
      (*self->onRemove)(result);
    }
    result = cf_BlockingQueue_delete(self);
  }
  return result;
}

void *cf_BlockingQueue_delete(cf_BlockingQueue *self) {
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
