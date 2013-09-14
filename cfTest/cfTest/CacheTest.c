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

static void onRemove(const char *key, void *val){
  int32_t *i = (int32_t *)val;
  printf("free %d\n", *i);
  cf_free(val);
}

void cf_CacheTest_test(void)
{
  cf_Cache cache;

  const char *key1 = "key1";
  char *value1 = NULL;
  const char *key2 = "key2";
  char *value2 = NULL;
  const char *key3 = "key3";
  char *value3 = NULL;
  char *val = NULL;
  cf_Error err;

  CF_ENTRY_FUNC

  cf_Cache_make(&cache, 2, onRemove);

  value1 = (char*)cf_malloc(4);
  *((int32_t*)value1) = 0;
  value2 = (char*)cf_malloc(4);
  *((int32_t*)value2) = 1;
  value3 = (char*)cf_malloc(4);
  *((int32_t*)value3) = 2;

  cf_Cache_set(&cache, key1, value1);
  cf_Cache_set(&cache, key2, value2);
  cf_Cache_set(&cache, key3, value3);

  printf("------\n");

  cf_verify(cf_Cahce_size(&cache) == 2);

  cf_Cache_get(&cache, key2, NULL, (void**)&val);
  cf_verify(val == value2);

  val = NULL;
  cf_Cache_get(&cache, key3, NULL, (void**)&val);
  cf_verify(val == value3);

  err = cf_Cache_get(&cache, key1, NULL, (void**)&val);
  cf_verify(err);

  cf_Cache_dispose(&cache);

  CF_EXIT_FUNC
}

void cf_CacheTest_register(void) {
  cf_Test_add(cf_CacheTest_test);
}
