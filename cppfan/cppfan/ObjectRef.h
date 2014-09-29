#ifndef OBJECTREF_H
#define OBJECTREF_H

#include "cppfan/common.h"
#include "cppfan/Object.h"

CF_BEGIN_NAMESPACE

/**
 * It similar to AutoPtr, the T must be extend from Object
 */
template<class T>
class ObjectRef : public Object {
  T *pointer;
public:
  ObjectRef() : pointer(NULL) {
  }

  ObjectRef(T *pointer) : pointer(pointer) {
  }

  ObjectRef(const ObjectRef &other) : pointer(other.pointer) {
    other.pointer->addRef();
  }

  virtual ~ObjectRef() {
    if (pointer) {
      pointer->release();
    }
  }

  ObjectRef &operator=(const ObjectRef& other) {
    other.pointer->addRef();
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

  virtual long hashCode() const { return pointer->hashCode(); }
  virtual bool equals(const Object &other) const { return pointer->equals(other); }
  virtual int compare(const Object &other) const { return pointer->compare(other); }
};

CF_END_NAMESPACE

#endif // OBJECTREF_H
