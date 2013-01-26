/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-19  Jed Young  Creation
 */

#ifndef _CF_TEST_H_
#define _CF_TEST_H_

#include "cfan/Array.h"

CF_BEGIN

/**
 * Test case
 */
typedef struct cf_Test_ {
  void (*func)();
  int attr;
  const char *name;
  const char *file;
  unsigned int line;
} cf_Test;

cf_ArrayTemplate(cf_ArrayTest, cf_Test)

extern cf_ArrayTest testList;
extern long cf_Test_errorCount;

/**
 * init test
 *
 */
void cf_Test_init(void);

/**
 * add a test case
 *
 */
inline void cf_Test_doAdd(void (*func)(), int attr, const char *name
                        , const char *file, const unsigned int line) {
  cf_Test test = { func, attr, name, file, line };
  cf_ArrayTest_add(&testList, &test);
}

/**
 * convenience add test case.
 *
 */
#define cf_Test_add(name)\
  cf_Test_doAdd(name, 0, #name , __FILE__ , __LINE__ )

/**
 * convenience add test case with a attr value.
 *
 */
#define cf_Test_addAttr(name, attr)\
  cf_Test_doAdd(name, attr, #name , __FILE__ , __LINE__ )

/**
 * run test case which starts with nameFilter and equals attrFilter
 *
 */
void cf_Test_run(const char *nameFilter, const int attrFilter);

/**
 * verify the conditon.
 * It's like assert but special for test.
 *
 */
#define cf_verify(exp) do {\
    if (!(exp)) {\
      cf_Log_log("test", cf_LogLevel_err, "test fail: %s", #exp);\
      ++cf_Test_errorCount;\
    }\
  } while(0);

CF_END

#endif