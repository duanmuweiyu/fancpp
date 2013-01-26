/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-19  Jed Young  Creation
 */

#include "cfan/Test.h"
#include "cfan/Str.h"

cf_ArrayTemplate_impl(cf_ArrayTest, cf_Test)

cf_ArrayTest testList;
long cf_Test_errorCount = 0;

void cf_Test_init(void) {
  cf_ArrayTest_make(&testList, 0, 100);
}

void cf_Test_run(const char *nameFilter, const int attrFilter) {
  long n;
  long i;
  long count = 0;
  long snapshot;
  long errCount = 0;
  cf_Test *test;

  cf_Log_log("test", cf_LogLevel_info, "start run test");

  n = cf_ArrayTest_size(&testList);
  for (i=0; i<n; ++i) {
    test = cf_ArrayTest_get(&testList, i);
    if (cf_Str_startsWith(test->name, nameFilter) && test->attr == attrFilter) {
      cf_Log_log("test", cf_LogLevel_info, "run: %s", test->name);
      snapshot = cf_Test_errorCount;
      (test->func)();
      if (cf_Test_errorCount != snapshot) {
        ++errCount;
      }
      ++count;
      cf_Log_log("test", cf_LogLevel_info, "end: %s", test->name);
    }
  }

  cf_Log_log("test", cf_LogLevel_info
             , "test end: fail %d, total %d, runed %d", errCount, n, count);
  if (errCount > 0) {
    cf_Log_log("test", cf_LogLevel_info, "test fail");
  } else {
    cf_Log_log("test", cf_LogLevel_info, "test success");
  }
}