#ifndef _CPPF_HASHMAP_H_
#define _CPPF_HASHMAP_H_

#include "cppfan/Object.h"
#include "cppfan/LinkedList.h"

CF_BEGIN_NAMESPACE

template<class K>
struct HashFunc {
  unsigned int operator()(K &key) {
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
  unsigned int operator()(int &key) {
    return key;
  }
};

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
  HashMap(int tableSize) : tableSize(tableSize), _size(0) {
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

  V *get(K &key) {
    HashMapElem *elem = first(key);
    while (elem) {
      if (elem->key == key) {
        return &elem->val;
      }
      elem = elem->next;
    }
    return NULL;
  }

  V &operator[](K &key) {
    V *v = get(key);
    if (v == NULL) {
      V nv;
      return set(key, nv);
    }
    return *v;
  }

  V &set(K &key, V &val) {
    LinkedList<HashMapElem> &link = table[hash(key)];
    HashMapElem *elem = link.first();
    while (elem) {
      if (elem->key == key) {
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

  bool contains(K &key) {
    HashMapElem *elem = first(key);
    while (elem) {
      if (elem->key == key) {
        return true;
      }
      elem = elem->next;
    }
    return false;
  }

  bool remove(K &key) {
    HashMapElem *elem = first(key);
    while (elem) {
      if (elem->key == key) {
        free(elem);
        --_size;
        return true;
      }
      elem = elem->next;
    } while (elem);
    return false;
  }

private:
  unsigned int hash(K &key) {
    hashFunc func;
    return func(key) % tableSize;
  }

  HashMapElem *first(K &key) {
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
