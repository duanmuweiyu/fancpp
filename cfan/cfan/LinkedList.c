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
  cf_LinkedListElem *left = self->tail.previous;
  cf_LinkedListElem *right = left->next;
  elem->next = right;
  right->previous = elem;
  elem->previous = left;
  left->next = elem;
}

/**
 * insert at first
 */
void cf_LinkedList_insert(cf_LinkedList*self, cf_LinkedListElem *elem) {
  cf_LinkedListElem *left = &self->head;
  cf_LinkedListElem *right = left->next;
  elem->next = right;
  right->previous = elem;
  elem->previous = left;
  left->next = elem;
}

void cf_LinkedList_insertBefore(cf_LinkedList*self
                                              , cf_LinkedListElem *me, cf_LinkedListElem *elem) {
  cf_assert(self);
  cf_assert(me);
  cf_assert(elem);
  CF_UNUSED(self);

  cf_LinkedListElem *left = me->previous;
  cf_LinkedListElem *right = me;
  elem->next = right;
  right->previous = elem;
  elem->previous = left;
  left->next = elem;
}

void cf_LinkedList_remove(cf_LinkedList*self, cf_LinkedListElem *elem) {
  if (elem == NULL) return;
  CF_UNUSED(self);
  elem->previous->next = elem->next;
  elem->next->previous = elem->previous;
}

cf_LinkedListElem *cf_LinkedList_get(cf_LinkedList*self, int index) {
  cf_LinkedListElem *elem;
  int i = 0;
  elem = self->head.next;
  while (elem != &self->tail) {
    if (i == index) {
      return elem;
    }
    elem = elem->next;
    ++i;
  }
  return NULL;
}

int cf_LinkedList_getSize(cf_LinkedList*self) {
  cf_LinkedListElem *elem;
  int i = 0;
  elem = self->head.next;
  while (elem != &self->tail) {
    ++i;
    elem = elem->next;
  }
  return i;
}

bool cf_LinkedList_isEmpty(cf_LinkedList *self) {
  return self->head.next == &self->tail;
}

void cf_LinkedList_freeLinkedElem(cf_LinkedList*self, struct cf_MemoryPool_ *pool) {
  cf_LinkedListElem *elem;
  while (self->head.next != &self->tail) {
    elem = self->head.next;
    self->head.next = elem->next;
    if (pool != NULL) {
      cf_MemoryPool_free(pool, elem);
    } else {
      cf_free(elem);
    }
  }
}

cf_LinkedListElem *cf_LinkedList_removeFirst(cf_LinkedList *self) {
  if (self->head.next == &self->tail) {
    return NULL;
  }
  cf_LinkedListElem *first = self->head.next;
  self->head.next = first->next;
  self->head.next->previous = &self->head;
  return first;
}
