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

CF_BEGIN

void cf_ArrayListTest_testAdd(void) {
  cf_ArrayList array;
  int a = 1;
  int b = 2;
  int c = 3;
  size_t size;
  unsigned int i = 0;
  int *value;
  CF_ENTRY_FUNC

  cf_ArrayList_make(&array, 0, 2, sizeof(int));
  cf_ArrayList_add(&array, &a);
  cf_ArrayList_add(&array, &b);
  cf_ArrayList_add(&array, &c);
  size = cf_ArrayList_size(&array);

  for (i=0; i<size; ++i) {
    value = (int*)cf_ArrayList_get(&array, i);
    printf("%d\n", *value);
  }

  cf_ArrayList_dispose(&array);

  CF_EXIT_FUNC
}

void cf_ArrayListTest_register(void) {
  cf_Test_add(cf_ArrayListTest_testAdd);
}

CF_END
