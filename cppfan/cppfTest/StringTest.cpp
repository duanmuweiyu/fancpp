#include "cppfan/cppfan.h"

CF_USING_NAMESPACE


CF_DEF_TEST(StringTest_test){
  StringRef s = "hello world";
  StringRef s1 = "hello world";
  StringRef s2 = s;
  cf_verify(s1 == s2);
}

CF_DEF_TEST(StringTest_test2){
  StringRef s;
  StringRef s1 = "hello world";
  s = s1;
  cf_verify(s1 == s);
}

CF_DEF_TEST(StringTest_testAdd){
  StringRef s = "hello";
  s += " world";
  StringRef s2 = "hello world";
  cf_verify(s == s2);
}
