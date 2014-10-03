#ifndef _CPPF_OBJECTREF_H
#define _CPPF_OBJECTREF_H

#include "cppfan/common.h"
#include "cppfan/Object.h"

CF_BEGIN_NAMESPACE

/**
 * It similar to AutoPtr, the T must be extend from Object
 */
template<class T>
class ObjectRef {
  T *pointer;
public:
  ObjectRef() : pointer(NULL) {
  }

  ObjectRef(T *pointer) : pointer(pointer) {
  }

  ObjectRef(const ObjectRef &other) : pointer(other.pointer) {
    if (other.pointer) {
      bool ok = other.pointer->addRef();
      cf_assert(ok);
    }
  }

  virtual ~ObjectRef() {
    clear();
  }

  ObjectRef &operator=(const ObjectRef& other) {
    if (other.pointer) {
      bool ok = other.pointer->addRef();
      cf_assert(ok);
    }
    if (pointer) {
      pointer->release();
    }
    pointer = other.pointer;
    return *this;
  }

  T *operator->() { return pointer; }

  T &operator*() { return *pointer; }

  T *getRawPtr() { return pointer; }

  void setRawPtr(T *p) { pointer = p; }

  bool isNull() { return pointer == NULL; }

  void clear() {
    if (pointer) {
      pointer->release();
      pointer = NULL;
    }
  }

  virtual long hashCode() const {
    if (pointer) {
      return pointer->hashCode();
    }
    return 0;
  }
  virtual bool equals(const Object &other) const {
    if (pointer) {
      return pointer->equals(other);
    } else {
      return 0;
    }
  }
  virtual int compare(const Object &other) const {
    if (pointer) {
      return pointer->compare(other);
    } else {
      return 0;
    }
  }
};

CF_END_NAMESPACE

#endif // OBJECTREF_H
