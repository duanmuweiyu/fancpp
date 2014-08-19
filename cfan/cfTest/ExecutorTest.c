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

static void *print(void *args) {
  printf("**%d**\n", *((int *)args));
  fflush(stdout);
  return 0;
}

void cf_ExecutorTest_test(void) {
  cf_Executor executor;
  int i;
  int *val;
  cf_Error err;
  CF_ENTRY_FUNC

  err = cf_Executor_make(&executor, 4, 300);
  cf_verify(err == cf_Error_ok);

  val = (int*)cf_malloc(sizeof(int) * 10);
  for (i=0; i<10; ++i) {
    val[i] = i;
    //printf("---%d\n", *(val+i));
    cf_Executor_addTask(&executor, print, val+i);
  }

  printf("=====================\n");
  fflush(stdout);
  cf_Executor_dispose(&executor);
  cf_free(val);

  CF_EXIT_FUNC
}

void cf_ExecutorTest_register(void) {
  cf_Test_add(cf_ExecutorTest_test);
}

CF_END
