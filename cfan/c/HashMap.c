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


cf_Error cf_HashMap_make(cf_HashMap *self, size_t size
  , size_t (*hashFunc)(cf_Value key)
  , int (*compFunc)(cf_Value v1, cf_Value v2) ) {

  CF_ENTRY_FUNC
  cf_assert(self);
  cf_assert(hashFunc);
  cf_assert(compFunc);

  self->size = size;
  self->table = (cf_HashMapElem*)cf_calloc(size, sizeof(cf_HashMapElem));
  if (NULL == self->table) {
    CF_EXIT_FUNC return cf_Error_alloc;
  }
  self->hashFunc = hashFunc;
  self->compFunc = compFunc;
  CF_EXIT_FUNC
  return cf_Error_ok;
}

cf_Error cf_HashMap_get(cf_HashMap *self, cf_Value key, cf_Value *oldKey, cf_Value *oldValue) {
  cf_HashMapElem *elem;

  CF_ENTRY_FUNC
  cf_assert(self);
  cf_assert(oldKey);
  cf_assert(oldValue);

  for (elem = self->table + (self->hashFunc(key) % self->size); elem != NULL && elem->used; elem = elem->next) {
    if (self->compFunc(key, elem->key) == 0) {
      *oldKey = elem->key;
      *oldValue = elem->value;
      CF_EXIT_FUNC
      return cf_Error_ok;
    }
  }
  oldKey->pointer = NULL;
  oldValue->pointer = NULL;
  CF_EXIT_FUNC
  return cf_Error_notfound;
}


cf_Error cf_HashMap_set(cf_HashMap *self, cf_Value key, cf_Value value, cf_Value *oldKey, cf_Value *oldValue) {
  cf_HashMapElem *elem;
  cf_HashMapElem *newElem;
  size_t hashValue;

  CF_ENTRY_FUNC
  cf_assert(self);
  cf_assert(oldKey);
  cf_assert(oldValue);

  hashValue = self->hashFunc(key) % self->size;
  for (elem = self->table + hashValue; elem != NULL && elem->used; elem = elem->next) {
    // if found
    if (self->compFunc(key, elem->key) == 0) {
      *oldKey = elem->key;
      *oldValue = elem->value;
      elem->key = key;
      elem->value = value;
      CF_EXIT_FUNC
      return cf_Error_ok;
    }
  }

  oldKey->pointer = NULL;
  oldValue->pointer = NULL;
  //now elem is last

  // if not found
  if (elem->used) {
    newElem = (cf_HashMapElem *)cf_malloc(sizeof(cf_HashMapElem));
    if (!newElem) {
      CF_EXIT_FUNC return cf_Error_alloc;
    }
    elem->next = newElem;
  } else {
    //will use this
    newElem = elem;
  }

  newElem->used = true;
  newElem->next = NULL;
  newElem->key = key;
  newElem->value = value;

  CF_EXIT_FUNC
  return cf_Error_ok;
}

void cf_HashMap_dispose(cf_HashMap *self) {
  CF_ENTRY_FUNC
  cf_assert(self);
  cf_free(self->table);
  CF_EXIT_FUNC
}

size_t cf_HashMap_strHash(cf_Value key) {
  CF_ENTRY_FUNC
  size_t hashValue;
  char *str = (char*)key.pointer;
  for (hashValue = 0; *str != '\0'; str++) {
    hashValue = *(str) + 31 * hashValue;
  }
  CF_EXIT_FUNC
  return hashValue;
}

int cf_HashMap_strComp(cf_Value v1, cf_Value v2) {
  return strcmp((char*)v1.pointer, (char*)v2.pointer);
}


