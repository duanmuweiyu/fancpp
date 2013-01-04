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

/*
* macro:
* hashFunc
* compFunc
*/

/*************************************************************************
* Template define
*/

#define cf_HashMapTemplate(K, V) \
\
/**\
 * Element\
 *
 */\
typedef struct cf_##K##V##HashMapElem_ {\
  bool used; /*flag for null*/\
  K key;\
  V value;\
  struct cf_##K##V##HashMapElem_ *next;\
} cf_##K##V##HashMapElem;\
\
/**\
 * Map\
 *\
 */\
typedef struct cf_##K##V##HashMap_ {\
  size_t    size;\
  cf_##K##V##HashMapElem *table;\
} cf_##K##V##HashMap;\
\
/**\
 * constructor\
 *\
 */\
cf_Error cf_##K##V##HashMap_make(cf_##K##V##HashMap *self, size_t size);\
\
/**\
 * lookup\
 *\
 */\
cf_Error cf_##K##V##HashMap_get(cf_##K##V##HashMap *self, K key, K *oldKey, V *oldValue);\
\
/**\
 * put\
 *\
 */\
cf_Error cf_##K##V##HashMap_set(cf_##K##V##HashMap *self, K key, V value, K *oldKey, V *oldValue);\
\
/**\
 * destroy content\
 *\
 */\
void cf_##K##V##HashMap_dispose(cf_##K##V##HashMap *self);\



/*************************************************************************
* Impl
*/

#define cf_HashMapTemplate_impl(K, V) \
\
cf_Error cf_##K##V##HashMap_make(cf_##K##V##HashMap *self, size_t size) {\
\
  CF_ENTRY_FUNC\
  cf_assert(self);\
\
  self->size = size;\
  self->table = (cf_##K##V##HashMapElem*)cf_calloc(size, sizeof(cf_##K##V##HashMapElem));\
  if (NULL == self->table) {\
    CF_EXIT_FUNC return cf_Error_alloc;\
  }\
  CF_EXIT_FUNC\
  return cf_Error_ok;\
}\
\
cf_Error cf_##K##V##HashMap_get(cf_##K##V##HashMap *self, K key, K *oldKey, V *oldValue) {\
  cf_##K##V##HashMapElem *elem;\
\
  CF_ENTRY_FUNC\
  cf_assert(self);\
  cf_assert(oldKey);\
  cf_assert(oldValue);\
\
  for (elem = self->table + (hashFunc(key) % self->size); elem != NULL && elem->used; elem = elem->next) {\
    if (compFunc(key, elem->key) == 0) {\
      *oldKey = elem->key;\
      *oldValue = elem->value;\
      CF_EXIT_FUNC\
      return cf_Error_ok;\
    }\
  }\
  CF_EXIT_FUNC\
  return cf_Error_notfound;\
}\
\
\
cf_Error cf_##K##V##HashMap_set(cf_##K##V##HashMap *self, K key, V value, K *oldKey, V *oldValue) {\
  cf_##K##V##HashMapElem *elem;\
  cf_##K##V##HashMapElem *newElem;\
  size_t hashValue;\
\
  CF_ENTRY_FUNC\
  cf_assert(self);\
  cf_assert(oldKey);\
  cf_assert(oldValue);\
\
  hashValue = hashFunc(key) % self->size;\
  for (elem = self->table + hashValue; elem != NULL && elem->used; elem = elem->next) {\
    /* if found*/\
    if (compFunc(key, elem->key) == 0) {\
      *oldKey = elem->key;\
      *oldValue = elem->value;\
      elem->key = key;\
      elem->value = value;\
      CF_EXIT_FUNC\
      return cf_Error_ok;\
    }\
  }\
\
  /*now elem is last*/\
\
  /* if not found*/\
  if (elem->used) {\
    newElem = (cf_##K##V##HashMapElem *)cf_malloc(sizeof(cf_##K##V##HashMapElem));\
    if (!newElem) {\
      CF_EXIT_FUNC return cf_Error_alloc;\
    }\
    elem->next = newElem;\
  } else {\
    /*will use this*/\
    newElem = elem;\
  }\
\
  newElem->used = true;\
  newElem->next = NULL;\
  newElem->key = key;\
  newElem->value = value;\
\
  CF_EXIT_FUNC\
  return cf_Error_ok;\
}\
\
void cf_##K##V##HashMap_dispose(cf_##K##V##HashMap *self) {\
  CF_ENTRY_FUNC\
  cf_assert(self);\
  cf_free(self->table);\
  CF_EXIT_FUNC\
}\


/*************************************************************************
* Default define
*/

#define Str char*
cf_HashMapTemplate(Str, Str)
#undef Str

#define Int int
cf_HashMapTemplate(Int, Int)
#undef Int

CF_END

#endif