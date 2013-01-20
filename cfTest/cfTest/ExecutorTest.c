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

static void *print(void *args) {
  printf("%d\n", (char *)args);
  return 0;
}

void cf_ExecutorTest_test(void) {
  cf_Executor executor;
  int i;
  int *val;
  CF_ENTRY_FUNC

  cf_Executor_make(&executor, 100, 70);

  val = (int*)cf_malloc(sizeof(int) * 1000);
  for (i=0; i<1000; ++i) {
    *(val+i) = i;
    cf_Executor_addTask(&executor, print, val+i);
  }
  cf_free(val);
  cf_Executor_dispose(&executor);

  CF_EXIT_FUNC
}

void cf_ExecutorTest_register(void) {
  cf_Test_add(cf_ExecutorTest_test);
}

