/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-24  Jed Young  Creation
 */

#ifndef _CF_HASHMAP_H_
#define _CF_HASHMAP_H_

#include "Error.h"
#include "Memory.h"
#include "miss.h"

#include <string.h>

CF_BEGIN

/**
 * Element
 *
 */
typedef struct cf_HashMapElem_ {
  bool used; //flag for null
  cf_Value key;
  cf_Value value;
  struct cf_HashMapElem_ *next;
} cf_HashMapElem;

/**
 * Map
 *
 */
typedef struct cf_HashMap_ {
  size_t    size;
  cf_HashMapElem *table;
  size_t (*hashFunc)(cf_Value key);
  int (*compFunc)(cf_Value v1, cf_Value v2);
} cf_HashMap;

/**
 * constructor
 *
 */
cf_Error cf_HashMap_make(cf_HashMap *self, size_t size
  , size_t (*hashFunc)(cf_Value key)
  , int (*compFunc)(cf_Value v1, cf_Value v2));

/**
 * lookup
 *
 */
cf_Error cf_HashMap_get(cf_HashMap *self, cf_Value key, cf_Value *oldKey, cf_Value *oldValue);

/**
 * put
 *
 */
cf_Error cf_HashMap_set(cf_HashMap *self, cf_Value key, cf_Value value, cf_Value *oldKey, cf_Value *oldValue);

/**
 * destroy content
 *
 */
void cf_HashMap_dispose(cf_HashMap *self);

/*************************************************************************
 * dictionary is a map that key is char string
 */

size_t cf_HashMap_strHash(cf_Value key);

int cf_HashMap_strComp(cf_Value v1, cf_Value v2);

inline cf_Error cf_HashMap_makeDict(cf_HashMap *self, size_t size) {
  return cf_HashMap_make(self, size, cf_HashMap_strHash, cf_HashMap_strComp);
}

CF_END

#endif

