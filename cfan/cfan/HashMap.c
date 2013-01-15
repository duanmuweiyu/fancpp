/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-24  Jed Young  Creation
 */

#include "HashMap.h"


/*************************************************************************
*
*/

size_t cf_HashMap_strHash(const char *str) {
  size_t hashValue;
  CF_ENTRY_FUNC
  for (hashValue = 0; *str != '\0'; str++) {
    hashValue = *(str) + 31 * hashValue;
  }
  CF_EXIT_FUNC
  return hashValue;
}

#define hashFunc(key) cf_HashMap_strHash(key)
#define compFunc(v1, v2) strcmp((v1), (v2))

cf_HashMapTemplate_impl(cf_HashMapSS, const char*, char*)

#undef hashFunc
#undef compFunc

/*************************************************************************
*
*/

#define hashFunc(key) key
#define compFunc(v1, v2) ((v1)-(v2))

cf_HashMapTemplate_impl(cf_HashMapII, int, int)

#undef hashFunc
#undef compFunc
