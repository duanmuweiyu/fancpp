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

int data[1024];

static void *add(void *args) {
  cf_BlockingQueue *queue;
  int i;
  cf_Error err;
  queue = (cf_BlockingQueue *)args;
  for (i=0; i<10; ++i) {
    data[i] = i*10;
    printf("%d...\n", data[i]);
    err = cf_BlockingQueue_add(queue, data + i, cf_BlockingStrategy_blocking);
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
  cf_BlockingQueue *queue;

  queue = (cf_BlockingQueue *)args;
  while (true) {
    value = (int*)(cf_BlockingQueue_delete(queue));
    if (value == NULL) break;
    printf("%d<=q\n", *value);
    fflush(stdout);
  }
  printf("print thread exit\n");
  return 0;
}

void cf_BlockingQueueTest_test(void) {
  cf_BlockingQueue queue;
  thrd_t t1, t2;
  int i;
  CF_ENTRY_FUNC

  cf_BlockingQueue_make(&queue, 2, sizeof(int));

  thrd_create(&t1, (thrd_start_t)add, &queue);
  thrd_create(&t2, (thrd_start_t)print, &queue);

//  {
//    struct timespec ts;
//    clock_gettime(TIME_UTC, &ts);
//    ts.tv_sec++;

//    /* Sleep... */
//    printf("thread sleepping\n");
//    fflush(stdout);
//    fflush(stdout);
//    thrd_sleep(&ts, NULL);
//    printf("thread sleeped\n");
//    fflush(stdout);
//    fflush(stdout);
//  }

  for (i=0; i< 100000; ++i) {
    thrd_yield();
  }

  cf_BlockingQueue_cancel(&queue);

  thrd_join(t1, NULL);
  thrd_join(t2, NULL);

  cf_BlockingQueue_dispose(&queue);
  CF_EXIT_FUNC
}

void cf_BlockingQueueTest_register(void) {
  cf_Test_add(cf_BlockingQueueTest_test);
}

CF_END