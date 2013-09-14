/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-9-14  Jed Young  Creation
 */

#include <stdio.h>
#include "cfan/cfan.h"

void cf_FileLogListenerTest_test(void) {
  //int tmp = 0;
  cf_FileLogListener *listener;
  CF_ENTRY_FUNC

  listener = (cf_FileLogListener*)cf_malloc(sizeof(cf_FileLogListener));
  cf_FileLogListener_make(listener, "./");
  cf_Log_addListener(&listener->super);

  cf_Log_debug(cf_Log_tag, "hello %f", 2.1);

  cf_Log_removeListener(listener->super.id);
  cf_FileLogListener_dispose(listener);

  CF_EXIT_FUNC
}

void cf_FileLogListenerTest_register(void) {
  cf_Test_add(cf_FileLogListenerTest_test);
}
