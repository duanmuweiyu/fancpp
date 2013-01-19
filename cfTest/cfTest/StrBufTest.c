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

  cf_StrBuf_make(&strBuf);
  cf_StrBuf_printf(&strBuf, 128, "hello %s, %d", "world", 2);

  printf("%s, %d\n", cf_StrBuf_str(&strBuf), cf_StrBuf_size(&strBuf));

  cf_verify(strcmp(cf_StrBuf_str(&strBuf), "hello world, 2") == 0);

  cf_StrBuf_dispose(&strBuf);
}

void cf_StrBufTest_testAdd(void) {
  cf_StrBuf strBuf;
  const char *str1 = "123";
  const char *str2 = "abc";
  CF_ENTRY_FUNC

  cf_StrBuf_make(&strBuf);
  cf_StrBuf_add(&strBuf, str1, -1);
  cf_StrBuf_add(&strBuf, str2, -1);

  puts(cf_StrBuf_str(&strBuf));

  cf_verify(strcmp(cf_StrBuf_str(&strBuf), "123abc") == 0);

  cf_StrBuf_dispose(&strBuf);

  CF_EXIT_FUNC
}

void cf_StrBufTest_register(void) {
  cf_Test_add(cf_StrBufTest_testPrintf);
  cf_Test_add(cf_StrBufTest_testAdd);
}

