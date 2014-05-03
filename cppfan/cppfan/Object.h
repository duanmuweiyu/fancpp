#ifndef _CPPF_OBJECT_H_
#define _CPPF_OBJECT_H_

#include "cppfan/common.h"

CF_BEGIN_NAMESPACE

class Object {
  int refCount;
public:
  Object() : refCount(0) {}
  virtual ~Object() {}

  void addRef() { ++refCount; }
  Object *release();
};


CF_END_NAMESPACE
#endif // OBJECT_H
