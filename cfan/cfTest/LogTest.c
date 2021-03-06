/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#include <stdio.h>
#include "cfan/cfan.h"

CF_BEGIN

void cf_LogTest_test(void) {
  //int tmp = 0;
  CF_ENTRY_FUNC

  cf_Log_debug(cf_Log_tag, "hello %f", 2.1);

  printf("Hello Wrold\n");

  CF_EXIT_FUNC
}

void cf_LogTest_register(void) {
  cf_Test_add(cf_LogTest_test);
}

CF_END