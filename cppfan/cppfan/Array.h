#ifndef _CPPF_ARRAY_H_
#define _CPPF_ARRAY_H_

#include "cppfan/Object.h"

CF_BEGIN_NAMESPACE

template<class T>
class Array : public Object {
  T *data;
  int capacity;
  int _size;
public:
  Array(int capacity) : capacity(capacity), _size(0) {
    data = (T*)cf_malloc(sizeof(T)*capacity);
  }

  ~Array() {
    for (int i=0; i<_size; ++i) {
      (data+i)->~T();
    }
    cf_free(data);
  }

  int size() const { return _size; }

  T &get(int i) const { cf_assert(i<_size); return data[i]; }

  T &operator[] (const int i) const { return get(i); }

  Array &add(T &t) {
    if (_size == capacity) {
      addCapacity(1);
    }
    cf_assert(_size < capacity);
    //memcpy(data+_size, &t, sizeof(T));
    new (data+_size) T();
    data[_size] = t;
    _size++;
    return *this;
  }

  bool reserver(int capacity) {
    if (this->capacity >= capacity) return true;
    void *tmp = cf_realloc(data, capacity);
    if (tmp == NULL) return false;
    data = (T*)tmp;
    this->capacity = capacity;
    return true;
  }

private:
  void addCapacity(int minAdd) {
    int nsize = 0;
    if (capacity>1E6) {
      nsize += capacity*3/3+minAdd;
    } else {
      nsize += capacity*2 + minAdd;
    }
    cf_assert(reserver(nsize));
  }

};

CF_END_NAMESPACE

#endif // ARRAY_H
