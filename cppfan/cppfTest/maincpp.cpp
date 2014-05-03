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
//#include "cppfTest/MemoryTest.h"

#include "cppfan/Test.h"

CF_USING_NAMESPACE

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
  Test::getInstance().run(name);

  cf_Trace_printTrace();
  cf_Trace_printPerformance();
  cf_Trace_dispose();
  cf_dumpMem();

  puts("END");
  return 0;
}

