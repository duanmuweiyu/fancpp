#ifndef _CPPF_HASHMAP_H_
#define _CPPF_HASHMAP_H_

#include "cppfan/Object.h"
#include "cppfan/LinkedList.h"
#include "cppfan/ObjectRef.h"
#include "cppfan/String.h"

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

template<>
struct HashFunc<StringRef> {
  unsigned int operator()(const StringRef &key) {
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
  class Iterator {
    HashMap *parent;
    int position;
    HashMapElem *elem;
  public:
    Iterator(HashMap *parent) :
      parent(parent), position(0), elem(NULL) {
    }

  public:
    K &getK() {
      return elem->key;
    }

    V &getV() {
      return elem->val;
    }

    void reset() {
      elem = NULL;
      position = 0;
    }

    /**
     * move to next elem.
     * return false at end of. return true has more element.
     */
    bool next() {
      //normal
      if (elem != NULL && elem != parent->table[position].end()) {
        if (elem->next != parent->table[position].end()) {
          elem = elem->next;
          return true;
        }
      }

      if (elem == NULL) {
        //finised
        if (position > 0) {
          return false;
        }
        //unstart
        else if (position == 0) {
          elem = parent->table[0].first();
          if (elem != parent->table[position].end()) { return true; }
        }
      }

      /*increase position*/
      do {
        ++(position);
        if ((position) == (parent->tableSize)){
          elem = NULL;
          return false;
        }
        //find new linkedlist
        elem = parent->table[position].first();
        if (elem != parent->table[position].end()) { return true; }
      } while (true);
      return false;
    }
  };
  friend class Iterator;
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
      while (elem != list->end()) {
        next = elem->next;
        free(elem);
        elem = next;
      }
    }
    delete[] table;
    cf_MemoryPool_dispose(&memPool);
  }

  Iterator getIterator() {
    return Iterator(this);
  }

  int size() const { return _size; }

  V *get(const K &key) {
    LinkedList<HashMapElem> &link = getLink(key);
    HashMapElem *elem = link.first();
    while (elem != link.end()) {
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
      V nv = V();
      return set(key, nv);
    }
    return *v;
  }

  V &set(const K &key, V &val) {
    LinkedList<HashMapElem> &link = table[hash(key)];
    HashMapElem *elem = link.first();
    while (elem != link.end()) {
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

  bool contains(const K &key) const {
    LinkedList<HashMapElem> &link = getLink(key);
    HashMapElem *elem = link.first();
    while (elem != link.end()) {
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
    while (elem != link.end()) {
      if (elem->key == key) {
        link.remove(elem);
        free(elem);
        --_size;
        return true;
      }
      elem = elem->next;
    };
    return false;
  }

  void clear() {
    for (int i=0; i<tableSize; ++i) {
      LinkedList<HashMapElem> &link = table[i];
      HashMapElem *elem = link.first();
      while (elem != link.end()) {
        HashMapElem *temp = elem;
        elem = elem->next;
        link.remove(temp);
        free(temp);
        --_size;
      };
    }
    cf_assert(_size == 0);
  }

private:
  unsigned int hash(const K &key) const {
    hashFunc func;
    return func(key) % tableSize;
  }

  LinkedList<HashMapElem> &getLink(const K &key) const {
    return const_cast<HashMap*>(this)->table[hash(key)];
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
