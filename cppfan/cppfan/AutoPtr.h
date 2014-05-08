#ifndef _CPPF_AUTOPTR_H_
#define _CPPF_AUTOPTR_H_

#include "common.h"

CF_BEGIN_NAMESPACE

template<class T>
class AutoPtr
{
  T *pointer;
  int *refCount;
public:
  bool isDelete;
public:
  AutoPtr() : pointer(NULL) {
    refCount = new int();
    *refCount = 0;
    isDelete = true;
  }

  AutoPtr(T *pointer) : pointer(pointer) {
    refCount = new int();
    *refCount = 0;
    isDelete = true;
  }

  AutoPtr(const AutoPtr &other) : pointer(other.pointer), isDelete(other.pointer) {
    ++(*other.refCount);
    this->refCount = other.refCount;
  }

  ~AutoPtr() {
    if (--(*refCount) < 0) {
      delete refCount;
      free(pointer);
    }
  }

  AutoPtr &operator=(const AutoPtr& other) {
    ++(*other.refCount);
    if (--(*refCount) < 0) {
      delete refCount;
      free(pointer);
    }
    refCount = other.refCount;
    pointer = other.pointer;
    isDelete = other.isDelete;
    return *this;
  }

  T *operator->() { return pointer; }

  T &operator*() { return *pointer; }

  T *getRawPtr() { return pointer; }

  void setRawPtr(T *p) { pointer = p; }

private:
  void free(T *d) {
    if (d == NULL) return;
    if (isDelete) {
      delete d;
    } else {
      cf_free(d);
    }
  }
};

CF_END_NAMESPACE

#endif // AUTOPTR_H
