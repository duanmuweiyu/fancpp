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

void printTime() {
  time_t tt;
  char tmpbuf[80];
  tt = time(NULL);
  strftime(tmpbuf,80,"%Y-%m-%d %H:%M:%S\n",localtime(&tt));
  printf("%s", tmpbuf);
}

static void onTime(void *arg) {
  int *count = (int *)arg;
  (*count)++;
  printf("%d\n", *count);
  printTime();
  fflush(stdout);

  CF_UNUSED(arg);
}

void cf_TimerTest_test(void) {
  cf_Error err;
  cf_Timer timer;
  cf_TimerEvent *event;
  int count = 0;
  CF_ENTRY_FUNC

  err = cf_Timer_make(&timer);
  cf_verify(err == cf_Error_ok);
  event = cf_new(cf_TimerEvent);
  event->delay = 0;
  event->interval = 1*1E9;
  event->repeat = 2;
  event->func = onTime;
  event->arg = &count;

  cf_Timer_add(&timer, event);
  cf_System_sleep(3*1000);
  cf_Timer_dispose(&timer);
  CF_EXIT_FUNC
}

void cf_TimerTest_register(void) {
  cf_Test_add(cf_TimerTest_test);
}

