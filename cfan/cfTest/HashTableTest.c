
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

static int mystrcmp(const void *k1, const void *k2, int n) {
  CF_UNUSED(n);
  return strcmp(k1, k2);
}

void cf_HashTable_testGet(void) {
  cf_HashTable map;

  char removedValue[256] = {0};
  char value[256] = {0};
  char key[] = "yjd";
  int maxLink;

  CF_ENTRY_FUNC

  cf_HashTable_make(&map, 2, 256, 256);
  map.compFunc = mystrcmp;
  map.hashFunc = cf_HashTable_strHash;

  cf_HashTable_set(&map, "123", "1234", NULL, NULL);
  cf_HashTable_set(&map, "abc", "abcd", NULL, NULL);
  cf_HashTable_set(&map, "abc", "123", NULL, &removedValue);
  cf_HashTable_set(&map, "yjd", "26", NULL, NULL);
  cf_HashTable_set(&map, "qq", "49", NULL, NULL);

  //printf("removed:%s\n", removedValue);
  cf_verify(strcmp(removedValue, "abcd") == 0);

  cf_HashTable_get(&map, key, NULL, value );
  //printf("%s:%s\n", key, value);
  cf_verify(strcmp(value, "26") == 0);

  //print each item as string
  {
    cf_HashTableIterator iter;
    char key2[256]={0};
    char value2[256]={0};
    cf_HashTable_createIterator(&map, &iter);
    while (!(cf_HashTableIterator_next(&iter))) {
      cf_HashTableIterator_get(&iter, key2, value2);
      printf("%s:%s\n", key2, value2);
    }
  }

  maxLink = cf_HashTable_count(&map);
  cf_verify(maxLink == 3);

  cf_HashTable_dispose(&map);
  CF_EXIT_FUNC
}

void cf_HashTable_testRemove(void) {
  cf_HashTable map;

  const char *key1 = "key1";
  char *value1 = "v1";
  const char *key2 = "key2";
  char *value2 = "v2";
  const char *key3 = "key3";
  char *value3 = "v3";
  char val[256] = {0};

  CF_ENTRY_FUNC

  cf_HashTable_make(&map, 2, 256, 256);

  cf_HashTable_set(&map, key1, value1, NULL, NULL);
  cf_HashTable_set(&map, key2, value2, NULL, NULL);

  cf_HashTable_remove(&map, key1, NULL, NULL);
  cf_HashTable_set(&map, key3, value3, NULL, NULL);

  cf_HashTable_get(&map, key3, NULL, val);
  cf_verify(strcmp(val, value3) == 0);

  cf_HashTable_dispose(&map);
  CF_EXIT_FUNC
}

void cf_HashTable_testIter(void) {
  cf_HashTable map;
  cf_HashTableIterator iter;

  char key2[256]={0};
  char value2[256]={0};
  int count = 0;
  cf_Error err;

  CF_ENTRY_FUNC

  cf_HashTable_make(&map, 2, 256, 256);

  cf_HashTable_set(&map, "123", "1234", NULL, NULL);
  cf_HashTable_set(&map, "abc", "abcd", NULL, NULL);
  cf_HashTable_set(&map, "abc", "123", NULL, NULL);
  cf_HashTable_set(&map, "yjd", "26", NULL, NULL);
  cf_HashTable_set(&map, "qq", "49", NULL, NULL);

  cf_HashTable_createIterator(&map, &iter);
  while (!(err=cf_HashTableIterator_next(&iter))) {
    cf_HashTableIterator_get(&iter, key2, value2);
    printf("%s:%s\n", key2, value2);
    ++count;
  }
  cf_verify(count == 4);
  cf_HashTable_dispose(&map);
  CF_EXIT_FUNC
}

void cf_HashTable_register(void) {
  cf_Test_add(cf_HashTable_testGet);
  cf_Test_add(cf_HashTable_testIter);
  cf_Test_add(cf_HashTable_testRemove);
}

CF_END