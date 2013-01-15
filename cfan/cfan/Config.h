/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-6  Jed Young  Creation
 */

#ifndef _CF_CONFIG_H_
#define _CF_CONFIG_H_

#include "HashMap.h"

#include <stdio.h>
#include <string.h>

CF_BEGIN

typedef struct cf_Config_ {
  cf_HashMapSS map;
} cf_Config;

cf_Error cf_Config_make(cf_Config *self, const char *path);

inline void cf_Config_get(cf_Config *self, const char *key, char *value, const char *defVal) {
  //cf_HashMapSS_get(&map, key, NULL, &value );
}

CF_END

#endif