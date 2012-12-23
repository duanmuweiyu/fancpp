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
#include "cfan.h"

int cf_LogTest_test(void)
{
  //int tmp = 0;
  CF_ENTRY_FUNC

  cf_Log_cfDebug("hello %f", 2.1);

  printf("Hello Wrold\n");

  CF_EXIT_FUNC
  return 0;
}

