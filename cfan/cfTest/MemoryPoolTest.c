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

void cf_MemoryPoolTest_test(void) {
  char *pointer1;
  char *pointer2;
  char *pointer3;
  char *pointer4;
  cf_MemoryPool pool;
  CF_ENTRY_FUNC

  cf_MemoryPool_make(&pool, 255, 2);

  pointer1 = (char *)cf_MemoryPool_alloc(&pool);
  pointer2 = (char *)cf_MemoryPool_alloc(&pool);
  cf_MemoryPool_free(&pool, pointer1);
  pointer3 = (char *)cf_MemoryPool_alloc(&pool);
  pointer4 = (char *)cf_MemoryPool_alloc(&pool);

  cf_MemoryPool_free(&pool, pointer2);
  cf_MemoryPool_free(&pool, pointer3);
  cf_MemoryPool_free(&pool, pointer4);

  cf_Log_info(cf_Log_tag, "will dispose\n");
  cf_MemoryPool_dispose(&pool);

  CF_EXIT_FUNC
}

void cf_MemoryPoolTest_register(void) {
  cf_Test_add(cf_MemoryPoolTest_test);
}

CF_END
