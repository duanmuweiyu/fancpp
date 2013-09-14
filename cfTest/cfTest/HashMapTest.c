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

void cf_HashMapTest_testGet(void)
{
  cf_HashMapSS map;

  char *removedValue;
  char *value;
  char key[] = "yjd";

  CF_ENTRY_FUNC

  cf_HashMapSS_make(&map, 2);

  cf_HashMapSS_set(&map, "123", "1234", NULL, NULL);
  cf_HashMapSS_set(&map, "abc", "abcd", NULL, NULL);
  cf_HashMapSS_set(&map, "abc", "123", NULL, &removedValue);
  cf_HashMapSS_set(&map, "yjd", "26", NULL, NULL);
  cf_HashMapSS_set(&map, "qq", "49", NULL, NULL);

  printf("removed:%s\n", removedValue);

  cf_HashMapSS_get(&map, key, NULL, &value );
  printf("%s:%s\n", key, value);

  cf_HashMapSS_dispose(&map);
  CF_EXIT_FUNC
}

void cf_HashMapTest_testRemove(void)
{
  cf_HashMapSS map;

  const char *key1 = "key1";
  char *value1 = NULL;
  const char *key2 = "key2";
  char *value2 = NULL;
  const char *key3 = "key3";
  char *value3 = NULL;
  char *val = NULL;

  CF_ENTRY_FUNC

  cf_HashMapSS_make(&map, 2);

  cf_HashMapSS_set(&map, key1, value1, NULL, NULL);
  cf_HashMapSS_set(&map, key2, value2, NULL, NULL);

  cf_HashMapSS_remove(&map, key1, NULL, NULL);
  cf_HashMapSS_set(&map, key3, value3, NULL, NULL);

  cf_HashMapSS_get(&map, key3, NULL, &val);
  cf_verify(val == value3);

  cf_HashMapSS_dispose(&map);
  CF_EXIT_FUNC
}

void cf_HashMapTest_testIter(void)
{
  cf_HashMapSS map;
  cf_HashMapSSIterator iter;

  const char *key2;
  char *value2;
  cf_Error err;

  CF_ENTRY_FUNC

  cf_HashMapSS_make(&map, 2);

  cf_HashMapSS_set(&map, "123", "1234", NULL, NULL);
  cf_HashMapSS_set(&map, "abc", "abcd", NULL, NULL);
  cf_HashMapSS_set(&map, "abc", "123", NULL, NULL);
  cf_HashMapSS_set(&map, "yjd", "26", NULL, NULL);
  cf_HashMapSS_set(&map, "qq", "49", NULL, NULL);

  cf_HashMapSS_createIterator(&map, &iter);
  while (!(err=cf_HashMapSSIterator_next(&iter))) {
    cf_HashMapSSIterator_get(&iter, &key2, &value2);
    printf("%s:%s\n", key2, value2);
  }

  cf_HashMapSS_dispose(&map);
  CF_EXIT_FUNC
}

void cf_HashMapTest_register(void)
{
  cf_Test_add(cf_HashMapTest_testGet);
  cf_Test_add(cf_HashMapTest_testIter);
}
