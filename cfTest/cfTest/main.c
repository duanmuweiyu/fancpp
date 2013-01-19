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

void cf_LogTest_register(void);
void cf_ArrayTest_register(void);
void cf_HashMapTest_register(void);
void cf_StrBufTest_register(void);
void cf_ConfigTest_register(void);
void cf_MemoryTest_register(void);
void cf_FileTest_register(void);
void cf_StrTest_register(void);

void registerAll(void) {
  cf_LogTest_register();
  cf_ArrayTest_register();
  cf_HashMapTest_register();
  cf_StrBufTest_register();
  cf_ConfigTest_register();
  cf_MemoryTest_register();
  cf_FileTest_register();
  cf_StrTest_register();
}

int main(int argc, char **argv)
{
  const char *name;
  cf_Test_init();
  registerAll();

  name = argc >= 2 ? argv[1] : "";
  cf_Test_run(name, 0);
  return 0;
}

