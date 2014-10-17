#ifndef _CPPF_CALLBACK_H
#define _CPPF_CALLBACK_H

#include "cppfan/common.h"

CF_BEGIN_NAMESPACE

class Callback {
public:
  virtual void *call(void *arg) = 0;
  virtual  ~Callback(){}
};

CF_END_NAMESPACE

#endif // CALLBACK_H
