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
  cf_StrStrHashMap map;
  char *key;
  char *value;
  char *key2;
  char *value2;
  char *str = "123";
  CF_ENTRY_FUNC

  cf_StrStrHashMap_make(&map, 2);

  key = "123";
  value = "1234";

  cf_StrStrHashMap_set(&map, str, value, &key2, &value2);

  cf_StrStrHashMap_get(&map, key, &key2, &value2 );
  printf("%s:%s\n", key2, value2);

  CF_EXIT_FUNC
  return 0;
}

