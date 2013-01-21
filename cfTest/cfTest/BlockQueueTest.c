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

int data[1024];

static void *add(void *args) {
  cf_BlockQueue *queue;
  int i;
  cf_Error err;
  queue = (cf_BlockQueue *)args;
  for (i=0; i<10; ++i) {
    data[i] = i*10;
    printf("%d...\n", data[i]);
    err = cf_BlockQueue_add(queue, data + i);
    if (err != cf_Error_ok) {
      break;
    }
    printf("%d=>q\n", data[i]);
    fflush(stdout);
  }
  printf("add thread exit\n");
  return 0;
}

static void *print(void *args) {
  int *value;
  cf_BlockQueue *queue;

  queue = (cf_BlockQueue *)args;
  while (true) {
    value = (int*)(cf_BlockQueue_delete(queue));
    if (value == NULL) break;
    printf("%d<=q\n", *value);
    fflush(stdout);
  }
  printf("print thread exit\n");
  return 0;
}

void cf_BlockQueueTest_test(void) {
  cf_BlockQueue queue;
  thrd_t t1, t2;
  CF_ENTRY_FUNC

  cf_BlockQueue_make(&queue, 2, sizeof(int));

  thrd_create(&t1, (thrd_start_t)add, &queue);
  thrd_create(&t2, (thrd_start_t)print, &queue);

  {
    struct timespec ts;
    clock_gettime(TIME_UTC, &ts);
    ts.tv_sec++;

    /* Sleep... */
    printf("thread sleepping\n");
    fflush(stdout);
    fflush(stdout);
    thrd_sleep(&ts, NULL);
    printf("thread sleeped\n");
    fflush(stdout);
    fflush(stdout);
  }

  cf_BlockQueue_cancel(&queue);

  thrd_join(t1, NULL);
  thrd_join(t2, NULL);

  cf_BlockQueue_dispose(&queue);
  CF_EXIT_FUNC
}

void cf_BlockQueueTest_register(void) {
  cf_Test_add(cf_BlockQueueTest_test);
}

