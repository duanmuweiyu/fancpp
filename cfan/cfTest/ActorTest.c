/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-9-7  Jed Young  Creation
 */

#include <stdio.h>
#include "cfan/cfan.h"

CF_BEGIN

static volatile int flag = 0;

static void receive(cf_Actor *self, cf_ActorMessage *msg) {
  int *count = (int *)msg->arg;
  (*count)++;
  msg->arg = NULL;
  ++flag;
  CF_UNUSED(self);
}

void cf_ActorTest_test(void) {
  cf_Executor executor;
  cf_Error err;
  cf_Actor actor;
  cf_ActorMessage msg;
  int i = 0;
  int count = 0;
  int const times = 1000;

  CF_ENTRY_FUNC

  err = cf_Executor_make(&executor, 4, 300);
  err = cf_Actor_make(&actor, &executor, receive, NULL);
  cf_assert(err == cf_Error_ok);

  for (i=0; i<times; ++i) {
    msg.name = "hello";
    msg.arg = &count;
    cf_Actor_send(&actor, &msg);
  }

  while (flag < times) {}

  cf_assert(count == times);

  cf_Executor_dispose(&executor);
  cf_Actor_dispose(&actor);

  CF_EXIT_FUNC
}

void cf_ActorTest_register(void) {
  cf_Test_add(cf_ActorTest_test);
}

CF_END