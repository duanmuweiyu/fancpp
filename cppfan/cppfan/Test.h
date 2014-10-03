#ifndef _CPPF_TEST_H_
#define _CPPF_TEST_H_

#include "cppfan/common.h"
#include "cppfan/Array.h"

CF_BEGIN_NAMESPACE

#define CF_DEF_TEST(name) \
  static void name(void);\
  class __##name##_class {\
  public:\
    __##name##_class() {\
      cf_Test_init();\
      cf_Test_doAdd(name, 0, #name , __FILE__ , __LINE__ );\
    }\
  };\
  __##name##_class __##name##_instance;\
  static void name(void)

CF_END_NAMESPACE
#endif // _CPPF_TEST_H_
