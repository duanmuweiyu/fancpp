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

void cf_ArrayTest_testAdd(void) {
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
    value = (int*)cf_Array_get(&array, i);
    printf("%d\n", *value);
  }

  cf_Array_dispose(&array);

  CF_EXIT_FUNC
}

void cf_ArrayTest_testSort(void) {
  cf_Array array;
  int a = 2;
  int b = 3;
  int c = 1;
  size_t size;
  unsigned int i = 0;
  int *value = NULL;
  long index;
  CF_ENTRY_FUNC

  cf_Array_make(&array, 0, 2, sizeof(int));
  cf_Array_add(&array, &a);
  cf_Array_add(&array, &b);
  cf_Array_add(&array, &c);
  size = cf_Array_size(&array);

  cf_ArrayI_qsort(&array);

  for (i=0; i<size; ++i) {
    value = (int*)cf_Array_get(&array, i);
    printf("%d\n", *value);
  }

  index = cf_ArrayI_bsearch(&array, value);
  cf_verify(index == 2);

  cf_Array_dispose(&array);

  CF_EXIT_FUNC
}

void cf_ArrayTest_register(void) {
  cf_Test_add(cf_ArrayTest_testAdd);
  cf_Test_add(cf_ArrayTest_testSort);
}

