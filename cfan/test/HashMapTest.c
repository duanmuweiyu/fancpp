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


int cf_HashMapTest_test(void)
{
  cf_HashMap map;
  cf_Value key;
  cf_Value value;
  cf_Value key2;
  cf_Value value2;
  char *str = "123";
  CF_ENTRY_FUNC

  cf_HashMap_makeDict(&map, 2);

  key.pointer = (void*)"123";
  value.pointer = (void*)"1234";

  cf_HashMap_set(&map, cf_toValue(str), value, &key2, &value2);

  cf_HashMap_get(&map, key, &key2, &value2 );
  printf("%s:%s\n", key2.pointer, value2.pointer);

  CF_EXIT_FUNC
  return 0;
}

