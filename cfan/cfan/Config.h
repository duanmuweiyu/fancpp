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

#include "cfan/HashMap.h"

#include <stdio.h>
#include <string.h>

CF_BEGIN

/**
 * Tool for read config file.
 * the file format like Java's Properties file.
 * for example:
 *   key = value
 *
 */
typedef struct cf_Config_ {
  cf_HashMapSS map;
} cf_Config;

/**
 * load from file
 *
 */
cf_Error cf_Config_make(cf_Config *self, const char *path);

/**
 * get value by key
 *
 */
static inline const char *cf_Config_get(cf_Config *self, const char *key, const char *defVal) {
  cf_Error err;
  char *value;
  err = cf_HashMapSS_get(&self->map, key, NULL, &value );
  if (err != cf_Error_ok) {
    return defVal;
  }
  return value;
}

CF_END

#endif
