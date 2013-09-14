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
void cf_QueueTest_register(void);
void cf_BlockingQueueTest_register(void);
void cf_ExecutorTest_register(void);
void cf_MemoryPoolTest_register(void);
void cf_LinkedListTest_register(void);
void cf_ActorTest_register(void);
void cf_CacheTest_register(void);
void cf_FileLogListenerTest_register(void);

/**
 * register all test case.
 *
 */
void registerAll(void) {
  cf_LogTest_register();
  cf_ArrayTest_register();
  cf_HashMapTest_register();
  cf_StrBufTest_register();
  cf_ConfigTest_register();
  cf_MemoryTest_register();
  cf_FileTest_register();
  cf_StrTest_register();
  cf_QueueTest_register();
  cf_BlockingQueueTest_register();
  cf_ExecutorTest_register();
  cf_MemoryPoolTest_register();
  cf_LinkedListTest_register();
  cf_ActorTest_register();
  cf_CacheTest_register();
  cf_FileLogListenerTest_register();
}

/**
 * execute the test.
 *
 */
int main(int argc, char **argv) {
  const char *name;
  name = argc >= 2 ? argv[1] : "";

  cf_Trace_traceOnExit();
  cf_Trace_init();

  //run test
  cf_Test_init();
  registerAll();

  cf_Test_run(name, 0);
  cf_Test_dispose();

  cf_Trace_printTrace();
  cf_Trace_printPerformance();
  cf_Trace_dispose();
  cf_dumpMem();

  return 0;
}

