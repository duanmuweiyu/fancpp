#ifndef _CF_cf_HashTable_H_
#define _CF_cf_HashTable_H_

#include "cfan/Error.h"

CF_BEGIN

/*
* Hash map is a container that contains key-value pairs with unique keys.
* And supports fast search by key.
*/

/**
 * Element
 *
 */
typedef struct cf_HashTableElem_ {
  bool used; /*flag for null*/
  struct cf_HashTableElem_ *next;
  //key position
  //value position
} cf_HashTableElem;

/**
 * Map
 *
 */
typedef size_t (*cf_HashTableHashFunc)(const void *, int);
typedef int (*cf_HashTableCompFunc)(const void *, const void *, int);

/**
 * build in hash and compare function
 */
size_t cf_HashTable_hashFunc(const void *key, int len);
int cf_HashTable_compFunc(const void *k1, const void *k2, int n);
int cf_HashTable_strCmp(const void *k1, const void *k2, int n);
size_t cf_HashTable_strHash(const void *str, int len);

typedef struct cf_HashTable_ {
  size_t    tableSize;
  size_t    size;
  unsigned int keySize;
  unsigned int valueSize;
  unsigned int elemSize; //elemHeaderSize + keySize + valueSize
  char *table;
  cf_HashTableElem *idle;/*deleted elements*/
  cf_HashTableHashFunc hashFunc;
  cf_HashTableCompFunc compFunc;
} cf_HashTable;
/**
 * Iterator
 *
 */
 typedef struct cf_HashTableIterator_ {
   cf_HashTable *parent;
   size_t position;
   cf_HashTableElem *elem;
 } cf_HashTableIterator;

/**
 * constructor
 *
 */
cf_Error cf_HashTable_make(cf_HashTable *self, size_t size, int keySize, int valueSize);

/**
 * lookup
 *
 */
cf_Error cf_HashTable_get(cf_HashTable *self, const void * key
                          , void *oldKey, void *oldValue);

/**
 * put
 *
 */
cf_Error cf_HashTable_set(cf_HashTable *self, const void * key
                          , const void * value, void *oldKey, void *oldValue);
/**
 * remove
 *
 */
cf_Error cf_HashTable_remove(cf_HashTable *self, const void * key
                             , void *oldKey, void *oldValue);

/**
 * destroy content
 *
 */
void cf_HashTable_dispose(cf_HashTable *self);
/**
 * print debug message
 */
size_t cf_HashTable_count(cf_HashTable *self);


cf_Error cf_HashTableIterator_next(cf_HashTableIterator *self);

cf_Error cf_HashTableIterator_get(cf_HashTableIterator *self, void *key, void *value);

static inline void cf_HashTable_createIterator(cf_HashTable *self, cf_HashTableIterator *iter) {
  cf_assert(self);
  cf_assert(iter);
  iter->parent = self;
  iter->elem = NULL;
  iter->position = 0;
}


CF_END

#endif // _CF_cf_HashTable_H_
