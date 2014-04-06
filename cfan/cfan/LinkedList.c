/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-8-24  Jed Young  Creation
 */

#include "cfan/LinkedList.h"
#include "cfan/MemoryPool.h"


void cf_LinkedList_add(cf_LinkedList*self, cf_LinkedListElem *elem) {
  if (self->head == NULL || self->tail == NULL) {
    self->head = elem;
    self->tail = elem;
    elem->next = NULL;
    elem->previous = NULL;
    return;
  }

  self->tail->next = elem;
  elem->previous = self->tail;
  elem->next = NULL;
  self->tail = elem;
}

/**
 * insert at first
 */
void cf_LinkedList_insert(cf_LinkedList*self, cf_LinkedListElem *elem) {
  if (self->head == NULL || self->tail == NULL) {
    self->head = elem;
    self->tail = elem;
    elem->next = NULL;
    elem->previous = NULL;
    return;
  }

  self->head->previous = elem;
  elem->next = self->head;
  elem->previous = NULL;
  self->head = elem;
}

void cf_LinkedList_insertBefore(cf_LinkedList*self
                                              , cf_LinkedListElem *me, cf_LinkedListElem *elem) {
  cf_assert(self);
  cf_assert(me);
  cf_assert(elem);

  if (self->head == me) {
    self->head = elem;
  } else {
    cf_assert(me->previous);
    me->previous->next = elem;
  }

  elem->next = me;
  elem->previous = me->previous;
  me->previous = elem;
}

void cf_LinkedList_remove(cf_LinkedList*self, cf_LinkedListElem *elem) {
  if (elem == NULL) return;
  if (elem->previous) {
    elem->previous->next = elem->next;
  } else {
    cf_assert(self->head == elem);
    self->head = elem->next;
  }

  if (elem->next) {
    elem->next->previous = elem->previous;
  } else {
    cf_assert(self->tail == elem);
    self->tail = elem->previous;
  }
}

cf_LinkedListElem *cf_LinkedList_get(cf_LinkedList*self, int index) {
  cf_LinkedListElem *elem;
  int i = 0;
  elem = self->head;
  while (elem && i<index) {
    elem = elem->next;
    ++i;
  }
  return elem;
}

int cf_LinkedList_getSize(cf_LinkedList*self) {
  cf_LinkedListElem *elem;
  int i = 0;
  elem = self->head;
  while (elem) {
    elem = elem->next;
    ++i;
  }
  return i;
}

void cf_LinkedList_freeLinkedElem(cf_LinkedList*self, struct cf_MemoryPool_ *pool) {
  cf_LinkedListElem *elem;
  cf_LinkedListElem *prev;
  elem = self->head;
  while (elem) {
    prev = elem;
    elem = elem->next;
    if (pool != NULL) {
      cf_MemoryPool_free(pool, prev);
    } else {
      cf_free(prev);
    }
  }
  self->head = NULL;
  self->tail = NULL;
}

cf_LinkedListElem *cf_LinkedList_removeFirst(cf_LinkedList *self) {
  cf_LinkedListElem *elem = self->head;
  if (elem == NULL) return NULL;

  cf_LinkedList_remove(self, elem);
  return elem;
}
