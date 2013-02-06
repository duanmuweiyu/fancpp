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

#include "cfan/Error.h"
#include "cfan/Memory.h"
#include "cfan/miss.h"

#include <string.h>
#include <stdio.h>

CF_BEGIN

/*
* Hash map is a container that contains key-value pairs with unique keys.
* And supports fast search by key.
* This is a hash Map template macro.
* depends macro:
*   hashFunc
*   compFunc
*/

/*************************************************************************
* Template head file define
*/
#define cf_HashMapTemplate(HashMap, K, V) \
\
/**\
 * Element\
 *
 */\
typedef struct HashMap##Elem_ {\
  bool used; /*flag for null*/\
  K key;\
  V value;\
  struct HashMap##Elem_ *next;\
} HashMap##Elem;\
\
/**\
 * Map\
 *\
 */\
typedef struct HashMap##_ {\
  size_t    size;\
  HashMap##Elem *table;\
  HashMap##Elem *idle;/*deleted elements*/\
} HashMap;\
/**\
 * Iterator\
 *\
 */\
 typedef struct HashMap##Iterator_ {\
   HashMap *parent;\
   size_t position;\
   HashMap##Elem *elem;\
 } HashMap##Iterator;\
\
/**\
 * constructor\
 *\
 */\
cf_Error HashMap##_make(HashMap *self, size_t size);\
\
/**\
 * lookup\
 *\
 */\
cf_Error HashMap##_get(HashMap *self, K key, K *oldKey, V *oldValue);\
\
/**\
 * put\
 *\
 */\
cf_Error HashMap##_set(HashMap *self, K key, V value, K *oldKey, V *oldValue);\
/**\
 * remove\
 *\
 */\
cf_Error HashMap##_remove(HashMap *self, K key, K *oldKey, V *oldValue);\
\
/**\
 * destroy content\
 *\
 */\
void HashMap##_dispose(HashMap *self);\
\
\
cf_Error HashMap##Iterator_next(HashMap##Iterator *self);\
\
cf_Error HashMap##Iterator_get(HashMap##Iterator *self, K *key, V *value);\
\
static inline void HashMap##_createIterator(HashMap *self, HashMap##Iterator *iter) {\
  cf_assert(self);\
  cf_assert(iter);\
  iter->parent = self;\
  iter->elem = NULL;\
  iter->position = 0;\
}\


/*************************************************************************
* Implemention macro
*/
#define cf_HashMapTemplate_impl(HashMap, K, V) \
\
cf_Error HashMap##_make(HashMap *self, size_t size) {\
\
  CF_ENTRY_FUNC\
  cf_assert(self);\
\
  self->size = size;\
  self->idle = NULL;\
  self->table = (HashMap##Elem*)cf_calloc(size, sizeof(HashMap##Elem));\
  if (NULL == self->table) {\
    CF_EXIT_FUNC return cf_Error_alloc;\
  }\
  CF_EXIT_FUNC\
  return cf_Error_ok;\
}\
\
cf_Error HashMap##_get(HashMap *self, K key, K *oldKey, V *oldValue) {\
  HashMap##Elem *elem;\
\
  CF_ENTRY_FUNC\
  cf_assert(self);\
  cf_assert(oldValue);\
\
  for (elem = self->table + (cf_hashFunc(key) % self->size); elem != NULL && elem->used; elem = elem->next) {\
    if (cf_compFunc(key, elem->key) == 0) {\
      if (oldKey) *oldKey = elem->key;\
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
cf_Error HashMap##_set(HashMap *self, K key, V value, K *oldKey, V *oldValue) {\
  HashMap##Elem *elem;\
  HashMap##Elem *newElem;\
  size_t hashValue;\
\
  CF_ENTRY_FUNC\
  cf_assert(self);\
\
  hashValue = cf_hashFunc(key) % self->size;\
  \
  elem = self->table + hashValue;\
  while ( elem->used ) {\
    /* if found*/\
    if (cf_compFunc(key, elem->key) == 0) {\
      if (oldKey) *oldKey = elem->key;\
      if (oldValue) *oldValue = elem->value;\
      elem->key = key;\
      elem->value = value;\
      CF_EXIT_FUNC\
      return cf_Error_ok;\
    }\
    if (elem->next == NULL) break;\
    elem = elem->next;\
  }\
\
  /*now elem is last*/\
\
  /* if not found*/\
  if (elem->used) {\
    if (self->idle != NULL) {\
      newElem = self->idle;\
      self->idle = newElem->next;\
    } else {\
      newElem = (HashMap##Elem *)cf_malloc(sizeof(HashMap##Elem));\
      if (!newElem) {\
        CF_EXIT_FUNC return cf_Error_alloc;\
      }\
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
cf_Error HashMap##_remove(HashMap *self, K key, K *oldKey, V *oldValue) {\
  HashMap##Elem *elem;\
  HashMap##Elem *temp = NULL;\
\
  CF_ENTRY_FUNC\
  cf_assert(self);\
\
  for (elem = self->table + (cf_hashFunc(key) % self->size); elem != NULL && elem->used; elem = elem->next) {\
    if (cf_compFunc(key, elem->key) == 0) {\
      if (oldKey) *oldKey = elem->key;\
      if (oldValue) *oldValue = elem->value;\
      if (temp != NULL) {\
        temp->next = NULL;\
        elem->next = self->idle;\
        self->idle = elem;\
      }\
      elem->used = false;\
      CF_EXIT_FUNC\
      return cf_Error_ok;\
    }\
    temp = elem;\
  }\
  CF_EXIT_FUNC\
  return cf_Error_notfound;\
}\
\
void HashMap##_dispose(HashMap *self) {\
  CF_ENTRY_FUNC\
  cf_assert(self);\
  cf_free(self->table);\
  CF_EXIT_FUNC\
}\
\
\
cf_Error HashMap##Iterator_next(HashMap##Iterator *self) {\
  CF_ENTRY_FUNC\
  if (self->elem != NULL) {\
    if (self->elem->next != NULL) {\
      self->elem = self->elem->next;\
      CF_EXIT_FUNC return cf_Error_ok;\
    }\
  } else if (self->position > 0) {\
    CF_EXIT_FUNC return cf_Error_notfound;\
  } else if (self->position == 0) {\
    self->elem = self->parent->table;\
    if (self->elem->used) { CF_EXIT_FUNC return cf_Error_ok; }\
  }\
\
  /*increase position*/\
  do {\
    ++(self->position);\
    if ((self->position) == (self->parent->size)){\
      self->elem = NULL;\
      CF_EXIT_FUNC return cf_Error_notfound;\
    }\
    self->elem = self->parent->table + self->position;\
    if (self->elem->used) { CF_EXIT_FUNC return cf_Error_ok; }\
  } while (true);\
  CF_EXIT_FUNC return cf_Error_unknow;\
}\
\
\
\
cf_Error HashMap##Iterator_get(HashMap##Iterator *self, K *key, V *value) {\
  CF_ENTRY_FUNC\
  cf_assert(self);\
  cf_assert(key);\
  cf_assert(value);\
  cf_assert(self->elem);\
  cf_assert(self->elem->used);\
  *key = self->elem->key;\
  *value = self->elem->value;\
  CF_EXIT_FUNC return cf_Error_ok;\
}\

/*************************************************************************
* Default define
*/

/**
 * string to string map
 */
cf_HashMapTemplate(cf_HashMapSS, const char*, char*)

/**
 * int to int map
 */
cf_HashMapTemplate(cf_HashMapII, int, int)


CF_END

#endif
