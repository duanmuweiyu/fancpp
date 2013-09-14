/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-24  Jed Young  Creation
 */

#include "cfan/HashMap.h"
#include "cfan/Trace.h"

/*========================================================================
 * string to string
 */

/**
 * hash function
 */
size_t cf_HashMap_strHash(const char *str) {
  size_t hashValue;
  for (hashValue = 0; *str != '\0'; str++) {
    hashValue = *(str) + 31 * hashValue;
  }
  return hashValue;
}

#define cf_hashFunc(key) cf_HashMap_strHash(key)
#define cf_compFunc(v1, v2) strcmp((v1), (v2))

cf_HashMapTemplate_impl(cf_HashMapSS, const char*, char*)

cf_HashMapTemplate_impl(cf_HashMapSP, const char*, void*)

#undef cf_hashFunc
#undef cf_compFunc

/*========================================================================
 * int to int
 */

#define cf_hashFunc(key) key
#define cf_compFunc(v1, v2) ((v1)-(v2))

cf_HashMapTemplate_impl(cf_HashMapII, int, int)

cf_HashMapTemplate_impl(cf_HashMapLP, long, void*)

#undef cf_hashFunc
#undef cf_compFunc
