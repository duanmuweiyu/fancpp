#ifndef _CPPF_COMMON_H_
#define _CPPF_COMMON_H_

#define CF_BEGIN_NAMESPACE namespace cppfan {

#define CF_END_NAMESPACE }

#define CF_USING_NAMESPACE using namespace cppfan;

#ifndef NULL
  #define NULL 0
#endif

#define CF_FIELD(Type, name) private: Type _name;\
  public: Type name() const { return _name; }\
  public: void name(Type name_) { _name = name_; }

#define CF_READONLY_FIELD(Type, name) private: Type _name;\
  public: Type name() const { return _name; }

#define CF_FIELD_POINTER(Type, name) private: Type _name;\
  public: Type *name() const { return &_name; }

#include "cfan/cfan.h"

#endif // COMMON_H
