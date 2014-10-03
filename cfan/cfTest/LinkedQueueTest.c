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

CF_BEGIN

void cf_LinkedQueueTest_test(void) {
  cf_LinkedQueue queue;
  int a = 1;
  int b = 2;
  int c = 3;
  int *value;
  CF_ENTRY_FUNC

  cf_LinkedQueue_make(&queue);
  cf_LinkedQueue_enqueue(&queue, &a);
  cf_LinkedQueue_enqueue(&queue, &b);
  cf_LinkedQueue_enqueue(&queue, &c);

  cf_LinkedQueue_dequeue(&queue, (void**)&value);
  cf_verify(*value == 1);

  cf_LinkedQueue_enqueue(&queue, &c);

  cf_LinkedQueue_dequeue(&queue, (void**)&value);
  cf_verify(*value == 2);

  cf_LinkedQueue_dequeue(&queue, (void**)&value);
  cf_verify(*value == 3);

  cf_LinkedQueue_dequeue(&queue, (void**)&value);
  cf_verify(*value == 3);

  cf_verify(cf_LinkedQueue_dequeue(&queue, (void**)&value) != cf_Error_ok);

  cf_LinkedQueue_dispose(&queue);

  CF_EXIT_FUNC
}

void cf_LinkedQueueTest_register(void) {
  cf_Test_add(cf_LinkedQueueTest_test);
}

CF_END
