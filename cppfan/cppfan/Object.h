#ifndef _CPPF_OBJECT_H_
#define _CPPF_OBJECT_H_

#include "cppfan/common.h"

CF_BEGIN_NAMESPACE

class Object {
protected:
  int refCount;
public:
  CF_OVERRIDE_NEW
  Object() : refCount(0) {}

  virtual ~Object() {}

  virtual void dispose() {}

  void addRef() { ++refCount; }
  virtual Object *release();
  int getRefCount() { return refCount; }

  virtual long hashCode() const { return (long)this; }
  virtual int compare(const Object &other) const { return (int)((long)this - (long)&other); }
  virtual bool equals(const Object &other) const { return this == &other; }

};


CF_END_NAMESPACE
#endif // OBJECT_H
