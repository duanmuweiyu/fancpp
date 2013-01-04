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

size_t cf_HashMap_strHash(char *str) {
  CF_ENTRY_FUNC
  size_t hashValue;
  for (hashValue = 0; *str != '\0'; str++) {
    hashValue = *(str) + 31 * hashValue;
  }
  CF_EXIT_FUNC
  return hashValue;
}

#define hashFunc(key) cf_HashMap_strHash(key)
#define compFunc(v1, v2) strcmp((v1), (v2))
#define Str char*

cf_HashMapTemplate_impl(Str, Str)

#undef Str
#undef hashFunc
#undef compFunc

/*************************************************************************
*
*/
#define Int int
#define hashFunc(key) key
#define compFunc(v1, v2) (v1)-(v2)

cf_HashMapTemplate_impl(Int, Int)

#undef Int
#undef hashFunc
#undef compFunc