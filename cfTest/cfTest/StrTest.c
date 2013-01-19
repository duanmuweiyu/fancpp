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

void cf_StrTest_testToken(void) {
  cf_StrToken tokener;
  char buf[] = "hello world";
  const char *delim = "or";
  char *next;
  CF_ENTRY_FUNC

  cf_StrToken_make(&tokener, buf, delim);

  while (next = cf_StrToken_next(&tokener)) {
    printf("%s\n", next);
  }

  CF_EXIT_FUNC
}

void cf_StrTest_testTrim(void) {
  char buf[] = "  hello world  ";
  const char *expected = "hello world";
  CF_ENTRY_FUNC

  cf_Str_trim(buf);
  puts(buf);
  cf_verify(cf_Str_equals(buf, expected));

  CF_EXIT_FUNC
}

void cf_StrTest_testStartsWith(void) {
  cf_verify(cf_Str_startsWith("hello world", ""));
  cf_verify(cf_Str_startsWith("hello world", "hello"));
}

void cf_StrTest_register(void) {
  cf_Test_add(cf_StrTest_testToken);
  cf_Test_add(cf_StrTest_testTrim);
  cf_Test_add(cf_StrTest_testStartsWith);
}

