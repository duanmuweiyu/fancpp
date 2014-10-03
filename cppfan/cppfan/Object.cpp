#include "cppfan/Object.h"

CF_USING_NAMESPACE

bool Object::addRef() {
  int oldVal = pf_increase(&refCount);
  return oldVal >= 0;
}

Object *Object::release() {
  pf_decrease(&refCount);
  if (refCount < 0) {
    dispose();
    delete this;
    return NULL;
  }
  return this;
}
