/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-9-7  Jed Young  Creation
 */

#include "cfan/Actor.h"


void *cf_Actor_run(void *arg) {
  cf_Actor *self = (cf_Actor *)arg;
  do {
    mtx_lock(&self->mutex);
    cf_ActorMessage *msg = self->queue.head;
    if (msg == NULL) break;
    cf_ActorMessageQueue_remove(&self->queue, msg);
    mtx_unlock(&self->mutex);
    self->receive(self, msg);
    mtx_lock(&self->allocMutex);
    cf_MemoryPool_free(&self->msgFacory, msg);
    mtx_unlock(&self->allocMutex);
  } while (true);
  self->isRuning = false;
  mtx_unlock(&self->mutex);
  return NULL;
}

void cf_Actor_send(cf_Actor *self, cf_ActorMessage *amsg) {
  cf_ActorMessage *msgCopy;

  //copy
  mtx_lock(&self->mutex);
  mtx_lock(&self->allocMutex);
  msgCopy = (cf_ActorMessage*)cf_MemoryPool_alloc(&self->msgFacory);
  mtx_unlock(&self->allocMutex);
  memcpy(msgCopy, amsg, sizeof(cf_ActorMessage));

  cf_ActorMessageQueue_add(&self->queue, msgCopy);

  if (!self->isRuning) {
    cf_Executor_addTask(self->executor, cf_Actor_run, self);
    self->isRuning = true;
  }
  mtx_unlock(&self->mutex);
}

void cf_Actor_dispose(cf_Actor *self) {
  cf_Executor_dispose(self->executor);
  mtx_destroy(&self->mutex);
  cf_ActorMessageQueue_freeElem(&self->queue, &self->msgFacory);
  cf_MemoryPool_dispose(&self->msgFacory);
}
