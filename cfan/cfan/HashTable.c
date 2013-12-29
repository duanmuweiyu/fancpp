
#include "cfan/HashTable.h"
#include "cfan/Trace.h"
#include "cfan/Memory.h"
#include "cfan/miss.h"
#include "cfan/Util.h"

#include <string.h>
#include <stdio.h>

static inline void copyKey(cf_HashTable *self, void * dest, const void * src) {
  cf_memcpy(dest, src, self->keySize);
}

static inline void copyValue(cf_HashTable *self, void * dest, const void * src) {
  cf_memcpy(dest, src, self->valueSize);
}

static inline void * getKey(cf_HashTable *self, cf_HashTableElem *elem) {
  CF_UNUSED(self);
  return ((char*)elem + sizeof(cf_HashTableElem));
}

static inline void * getValue(cf_HashTable *self, cf_HashTableElem *elem) {
  return ((char*)elem + sizeof(cf_HashTableElem) + self->keySize);
}

static inline cf_HashTableElem *getAt(cf_HashTable *self, size_t i) {
  return (cf_HashTableElem*)(self->table + (i*self->elemSize));
}

int cf_HashTable_compFunc(const void *k1, const void *k2, int n) {
  return memcmp(k1, k2, n);
}

size_t cf_HashTable_hashFunc(const void *key, int len) {
  const unsigned char * d = (const unsigned char *)key;
  uint32_t h = 0;
  for (int i = 0; i < len; ++i) {
    h = (h << 5) - h + d[i];
  }
  return h;
}

cf_Error cf_HashTable_make(cf_HashTable *self, size_t size, int keySize, int valueSize) {
  int elemSize;
  cf_assert(self);

  self->tableSize = size;
  self->size = 0;
  self->idle = NULL;
  self->hashFunc = cf_HashTable_hashFunc;
  self->compFunc = cf_HashTable_compFunc;
  self->keySize = keySize;
  self->valueSize = valueSize;
  elemSize = (CF_ALIGN(sizeof(cf_HashTableElem)) + self->keySize + self->valueSize);
  self->elemSize = elemSize;
  self->table = (char*)cf_calloc(size, elemSize);
  if (NULL == self->table) {
    return cf_Error_alloc;
  }
  return cf_Error_ok;
}

cf_Error cf_HashTable_get(cf_HashTable *self, const void * key
                          , void *oldKey, void *oldValue) {
  cf_HashTableElem *elem;
  size_t hashValue;

  cf_assert(self);
  cf_assert(oldValue);

  hashValue = self->hashFunc(key, self->keySize) % self->tableSize;
  for (elem = getAt(self, hashValue)
       ; elem != NULL && elem->used; elem = elem->next) {
    if (self->compFunc(key, getKey(self, elem), self->keySize) == 0) {
      if (oldKey) copyKey(self, oldKey, getKey(self, elem));
      copyValue(self, oldValue, getValue(self, elem));
      return cf_Error_ok;
    }
  }
  return cf_Error_notfound;
}


cf_Error cf_HashTable_set(cf_HashTable *self, const void * key
                          , const void * value, void *oldKey, void *oldValue) {
  cf_HashTableElem *elem;
  cf_HashTableElem *newElem;
  size_t hashValue;

  cf_assert(self);

  hashValue = self->hashFunc(key, self->keySize) % self->tableSize;

  elem = getAt(self, hashValue);
  while ( elem->used ) {
    /* if found*/
    if (self->compFunc(key, getKey(self, elem), self->keySize) == 0) {
      if (oldKey) copyKey(self, oldKey, getKey(self, elem));
      if (oldValue) copyValue(self, oldValue, getValue(self, elem));
      copyKey(self, getKey(self, elem), key);
      copyValue(self, getValue(self, elem), value);
      return cf_Error_ok;
    }
    if (elem->next == NULL) break;
    elem = elem->next;
  }

  /*now elem is last*/

  /* if not found*/
  if (elem->used) {
    if (self->idle != NULL) {
      newElem = self->idle;
      self->idle = newElem->next;
    } else {
      newElem = (cf_HashTableElem *)cf_malloc(self->elemSize);
      if (!newElem) {
        return cf_Error_alloc;
      }
    }
    elem->next = newElem;
  } else {
    /*will use this*/
    newElem = elem;
  }

  newElem->used = true;
  newElem->next = NULL;
  copyKey(self, getKey(self, newElem), key);
  copyValue(self, getValue(self, newElem), value);

  ++self->size;

  return cf_Error_ok;
}

