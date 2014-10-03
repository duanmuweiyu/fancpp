#ifndef _CPPF_CACHE_H
#define _CPPF_CACHE_H

#include "cppfan/Object.h"
#include "cppfan/HashMap.h"
#include "cppfan/LinkedList.h"

CF_BEGIN_NAMESPACE

template<typename K, typename V>
class Cache : public Object {
  struct CacheItem {
    K key;
    V val;
    CacheItem *previous;
    CacheItem *next;
  };
  HashMap<K,CacheItem*> map;
  LinkedList<CacheItem> list;
  CF_FIELD(long, maxSize)
public:
  Cache(long maxSize) : map(maxSize), _maxSize(maxSize) {
  }

  ~Cache() {
    clear();
  }

  V &get(K &key) {
    CacheItem *item = *map.get(key);
    list.remove(item);
    list.insertFirst(item);
    return item->val;
  }

  V &_get(K &key) {
    CacheItem *item = *map.get(key);
    return item->val;
  }

  void set(K &key, V &val) {
    CacheItem *item = new CacheItem();
    item->key = key;
    item->val = val;
    map.set(key, item);
    list.insertFirst(item);

    clearUp(maxSize());
  }

  bool contains(K &key) const {
    return map.contains(key);
  }

  long size() const { return map.size(); }

  void clear() {
    clearUp(0);
  }

protected:
  virtual void onRemove(K &key, V &val) {
    CF_UNUSED(key);
    cf_deleteIt(val);
  }

private:
  void clearUp(long max) {
    while (map.size() > max) {
      CacheItem *item = list.last();
      map.remove(item->key);
      list.remove(item);
      onRemove(item->key, item->val);
      delete item;
    }
  }
};

CF_END_NAMESPACE

#endif // CACHE_H
