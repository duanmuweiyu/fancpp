/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-8-24  Jed Young  Creation
 */

#include <stdio.h>
#include "cfan/cfan.h"

CF_BEGIN

static void verifyLink(cf_LinkedList *list) {
  cf_LinkedListElem *elem;

  CF_ENTRY_FUNC

  cf_assert(list->head.previous == NULL);
  cf_assert(list->tail.next == NULL);

  elem = cf_LinkedList_first(list);
  while (elem != cf_LinkedList_end(list)) {
    cf_assert(elem->previous->next == elem);
    elem = elem->next;
  }

  CF_EXIT_FUNC
}

void cf_LinkedListTest_testAdd(void) {
  cf_LinkedList list;
  cf_LinkedListElem elem0;
  cf_LinkedListElem elem1;
  cf_LinkedListElem elem2;
  cf_LinkedListElem elem3;
  cf_LinkedListElem *elem;
  int a = 1;
  int b = 2;
  int c = 3;
  int d = 4;
  int size;
  CF_ENTRY_FUNC

  cf_LinkedList_make(&list);
  elem0.value = &a;
  elem1.value = &b;
  elem2.value = &c;
  elem3.value = &d;


  cf_LinkedList_add(&list, &elem0);
  cf_LinkedList_add(&list, &elem1);
  size = cf_LinkedList_getSize(&list);
  cf_verify(size == 2);
  verifyLink(&list);

  cf_LinkedList_insert(&list, &elem2);
  size = cf_LinkedList_getSize(&list);
  cf_verify(size == 3);
  elem = cf_LinkedList_get(&list, 0);
  cf_verify(elem == &elem2);
  verifyLink(&list);

  cf_LinkedList_insertBefore(&list, &elem1, &elem3);
  size = cf_LinkedList_getSize(&list);
  cf_verify(size == 4);
  elem = cf_LinkedList_get(&list, 2);
  cf_verify(elem == &elem3);
  verifyLink(&list);

  cf_LinkedList_remove(&list, &elem0);
  size = cf_LinkedList_getSize(&list);
  cf_verify(size == 3);
  verifyLink(&list);

  //cf_LinkedList_dispose(&list);

  CF_EXIT_FUNC
}

void cf_LinkedListTest_register(void) {
  cf_Test_add(cf_LinkedListTest_testAdd);
}

CF_END
