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
#include "cfan.h"

int cf_StrBufTest_test(void)
{
  cf_StrBuf strBuf;
  const char *str1 = "123";
  const char *str2 = "abc";
  CF_ENTRY_FUNC

  cf_StrBuf_make(&strBuf);
  cf_StrBuf_add(&strBuf, str1, -1);
  cf_StrBuf_add(&strBuf, str2, -1);

  puts(cf_StrBuf_str(&strBuf));

  cf_StrBuf_dispose(&strBuf);

  CF_EXIT_FUNC
  return 0;
}

