/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-20  Jed Young  Creation
 */

#ifndef _CF_QUEUE_H_
#define _CF_QUEUE_H_

#include "cfan/Error.h"
#include "cfan/Memory.h"

#include <string.h>

typedef struct cf_Queue_ {
  char *buf;
  unsigned int elemSize;
  size_t capacity;
  size_t front;
  size_t rear;
} cf_Queue;

inline cf_Error cf_Queue_make(cf_Queue *self, size_t capacity, unsigned int elemSize) {
  self->capacity = capacity+1;//add one at here
  self->elemSize = elemSize;
  self->buf = (char*)cf_malloc(capacity * elemSize);
  self->front = 0;
  self->rear = 0;
  if (self->buf == NULL) {
    return cf_Error_alloc;
  }
  return cf_Error_ok;
}

inline size_t cf_Queue_size(cf_Queue *self) {
  if (self->rear > self->front) {
    return self->rear - self->front;
  } else if (self->rear == self->front) {
    return 0;
  } else if (self->rear < self->front) {
    return self->rear + (self->capacity - self->front);
  }
}

inline bool cf_Queue_isEmpty(cf_Queue *self) {
  if (self->front == self->rear) {
    return true;
  }
  return false;
}

inline cf_Error cf_Queue_add(cf_Queue *self, void *elem) {
  size_t npos;
  npos = (self->rear + 1) % self->capacity;
  if (npos == self->front) {
    return cf_Error_error;
  }
  memcpy(self->buf + (self->rear * self->elemSize), elem, self->elemSize);
  self->rear = npos;
  return cf_Error_ok;
}

inline void *cf_Queue_delete(cf_Queue *self) {
  void *elem;
  if (self->front == self->rear) {
    return NULL;
  }
  elem = self->buf + (self->front * self->elemSize);
  self->front = (self->front + 1) % self->capacity;
  return elem;
}

inline void *cf_Queue_peek(cf_Queue *self) {
  if (self->front == self->rear) {
    return NULL;
  }
  return self->buf + (self->front * self->elemSize);
}

inline void cf_Queue_dispose(cf_Queue *self) {
  cf_free(self->buf);
}

#endif