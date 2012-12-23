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

int cf_ArrayTest_test(void)
{
  cf_Array array;
  int a = 1;
  int b = 2;
  int c = 3;
  size_t size;
  unsigned int i = 0;
  int *value;
  CF_ENTRY_FUNC

  cf_Array_make(&array, 0, 2, sizeof(int));
  cf_Array_add(&array, &a);
  cf_Array_add(&array, &b);
  cf_Array_add(&array, &c);
  size = cf_Array_size(&array);

  for (i=0; i<size; ++i) {
    cf_Array_get(&array, i, (void**)&value);
    printf("%d\n", *value);
  }

  cf_dumpMem();

  CF_EXIT_FUNC
  return 0;
}

