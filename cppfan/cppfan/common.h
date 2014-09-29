#ifndef _CPPF_COMMON_H_
#define _CPPF_COMMON_H_

#define CF_BEGIN_NAMESPACE namespace cppfan {

#define CF_END_NAMESPACE }

#define CF_USING_NAMESPACE using namespace cppfan;

#ifndef NULL
  #define NULL 0
#endif

#define CF_FIELD(Type, name) private: Type _##name;\
  public: Type name() const { return _##name; }\
  public: void name(Type name_) { _##name = name_; }\
  private:

#define CF_READONLY_FIELD(Type, name) private: Type _##name;\
  public: Type name() const { return _##name; }\
  private:

#define CF_FIELD_POINTER(Type, name) private: Type _##name;\
  public: Type *name() { return &_##name; }\
  private:

#define CF_FIELD_REF(Type, name) private: Type _##name;\
  public: Type &name() { return _##name; }\
  private:

#define CF_FIELD_CONST_REF(Type, name) private: Type _##name;\
  public: Type &name() const { return _##name; } \
  public: void name(Type &name_) { _##name = name_; }\
  private:

#include "cfan/cfan.h"

#endif // COMMON_H
