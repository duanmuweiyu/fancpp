#include "cppfan/cppfan.h"

CF_USING_NAMESPACE


CF_DEF_TEST(StringTest_test){
  String s = "hello world";
  String s1 = "hello world";
  String s2 = s;
  cf_verify(s1 == s2);
}

