/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-8-24  Jed Young  Creation
 */

#ifndef _CF_LINKEDLIST_H_
#define _CF_LINKEDLIST_H_

#include "cfan/Error.h"
#include "cfan/MemoryPool.h"

CF_BEGIN


/**
 * LinkedList template
 *
 */

#define cf_LinkedListTemplate(LinkedList, LinkedListElem) \
\
static inline void LinkedList##_add(LinkedList *self, LinkedListElem *elem) {\
  if (self->head == NULL || self->tail == NULL) {\
    self->head = elem;\
    self->tail = elem;\
    elem->next = NULL;\
    elem->previous = NULL;\
    return;\
  }\
\
  self->tail->next = elem;\
  elem->previous = self->tail;\
  elem->next = NULL;\
  self->tail = elem;\
}\
\
/** \
 * insert at first \
 */ \
static inline void LinkedList##_insert(LinkedList *self, LinkedListElem *elem) {\
  if (self->head == NULL || self->tail == NULL) {\
    self->head = elem;\
    self->tail = elem;\
    elem->next = NULL;\
    elem->previous = NULL;\
    return;\
  }\
\
  self->head->previous = elem;\
  elem->next = self->head;\
  elem->previous = NULL;\
  self->head = elem;\
}\
\
static inline void LinkedList##_insertBefore(LinkedList *self\
                                              , LinkedListElem *me, LinkedListElem *elem) {\
  cf_assert(self);\
  cf_assert(me);\
  cf_assert(elem);\
\
  if (self->head == me) {\
    self->head = elem;\
  } else {\
    cf_assert(me->previous);\
    me->previous->next = elem;\
  }\
\
  elem->next = me;\
  elem->previous = me->previous;\
  me->previous = elem;\
}\
\
static inline void LinkedList##_remove(LinkedList *self, LinkedListElem *elem) {\
  if (elem == NULL) return;\
  if (elem->previous) {\
    elem->previous->next = elem->next;\
  } else {\
    cf_assert(self->head == elem);\
    self->head = elem->next;\
  }\
\
  if (elem->next) {\
    elem->next->previous = elem->previous;\
  } else {\
    cf_assert(self->tail == elem);\
    self->tail = elem->previous;\
  }\
}\
\
static inline LinkedListElem *LinkedList##_get(LinkedList *self, int index) {\
  LinkedListElem *elem;\
  int i = 0;\
  elem = self->head;\
  while (elem && i<index) {\
    elem = elem->next;\
    ++i;\
  }\
  return elem;\
}\
\
static inline int LinkedList##_getSize(LinkedList *self) {\
  LinkedListElem *elem;\
  int i = 0;\
  elem = self->head;\
  while (elem) {\
    elem = elem->next;\
    ++i;\
  }\
  return i;\
}\
\
static inline void LinkedList##_freeElem(LinkedList *self, cf_MemoryPool *pool) {\
  LinkedListElem *elem;\
  LinkedListElem *prev;\
  elem = self->head;\
  while (elem) {\
    prev = elem;\
    elem = elem->next;\
    if (pool != NULL) {\
      cf_MemoryPool_free(pool, prev);\
    } else {\
      cf_free(prev);\
    }\
  }\
  self->head = NULL;\
  self->tail = NULL;\
}

/*========================================================================
 * predefine LinkedListe
 */

/**
 * LinkedList element
 */
typedef struct cf_LinkedListElem_ {
  void *value;
  struct cf_LinkedListElem_ *previous;
  struct cf_LinkedListElem_ *next;
} cf_LinkedListElem;

/**
 * LinkedList parent
 */
typedef struct cf_LinkedList_ {
  cf_LinkedListElem *head;
  cf_LinkedListElem *tail;
  cf_MemoryPool allocator;
} cf_LinkedList;

/**
 * default ctor
 */
static inline void cf_LinkedList_make(cf_LinkedList *self, size_t objCount) {
  self->head = NULL;
  self->tail = NULL;
  cf_MemoryPool_make(&self->allocator, sizeof(cf_LinkedListElem), objCount);
}

/**
 * define methods
 */
cf_LinkedListTemplate(cf_LinkedList, cf_LinkedListElem)

static inline void cf_LinkedList_dispose(cf_LinkedList *self) {
  //cf_LinkedList_dispose_(self, &self->allocator);
  cf_MemoryPool_dispose(&self->allocator);
}

/**
 * add to last
 */
static inline void cf_LinkedList_pushBack(cf_LinkedList *self, void *val) {
  cf_LinkedListElem *elem = (cf_LinkedListElem*)cf_MemoryPool_alloc(&self->allocator);
  elem->value = val;
  cf_LinkedList_add(self, elem);
}

/**
 * remove and return first obj
 */
static inline void *cf_LinkedList_removeFirst(cf_LinkedList *self) {
  cf_LinkedListElem *elem = self->head;
  if (elem == NULL) return NULL;

  cf_LinkedList_remove(self, elem);
  void *val = elem->value;
  cf_MemoryPool_free(&self->allocator, elem);
  return val;
}

CF_END

#endif
