#ifndef _CPPF_ARRAY_H_
#define _CPPF_ARRAY_H_

#include "cppfan/Object.h"

CF_BEGIN_NAMESPACE

template<typename T>
class Array : public Object {
  T *data;
  int capacity;
  int _size;
public:
  Array() : data(NULL), capacity(0), _size(0) {
  }

  Array(int capacity, int size = 0) : capacity(capacity), _size(size) {
    data = (T*)cf_malloc(sizeof(T)*capacity);

    //init
    if (size > 0) {
      for (int i=0; i<size; ++i) {
        new (data+i) T();
      }
    }
  }

  ~Array() {
    clear();
    cf_free(data);
  }

  void clear() {
    for (int i=0; i<_size; ++i) {
      (data+i)->~T();
    }
    _size = 0;
  }

  int _getCapacity() { return capacity; }

  int size() const { return _size; }
  void _setSize(int n) {
    cf_assert(n <= capacity);
    _size = n;
  }

  void resize(int n) {
    if (n > capacity) {
      reserver(n);
    }
    if (size() < n) {
      for (int i=size(); i<n; ++i) {
        new (data+i) T();
      }
    } else if (size() > n) {
      for (int i=n; i<_size; ++i) {
        (data+i)->~T();
      }
    }
    _setSize(n);
  }

  T *getPointer() { return data; }

  T &get(int i) const { cf_assert(i<_size); return data[i]; }

  T &operator[] (const int i) const { return get(i); }

  Array &add(const T &t) {
    if (_size == capacity) {
      addCapacity(1);
    }
    cf_assert(_size < capacity);
    new (data+_size) T(t);
    ++_size;
    return *this;
  }

  bool reserver(int capacity) {
    if (this->capacity >= capacity) return true;
    void *tmp;
    if (data) {
      tmp = cf_realloc(data, sizeof(T)*capacity);
    } else {
      tmp = cf_malloc(sizeof(T)*capacity);
    }
    cf_checkMem(tmp);
    if (tmp == NULL) return false;
    data = (T*)tmp;
    this->capacity = capacity;
    return true;
  }

  bool remove(int i) {
    if (i < 0 || i>= _size) {
      return false;
    }
    (data+i)->~T();
    char *p1 = (char *)(data+i);
    char *p2 = (char *)(data+i+1);
    int len = _size - i -1;
    memmove(p1, p2, len);
    --_size;
    return true;
  }

protected:
  void addCapacity(int minAdd) {
    int nsize;
    if (capacity>1E6) {
      nsize = capacity*2/3+minAdd;
    } else {
      nsize = capacity*2 + minAdd;
    }
    bool rc = reserver(nsize);
    CF_UNUSED(rc);
    cf_assert(rc);
  }

};

CF_END_NAMESPACE

#endif // ARRAY_H
