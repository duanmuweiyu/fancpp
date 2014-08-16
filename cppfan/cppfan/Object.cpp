#include "cppfan/Object.h"

CF_USING_NAMESPACE

Object *Object::release() {
  pf_decrease(&refCount);
  if (refCount < 0) {
    delete this;
    return NULL;
  }
  return this;
}
