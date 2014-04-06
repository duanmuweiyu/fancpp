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

void cf_StrBufTest_testPrintf(void) {
  cf_StrBuf strBuf;
  CF_ENTRY_FUNC

  cf_StrBuf_makeDefault(&strBuf);
  cf_StrBuf_printf(&strBuf, 128, "hello %s, %d", "world", 2);

  printf("%s, %d\n", cf_StrBuf_str(&strBuf), (int)cf_StrBuf_size(&strBuf));

  cf_verify(strcmp(cf_StrBuf_str(&strBuf), "hello world, 2") == 0);

  cf_StrBuf_dispose(&strBuf);
  CF_EXIT_FUNC
}

void cf_StrBufTest_testPrintfOverflow(void) {
  cf_StrBuf strBuf;
  CF_ENTRY_FUNC

  cf_StrBuf_makeDefault(&strBuf);
  cf_StrBuf_printf(&strBuf, 2, "0000000000000000000000000000000000000000000000000"
                   "hello %s, %d", "world", 2);

  printf("%s, %d\n", cf_StrBuf_str(&strBuf), (int)cf_StrBuf_size(&strBuf));

  cf_verify(strcmp(cf_StrBuf_str(&strBuf), "0000000000000000000000000000000000000000000000000"
                   "hello world, 2") == 0);

  cf_StrBuf_dispose(&strBuf);
  CF_EXIT_FUNC
}

void cf_StrBufTest_testAdd(void) {
  cf_StrBuf strBuf;
  const char *str1 = "123";
  const char *str2 = "abc";
  CF_ENTRY_FUNC

  cf_StrBuf_makeDefault(&strBuf);
  cf_StrBuf_add(&strBuf, str1, -1);
  cf_StrBuf_add(&strBuf, str2, -1);

  puts(cf_StrBuf_str(&strBuf));

  cf_verify(strcmp(cf_StrBuf_str(&strBuf), "123abc") == 0);

  cf_StrBuf_dispose(&strBuf);

  CF_EXIT_FUNC
}

void cf_StrBufTest_testAddOverflow(void) {
  cf_StrBuf *strBuf;
  const char *str1 = "123";
  const char *str2 = "abc";
  int i;
  CF_ENTRY_FUNC

  strBuf = cf_StrBuf_makeSize(2);
  cf_verify(strBuf);

  for (i=0; i<100; ++i) {
    cf_StrBuf_add(strBuf, str1, -1);
    cf_StrBuf_add(strBuf, str2, -1);
  }

  cf_check(strBuf);

  cf_verify(strBuf->size == 600);

  cf_Object_addRef(strBuf);
  cf_Object_release(&strBuf->super);
  cf_Object_release(&strBuf->super);

  CF_EXIT_FUNC
}

void cf_StrBufTest_register(void) {
  cf_Test_add(cf_StrBufTest_testPrintf);
  cf_Test_add(cf_StrBufTest_testPrintfOverflow);
  cf_Test_add(cf_StrBufTest_testAdd);
  cf_Test_add(cf_StrBufTest_testAddOverflow);
}

