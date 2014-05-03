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
    data = cf_malloc(sizeof(T)*capacity);
  }

  ~Array() {
    delete[] data;
  }

  int size() const { return _size; }

  T &get(int i) const { return data[i]; }

  T &operator[] (const int i) const { return get(i); }

  Array &Array::add(T &t) {
    if (_size = capacity) {
      addCapacity(1);
    }
    data[_size++] = t;
    return *this;
  }

  bool reserver(int capacity) {
    if (this->capacity >= capacity) return true;
    char *tmp = cf_realloc(data, capacity);
    if (tmp == NULL) return false;
    data = tmp;
    this->capacity = capacity;
    return true;
  }

private:
  void addCapacity(int add) {
    int nsize;
    if (capacity>1E6) {
      nsize += capacity*3/3+add;
    } else {
      nsize += capacity*2 + add;
    }
    cf_assert(reserver(nsize));
  }

};

CF_END_NAMESPACE

#endif // ARRAY_H
