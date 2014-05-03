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


void cf_ConfigTest_test(void) {
  cf_Config map;
  cf_HashMapSSIterator iter;

  const char *key2;
  char *value2;
  cf_Error err;

  CF_ENTRY_FUNC

  cf_Config_make(&map, "F:/temp/testProps.props");

  cf_HashMapSS_createIterator(&map.map, &iter);
  while (!(err=cf_HashMapSSIterator_next(&iter))) {
    cf_HashMapSSIterator_get(&iter, &key2, &value2);
    printf("%s:%s\n", key2, value2);
  }

  cf_Config_dispose(&map);

  CF_EXIT_FUNC
}

void cf_ConfigTest_register(void) {
  cf_Test_add(cf_ConfigTest_test);
}
