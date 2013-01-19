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

void cf_LogTest_test(void);
void cf_ArrayTest_test(void);
void cf_HashMapTest_test(void);
void cf_StrBufTest_test(void);
void cf_ConfigTest_test(void);
void cf_MemoryTest_test(void);
void cf_FileTest_test(void);

int main(void)
{
  cf_FileTest_test();
  return 0;
}

