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

  while ((next = cf_StrToken_next(&tokener)) != NULL) {
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

  cf_verify(cf_Str_endsWith("hello world", ""));
  cf_verify(cf_Str_endsWith("hello world", "world"));
}

void cf_StrTest_testFindIndex(void) {
  cf_verify(cf_Str_index("hello world", "a") == -1);
  cf_verify(cf_Str_index("hello world", "h") == 0);
  cf_verify(cf_Str_index("hello world", "e") == 1);
  cf_verify(cf_Str_index("hello world", "d") == strlen("hello world")-1);

  cf_verify(cf_Str_lastIndex("hello world", "o") == 7);
  cf_verify(cf_Str_lastIndex("hello world", "l") == strlen("hello world")-2);
  cf_verify(cf_Str_lastIndex("hello world", "a") == -1);
}

void cf_StrTest_testReplace(void) {
  char str1[] = "hello world";
  char str2[] = "hello world";
  cf_Str_replaceChar(str1, 'l', '@');
  cf_verify(cf_Str_equals(str1, "he@@o wor@d"));
  cf_Str_replaceChar(str2, 'd', '@');
  cf_verify(cf_Str_equals(str2, "hello worl@"));
}

void cf_StrTest_testCopy(void) {
  char str1[] = "hello world";
  char *str2;
  str2 = cf_Str_dup(str1);
  cf_verify(cf_Str_equals(str1, str2));
  cf_free(str2);
}

void cf_StrTest_register(void) {
  cf_Test_add(cf_StrTest_testToken);
  cf_Test_add(cf_StrTest_testTrim);
  cf_Test_add(cf_StrTest_testStartsWith);
  cf_Test_add(cf_StrTest_testFindIndex);
  cf_Test_add(cf_StrTest_testReplace);
  cf_Test_add(cf_StrTest_testCopy);
}

