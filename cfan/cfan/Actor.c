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

cf_Error cf_Actor_make(cf_Actor *self, cf_Executor *executor, cf_ActorReceive receive) {
  self->receive = receive;
  self->isRuning = false;
  self->executor = executor;

  cf_LinkedList_make(&self->queue.super);
  cf_MemoryPool_make(&self->queue.allocator, sizeof(cf_ActorMessage), 100);

  if (mtx_init(&self->mutex, mtx_recursive) != thrd_success) {
    return cf_Error_thread;
  }
  if (mtx_init(&self->allocMutex, mtx_recursive) != thrd_success) {
    return cf_Error_thread;
  }
  return cf_Error_ok;
}

void *cf_Actor_run(void *arg) {
  cf_Actor *self = (cf_Actor *)arg;
  do {
    mtx_lock(&self->mutex);
    cf_ActorMessage *msg = (cf_ActorMessage*)self->queue.super.head;
    if (msg == NULL) break;
    cf_LinkedList_remove(&self->queue.super, &msg->super);
    mtx_unlock(&self->mutex);
    self->receive(self, msg);
    mtx_lock(&self->allocMutex);
    cf_MemoryPool_free(&self->queue.allocator, msg);
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
  msgCopy = (cf_ActorMessage*)cf_MemoryPool_alloc(&self->queue.allocator);
  mtx_unlock(&self->allocMutex);
  cf_memcpy(msgCopy, amsg, sizeof(cf_ActorMessage));

  cf_LinkedList_add(&self->queue.super, &msgCopy->super);

  if (!self->isRuning) {
    cf_Executor_addTask(self->executor, cf_Actor_run, self);
    self->isRuning = true;
  }
  mtx_unlock(&self->mutex);
}

void cf_Actor_dispose(cf_Actor *self) {
  cf_Executor_dispose(self->executor);
  mtx_destroy(&self->mutex);
  cf_LinkedList_freeLinkedElem(&self->queue.super, &self->queue.allocator);
  cf_MemoryPool_dispose(&self->queue.allocator);
}