cf_Error cf_HashTable_remove(cf_HashTable *self, const void * key
                             , void *oldKey, void *oldValue) {
  cf_HashTableElem *elem;
  cf_HashTableElem *temp = NULL;

  CF_ENTRY_FUNC
  cf_assert(self);

  for (elem = getAt(self, self->hashFunc(key, self->keySize) % self->tableSize)
       ; elem != NULL && elem->used; elem = elem->next) {
    if (self->compFunc(key, getKey(self, elem), self->keySize) == 0) {
      if (oldKey) copyKey(self, oldKey, getKey(self, elem));
      if (oldValue) copyValue(self, oldValue, getValue(self, elem));
      if (temp != NULL) {
        temp->next = elem->next;
        elem->next = self->idle;
        self->idle = elem;
      } else if (temp == NULL && elem->next) {
        temp = elem->next;
        *elem = *(elem->next);
        temp->next = self->idle;
        self->idle = temp;
        cf_assert(elem->used);
        --self->size;
        CF_EXIT_FUNC
        return cf_Error_ok;
      }
      elem->used = false;
      --self->size;
      CF_EXIT_FUNC
      return cf_Error_ok;
    }
    temp = elem;
  }
  CF_EXIT_FUNC
  return cf_Error_notfound;
}

void cf_HashTable_dispose(cf_HashTable *self) {
  size_t i;
  cf_HashTableElem *elem;
  cf_HashTableElem *next = NULL;

  CF_ENTRY_FUNC
  cf_assert(self);

  for (i=0; i < self->tableSize; ++i) {
    elem = getAt(self, i);
    if (elem->used && elem->next != NULL) {
      elem = elem->next;
      while (elem->used) {
        next = elem->next;
        cf_free(elem);
        if (next == NULL) break;
        elem = next;
      }
    }
  }

  if (self->idle != NULL) {
    elem = self->idle;
    while (true) {
      next = elem->next;
      cf_free(elem);
      if (next == NULL) break;
      elem = next;
    }
  }

  cf_free(self->table);
  self->size = 0;
  CF_EXIT_FUNC
}

size_t cf_HashTable_count(cf_HashTable *self) {
  size_t i;
  cf_HashTableElem *elem;
  size_t maxCount = 0;
  size_t count = 0;
  size_t allCount = 0;
  size_t emptyCount = 0;

  cf_assert(self);
  for (i=0; i < self->tableSize; ++i) {
    elem = getAt(self, i);
    if (elem->used) {
      count = 1;
      ++allCount;
      while (elem->next) {
        elem = elem->next;
        ++count;
        ++allCount;
      }
      if (maxCount < count) {
        maxCount = count;
      }
    } else {
      ++emptyCount;
    }
  }
  cf_assert(self->size == allCount);
//  printf("cf_HashTableCount: tableSize:%d, size:%d=%d, maxList:%d, empty:%dn"
//         , (int)self->tableSize, (int)self->size, (int)allCount
//         , (int)maxCount, (int)emptyCount);
  return maxCount;
}


cf_Error cf_HashTableIterator_next(cf_HashTableIterator *self) {
  if (self->elem != NULL) {
    if (self->elem->next != NULL) {
      self->elem = self->elem->next;
      return cf_Error_ok;
    }
  } else if (self->position > 0) {
    return cf_Error_notfound;
  } else if (self->position == 0) {
    self->elem = getAt(self->parent, 0);
    if (self->elem->used) { return cf_Error_ok; }
  }

  /*increase position*/
  do {
    ++(self->position);
    if ((self->position) == (self->parent->tableSize)){
      self->elem = NULL;
      return cf_Error_notfound;
    }
    self->elem = getAt(self->parent, self->position);
    if (self->elem->used) { return cf_Error_ok; }
  } while (true);
  return cf_Error_unknow;
}

cf_Error cf_HashTableIterator_get(cf_HashTableIterator *self, void *key, void *value) {
  cf_assert(self);
  cf_assert(key);
  cf_assert(value);
  cf_assert(self->elem);
  cf_assert(self->elem->used);
  copyKey(self->parent, key, getKey(self->parent, self->elem));
  copyValue(self->parent, value, getValue(self->parent, self->elem));
  return cf_Error_ok;
}
