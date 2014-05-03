/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-20  Jed Young  Creation
 */

#include <stdio.h>
#include "cfan/cfan.h"

void cf_QueueTest_test(void) {
  cf_Queue queue;
  int a = 1;
  int b = 2;
  int c = 3;
  size_t size;
  cf_Error err;
  int value;
  CF_ENTRY_FUNC

  cf_Queue_make(&queue, 2, sizeof(int));
  cf_Queue_add(&queue, &a);
  printf("%d, %d\n", (int)queue.front, (int)queue.rear);
  cf_Queue_add(&queue, &b);
  printf("%d, %d\n", (int)queue.front, (int)queue.rear);
  err = cf_Queue_add(&queue, &c);
  printf("%d, %d\n", (int)queue.front, (int)queue.rear);

  cf_verify(err != cf_Error_ok);

  size = cf_Queue_size(&queue);
  cf_verify(size == 2);

  value = *((int*)cf_Queue_delete(&queue));
  cf_verify(value == 1);

  size = cf_Queue_size(&queue);
  cf_verify(size == 1);

  cf_Queue_add(&queue, &c);
  size = cf_Queue_size(&queue);
  cf_verify(size == 2);

  value = *((int*)cf_Queue_delete(&queue));
  cf_verify(value == 2);

  value = *((int*)cf_Queue_delete(&queue));
  cf_verify(value == 3);

  cf_verify(cf_Queue_delete(&queue) == NULL);

  cf_Queue_dispose(&queue);

  CF_EXIT_FUNC
}

void cf_QueueTest_register(void) {
  cf_Test_add(cf_QueueTest_test);
}

