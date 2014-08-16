#ifndef _CPPF_OBJECT_H_
#define _CPPF_OBJECT_H_

#include "cppfan/common.h"

CF_BEGIN_NAMESPACE

/**
 * Base class for all class.
 * support reference count to manager memory.
 */
class Object {
protected:
  /**
   * current reference count, base 0
   */
  int refCount;

public:
  CF_OVERRIDE_NEW
  Object() : refCount(0) {}

  virtual ~Object() {}

  /**
   * increase reference count
   */
  void addRef() { pf_increase(&refCount); }

  /**
   * decrease reference count.
   * if no any reference will call delete self.
   */
  virtual Object *release();

  /**
   * get current reference count, base 1.
   */
  int getRefCount() const { return refCount+1; }

  virtual long hashCode() const { return (long)this; }
  virtual int compare(const Object &other) const { return (int)((long)this - (long)&other); }
  virtual bool equals(const Object &other) const { return this == &other; }

};


CF_END_NAMESPACE
#endif // OBJECT_H
