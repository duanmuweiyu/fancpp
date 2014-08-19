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

void cf_MemoryTest_testBase(void) {
  char *pointer;
  CF_ENTRY_FUNC

  pointer = (char *)cf_malloc(100);

  cf_dumpMem();
  cf_free(pointer);
  cf_dumpMem();

  CF_EXIT_FUNC
}

void cf_MemoryTest_testRealloc(void) {
  char *pointer1;
  char *pointer2;
  char *pointer3;
  CF_ENTRY_FUNC

  pointer1 = (char *)cf_malloc(100);
  pointer2 = (char *)cf_malloc(100);
  pointer3 = (char *)cf_malloc(100);

  cf_checkAllMem();
  pointer2 = (char *)cf_realloc(pointer2, 5000);
  cf_checkAllMem();

  cf_free(pointer1);
  cf_checkAllMem();
  cf_free(pointer3);
  cf_checkAllMem();
  cf_free(pointer2);
  cf_checkAllMem();

  CF_EXIT_FUNC
}

void cf_MemoryTest_testAll(void) {
  char *pointer1;
  char *pointer2;
  char *pointer3;
  char *pointer4;
  CF_ENTRY_FUNC

  pointer1 = (char *)cf_malloc(100);
  pointer2 = (char *)cf_calloc(10,5);
  pointer3 = (char *)cf_malloc(100);
  pointer4 = (char *)cf_malloc(100);

  pointer4 = (char *)cf_realloc(pointer4, 120);
  pointer1 = (char *)cf_realloc(pointer1, 50);

  puts("**********");
  cf_dumpMem();
  cf_free(pointer1);
  puts("**********");
  cf_dumpMem();
  cf_free(pointer2);
  puts("**********");
  cf_dumpMem();
  cf_free(pointer3);
  puts("**********");
  cf_dumpMem();
  cf_free(pointer4);
  puts("**********");
  cf_dumpMem();

  cf_checkAllMem();

  CF_EXIT_FUNC
}

void cf_MemoryTest_register(void) {
  cf_Test_add(cf_MemoryTest_testBase);
  cf_Test_add(cf_MemoryTest_testAll);
  cf_Test_add(cf_MemoryTest_testRealloc);
}

CF_END
