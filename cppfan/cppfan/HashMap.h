#ifndef _CPPF_HASHMAP_H_
#define _CPPF_HASHMAP_H_

#include "cppfan/Object.h"
#include "cppfan/LinkedList.h"
#include "cppfan/ObjectRef.h"

CF_BEGIN_NAMESPACE

/*========================================================================
 * Hash Function
 */

template<class K>
struct HashFunc {
  unsigned int operator()(const K &key) {
    int s = sizeof(K);
    unsigned char *str = (unsigned char*)(&key);
    unsigned int hashValue;
    int i;
    for (hashValue = 0, i=0; i<s; ++i) {
      hashValue = *(str) + 31 * hashValue;
    }
    return hashValue;
  }
};

template<>
struct HashFunc<int> {
  unsigned int operator()(const int &key) {
    return key;
  }
};

template<>
struct HashFunc<Object> {
  unsigned int operator()(const Object &key) {
    return key.hashCode();
  }
};

/*========================================================================
 * Hash Map
 */

template<typename K, typename V, typename hashFunc=HashFunc<K> >
class HashMap : public Object {
  struct HashMapElem {
    K key;
    V val;
    HashMapElem *next;
    HashMapElem *previous;
  };
  LinkedList<HashMapElem> *table;
  cf_MemoryPool memPool;
  int _size;
  int tableSize;
public:
  HashMap(int tableSize) : _size(0), tableSize(tableSize) {
    table = new LinkedList<HashMapElem>[tableSize];
    cf_MemoryPool_make(&memPool, sizeof(HashMapElem), tableSize);
  }

  ~HashMap() {
    for (int i=0; i<tableSize; ++i) {
      LinkedList<HashMapElem> *list = table+i;
      HashMapElem *elem = list->first();
      HashMapElem *next;
      while (elem) {
        next = elem->next;
        free(elem);
        elem = next;
      }
    }
    delete[] table;
    cf_MemoryPool_dispose(&memPool);
  }

  int size() { return _size; }

  V *get(const K &key) {
    HashMapElem *elem = first(key);
    while (elem) {
      if (elem->key == (K &)key) {
        return &elem->val;
      }
      elem = elem->next;
    }
    return NULL;
  }

  V &operator[](const K &key) {
    V *v = get(key);
    if (v == NULL) {
      V nv;
      return set(key, nv);
    }
    return *v;
  }

  V &set(const K &key, V &val) {
    LinkedList<HashMapElem> &link = table[hash(key)];
    HashMapElem *elem = link.first();
    while (elem) {
      if (elem->key == (K &)key) {
        elem->val = val;
        return elem->val;
      }
      elem = elem->next;
    }
    elem = alloc();
    elem->key = key;
    elem->val = val;
    ++_size;
    link.add(elem);
    return elem->val;
  }

  bool contains(const K &key) {
    HashMapElem *elem = first(key);
    while (elem) {
      if (elem->key == key) {
        return true;
      }
      elem = elem->next;
    }
    return false;
  }

  bool remove(const K &key) {
    LinkedList<HashMapElem> &link = table[hash(key)];
    HashMapElem *elem = link.first();
    while (elem) {
      if (elem->key == key) {
        link.remove(elem);
        free(elem);
        --_size;
        return true;
      }
      elem = elem->next;
    } while (elem);
    return false;
  }

private:
  unsigned int hash(const K &key) {
    hashFunc func;
    return func(key) % tableSize;
  }

  HashMapElem *first(const K &key) {
    return table[hash(key)].first();
  }

  HashMapElem *alloc() {
    void *p = cf_MemoryPool_alloc(&memPool);
    return new (p) HashMapElem;
  }

  void free(HashMapElem *elem) {
    elem->~HashMapElem();
    cf_MemoryPool_free(&memPool, elem);
  }
};

CF_END_NAMESPACE
#endif // HASHMAP_H
