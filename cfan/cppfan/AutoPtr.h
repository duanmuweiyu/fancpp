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
  AutoPtr() : pointer(NULL) {
    refCount = new int();
    *refCount = 0;
  }

  AutoPtr(T *pointer) : pointer(pointer) {
    refCount = new int();
    *refCount = 0;
  }

  AutoPtr(const AutoPtr &other) : pointer(other.pointer) {
    ++(*other.refCount);
    this->refCount = other.refCount;
  }

  ~AutoPtr() {
    if (--(*refCount) < 0 && pointer) {
      delete pointer;
    }
  }

  AutoPtr &operator=(const AutoPtr& other) {
    ++(*other.refCount);
    if (--(*refCount) < 0 && pointer) {
      delete pointer;
    }
    pointer = other.pointer;
    return *this;
  }

  T *operator->() { return pointer; }

  T &operator*() { return *pointer; }

  T *getRawPtr() { return pointer; }
};

CF_END_NAMESPACE

#endif // AUTOPTR_H
