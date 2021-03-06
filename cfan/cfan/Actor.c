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
#include "cfan/Str.h"

cf_Error cf_Actor_make(cf_Actor *self, cf_Executor *executor, cf_ActorReceive receive, cf_Timer *timer) {
  self->receive = receive;
  self->isRuning = false;
  self->executor = executor;
  self->timer = timer;
  self->mergeMessage = NULL;

  cf_LinkedList_make(&self->queue.super);
  cf_MemoryPool_make(&self->queue.allocator, sizeof(cf_ActorMessage), 100);

  if (mtx_init(&self->mutex, mtx_recursive) != thrd_success) {
    return cf_Error_thread;
  }

  return cf_Error_ok;
}

void *cf_Actor_run(void *arg) {
  cf_Actor *self = (cf_Actor *)arg;
  cf_ActorMessage message;
  do {
    mtx_lock(&self->mutex);
    cf_ActorMessage *msg = (cf_ActorMessage*)cf_LinkedList_removeFirst(&self->queue.super);
    if (msg == NULL) {
      mtx_unlock(&self->mutex);
      break;
    }
    message = *msg;
    cf_MemoryPool_free(&self->queue.allocator, msg);
    mtx_unlock(&self->mutex);

    self->receive(self, &message);

  } while (true);
  self->isRuning = false;
  return NULL;
}

void cf_Actor_send(cf_Actor *self, cf_ActorMessage *amsg) {
  cf_ActorMessage *msgCopy;
  cf_ActorMessage *elem;

  //copy
  mtx_lock(&self->mutex);
  msgCopy = (cf_ActorMessage*)cf_MemoryPool_alloc(&self->queue.allocator);
  cf_memcpy(msgCopy, amsg, sizeof(cf_ActorMessage));

  //merage message
  if (self->mergeMessage) {
    elem = (cf_ActorMessage*)cf_LinkedList_first(&self->queue.super);
    while (elem != (cf_ActorMessage*)cf_LinkedList_end(&self->queue.super)) {
      if (cf_Str_equals(elem->name, amsg->name)) {
        if ((*self->mergeMessage)(self, msgCopy, elem)) {
          cf_LinkedList_remove(&self->queue.super, &elem->super);
          cf_MemoryPool_free(&self->queue.allocator, elem);
        }
      }
      elem = (cf_ActorMessage*)elem->super.next;
    }
  }

  cf_LinkedList_add(&self->queue.super, &msgCopy->super);

  if (!self->isRuning) {
    cf_Executor_addTask(self->executor, cf_Actor_run, self);
    self->isRuning = true;
  }
  mtx_unlock(&self->mutex);
}

void cf_Actor_dispose(cf_Actor *self) {
//  if (self->timer) {
//    cf_Timer_dispose(self->timer);
//  }
  //cf_Executor_dispose(self->executor);
  mtx_lock(&self->mutex);
  cf_LinkedList_freeLinkedElem(&self->queue.super, &self->queue.allocator);
  cf_MemoryPool_dispose(&self->queue.allocator);
  mtx_unlock(&self->mutex);

  self->executor = NULL;
  mtx_destroy(&self->mutex);
}


struct cf_ActorSendLaterParam {
  cf_Actor *self;
  cf_ActorMessage *msg;
};

static void onTime(void *arg) {
  struct cf_ActorSendLaterParam *param = (struct cf_ActorSendLaterParam *)arg;
  cf_Actor_send(param->self, param->msg);
}

void cf_Actor_sendLater(cf_Actor *self, cf_ActorMessage *msg, uint64_t ns) {
  cf_TimerEvent *event;
  struct cf_ActorSendLaterParam *param;
  cf_ActorMessage *copymsg;
  cf_assert(self->timer);

  event = cf_new(cf_TimerEvent);
  event->delay = ns;
  event->interval = 0;
  event->repeat = 1;
  event->func = onTime;

  param = cf_new(struct cf_ActorSendLaterParam);
  copymsg = cf_new(cf_ActorMessage);
  *copymsg = *msg;
  param->self = self;
  param->msg = copymsg;
  event->arg = param;

  cf_Timer_add(self->timer, event);
}
