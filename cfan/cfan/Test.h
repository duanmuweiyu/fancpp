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

typedef struct cf_Test_ {
  void (*func)();
  int attr;
  const char *name;
  const char *file;
  unsigned int line;
} cf_Test;

extern cf_Array testList;
extern long cf_Test_errorCount;

void cf_Test_init(void);

inline void cf_Test_doAdd(void (*func)(), int attr, const char *name
                        , const char *file, const unsigned int line) {
  cf_Test test = { func, attr, name, file, line };
  cf_Array_add(&testList, &test);
}

#define cf_Test_add(name)\
  cf_Test_doAdd(name, 0, #name , __FILE__ , __LINE__ )

#define cf_Test_addAttr(name, attr)\
  cf_Test_doAdd(name, attr, #name , __FILE__ , __LINE__ )

void cf_Test_run(const char *nameFilter, const int attrFilter);

#define cf_verify(exp) do {\
    if (!(exp)) {\
      cf_Log_log("test", cf_LogLevel_err, "test fail: %s", #exp);\
      ++cf_Test_errorCount;\
    }\
  } while(0);

#endif
