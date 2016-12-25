/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-20  Jed Young  Creation
 */

#ifndef _CF_LINKEDQUEUE_H_
#define _CF_LINKEDQUEUE_H_

#include "cfan/Error.h"
#include "cfan/Memory.h"
#include "cfan/Util.h"

#include "tinyCThread/tinycthread.h"

CF_BEGIN

typedef struct cf_LinkedQueue_Node_ {
  void *pointer;
  struct cf_LinkedQueue_Node_ *next;
} cf_LinkedQueue_Node;

/**
 * LinkedQueue is a FIFO sequence
 * one thread dequeue or one thread enquque is thread safe.
 */
typedef struct cf_LinkedQueue_ {
  cf_LinkedQueue_Node *head;
  cf_LinkedQueue_Node *tail;
  mtx_t mutex;
} cf_LinkedQueue;

cf_Error cf_LinkedQueue_make(cf_LinkedQueue *self);

/**
 * get and remove front of queue.
 * @return cf_Error_null if queue is empty.
 */
cf_Error cf_LinkedQueue_dequeue(cf_LinkedQueue *self, void **out);

/**
 * add to the last of queue
 */
void cf_LinkedQueue_enqueue(cf_LinkedQueue *self, void *ptr);

/**
 * Thread safe dequeue
 */
cf_Error cf_LinkedQueue_safeDequeue(cf_LinkedQueue *self, void **out);

/**
 * Thread safe enqueue
 */
void cf_LinkedQueue_safeEnqueue(cf_LinkedQueue *self, void *ptr);

void cf_LinkedQueue_dispose(cf_LinkedQueue *self);

CF_END
#endif // _CF_LINKEDQUEUE_H_
