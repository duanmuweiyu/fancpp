/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-9-7  Jed Young  Creation
 */

#ifndef _CF_ACTOR_H_
#define _CF_ACTOR_H_

#include "cfan/Error.h"
#include "cfan/LinkedList.h"
#include "cfan/Executor.h"
#include "cfan/MemoryPool.h"
#include "cfan/Timer.h"

CF_BEGIN

/**
 * Message struct
 */
typedef struct cf_ActorMessage_ {
  cf_LinkedListElem super;
  const char *name;
  void *arg;
} cf_ActorMessage;

/**
 * Message Queue
 */
typedef struct cf_ActorMessageQueue_ {
  cf_LinkedList super;
  cf_MemoryPool allocator;
} cf_ActorMessageQueue;

struct cf_Actor_;
typedef void (*cf_ActorReceive)(struct cf_Actor_ *, cf_ActorMessage *);

/**
 * Actor
 * a actor is a message queue and thread pool
 * send message by 'send' or 'sendLater'
 * receive will callback to handle message.
 */
typedef struct cf_Actor_ {
  cf_ActorMessageQueue queue;
  cf_ActorReceive receive;
  bool isRuning;
  cf_Executor *executor;
  cf_Timer *timer;
  mtx_t mutex;
  mtx_t allocMutex;
} cf_Actor;

/**
 * @param timer NULl if not call sendLater
 */
cf_Error cf_Actor_make(cf_Actor *self, cf_Executor *executor, cf_ActorReceive receive, cf_Timer *timer);

/**
 * send message to actor.
 * the msg memory will be copy.
 */
void cf_Actor_send(cf_Actor *self, cf_ActorMessage *msg);

/**
 * schedule message and call later.
 * @param msg message to send.
 * @param ns nanoTime to delay.
 */
void cf_Actor_sendLater(cf_Actor *self, cf_ActorMessage *msg, uint64_t ns);

/**
 * destory actor content.
 * blok until other thread exit.
 */
void cf_Actor_dispose(cf_Actor*self);

CF_END

#endif
