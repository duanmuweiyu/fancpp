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
#include "cfan/Object.h"

CF_BEGIN

struct cf_MemoryPool_;

/**
 * LinkedList
 *
 */
/**
 * LinkedList element
 */
typedef struct cf_LinkedListElem_ {
  cf_Object super;
  struct cf_LinkedListElem_ *previous;
  struct cf_LinkedListElem_ *next;
  void *value;
  //...more user data
} cf_LinkedListElem;

/**
 * LinkedList parent
 */
typedef struct cf_LinkedList_ {
  cf_Object super;
  cf_LinkedListElem *head;
  cf_LinkedListElem *tail;
} cf_LinkedList;

/**
 * default ctor
 */
static inline void cf_LinkedList_make(cf_LinkedList *self) {
  self->super.vtable = NULL;
  self->super.refCount = 0;
  self->head = NULL;
  self->tail = NULL;
}

/**
 * push back
 */
void cf_LinkedList_add(cf_LinkedList *self, cf_LinkedListElem *elem);

/**
 * insert at first
 */
void cf_LinkedList_insert(cf_LinkedList *self, cf_LinkedListElem *elem);

/**
 * insert before element
 */
void cf_LinkedList_insertBefore(cf_LinkedList *self
                                              , cf_LinkedListElem *me, cf_LinkedListElem *elem);

/**
 * remove element from self.
 * you must free elem memory by cf_LinkedList_freeElem
 */
void cf_LinkedList_remove(cf_LinkedList *self, cf_LinkedListElem *elem);

/**
 * return the element at index positon
 */
cf_LinkedListElem *cf_LinkedList_get(cf_LinkedList *self, int index);

/**
 * return elements count
 */
int cf_LinkedList_getSize(cf_LinkedList *self);

/**
 * free all elem ref in list.
 * memory pool is optional if NULL
 */
void cf_LinkedList_freeLinkedElem(cf_LinkedList *self, struct cf_MemoryPool_ *pool);

/**
 * remove and return first element
 */
cf_LinkedListElem *cf_LinkedList_removeFirst(cf_LinkedList *self);

CF_END

#endif
