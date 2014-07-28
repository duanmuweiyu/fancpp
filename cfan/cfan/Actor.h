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

CF_BEGIN

/**
 * Message Queue
 */
typedef struct cf_ActorMessage_ {
  cf_LinkedListElem super;
  const char *name;
  void *arg;
} cf_ActorMessage;

typedef struct cf_ActorMessageQueue_ {
  cf_LinkedList super;
  cf_MemoryPool allocator;
} cf_ActorMessageQueue;

struct cf_Actor_;
typedef void (*cf_ActorReceive)(struct cf_Actor_ *, cf_ActorMessage *);

/**
 * Actor
 */
typedef struct cf_Actor_ {
  cf_ActorMessageQueue queue;
  cf_ActorReceive receive;
  bool isRuning;
  cf_Executor *executor;
  mtx_t mutex;
  mtx_t allocMutex;
} cf_Actor;


cf_Error cf_Actor_make(cf_Actor *self, cf_Executor *executor, cf_ActorReceive receive);

void cf_Actor_send(cf_Actor *self, cf_ActorMessage *msg);
void cf_Actor_dispose(cf_Actor*self);

CF_END

#endif
