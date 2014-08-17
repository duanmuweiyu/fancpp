#ifndef _CPPF_ARRAY_H_
#define _CPPF_ARRAY_H_

#include "cppfan/Object.h"

CF_BEGIN_NAMESPACE

/**
 */
template<typename T>
class Array : public Object {
  T *data;
  unsigned int capacity;
  unsigned int _size;
public:
  Array() : data(NULL), capacity(0), _size(0) {
  }

  Array(unsigned int capacity, unsigned int size = 0) : capacity(capacity), _size(size) {
    data = (T*)cf_malloc(sizeof(T)*capacity);

    //init
    if (size > 0) {
      for (unsigned int i=0; i<size; ++i) {
        new (data+i) T();
      }
    }
  }

  ~Array() {
    clear();
    cf_free(data);
  }

  void _initByRawData(T *data, unsigned int capacity, unsigned int size) {
    this->data = data;
    this->capacity = capacity;
    this->_size = size;
  }

  /**
   * remove all element from array, the memory is not change.
   */
  void clear() {
    for (unsigned int i=0; i<_size; ++i) {
      (data+i)->~T();
    }
    _size = 0;
  }

  /**
   * get the capacity of raw memory
   */
  unsigned int _getCapacity() { return capacity; }

  /**
   * get the elem count of array
   */
  unsigned int size() const { return _size; }

  /**
   * resize but not init new add element.
   */
  void _setSize(unsigned int n) {
    cf_assert(n <= capacity);
    _size = n;
  }

  /**
   * ensure n elem in array
   */
  void resize(unsigned int n) {
    if (n > capacity) {
      reserver(n);
    }
    if (size() < n) {
      for (unsigned int i=size(); i<n; ++i) {
        new (data+i) T();
      }
    } else if (size() > n) {
      for (unsigned int i=n; i<_size; ++i) {
        (data+i)->~T();
      }
    }
    _setSize(n);
  }

  /**
   * get the start raw data of array
   */
  T *getPointer() { return data; }

  /**
   * get element at i
   */
  T &get(unsigned int i) const { cf_assert(i<_size); return data[i]; }

  /**
   * same ot get(i)
   */
  T &operator[] (const unsigned int i) const { return get(i); }

  /**
   * push back of array
   */
  Array &add(const T &t) {
    if (_size == capacity) {
      addCapacity(1);
    }
    cf_assert(_size < capacity);
    new (data+_size) T(t);
    ++_size;
    return *this;
  }

  /**
   * ensure capacity is exactly is @param capacity
   */
  bool reserver(unsigned int capacity) {
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

  /**
   * remove element at i
   * @return return true if success, return false if i out of range
   */
  bool remove(unsigned int i) {
    if (i>= _size) {
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
  /**
   * increase the capacity at lest @param minAdd
   */
  void addCapacity(unsigned int minAdd) {
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
