#ifndef _CPPF_HASHMAP_H_
#define _CPPF_HASHMAP_H_

#include "cppfan/Object.h"
#include "cppfan/MemPool.h"
#include "cppfan/LinkedList.h"

CF_BEGIN_NAMESPACE

template<class K>
class HashFunc {
public:
  unsigned int operator()(K &key) {
    int s = sizeof(K);
    unsigned char *str = (char*)(&key);
    unsigned int hashValue;
    for (hashValue = 0, i=0; i<s; ++i) {
      hashValue = *(str) + 31 * hashValue;
    }
    return hashValue;
  }
};

template<class K, class V, class hashFunc=HashFunc<K> >
class HashMap : public Object {
  class HashMapElem {
    K key;
    V val;
    HashMapElem *next;
    HashMapElem *previous;
  };
  LinkedList<HashMapElem> *table;
  MemPool memPool;
  int _size;
  int tableSize;
public:
  HashMap(int tableSize) : tableSize(tableSize), _size(0), memPool(sizeof(HashMapElem), tableSize) {
    table = new LinkedList<HashMapElem>[tableSize];
  }

  ~HashMap() {
    delete[] table;
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
  V *operator[](K &key) { return get(key); }

  void set(K &key, V &val) {
    LinkedList<HashMapElem> &link = table[hash(key)];
    HashMapElem *elem = link.first();
    while (elem) {
      if (elem->key == key) {
        elem->val = val;
        return;
      }
      elem = elem->next;
    }
    elem = alloc();
    elem->key = key;
    elem->val = val;
    ++_size;
    link.add(elem);
  }
  void operator[](K &key, V &val) { set(key, val); }

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
    return hashFunc(key) % _size;
  }

  HashMapElem *first(K &key) {
    return table[hash(key)].first();
  }

  HashMapElem *alloc() {
    return new (memPool.alloc()) HashMapElem;
  }

  void free(HashMapElem *elem) {
    elem->~HashMapElem();
  }
};

CF_END_NAMESPACE
#endif // HASHMAP_H
