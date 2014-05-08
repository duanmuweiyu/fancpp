#include "cppfan/cppfan.h"

CF_USING_NAMESPACE

class AutoPtrTestObj : public Object {
public:
  int val;
};

CF_DEF_TEST(AutoPtrTest_test){
  AutoPtr<AutoPtrTestObj> p1 = new AutoPtrTestObj();
}


CF_DEF_TEST(AutoPtrTest_testAssign){
  AutoPtr<AutoPtrTestObj> p1 = new AutoPtrTestObj();
  AutoPtr<AutoPtrTestObj> p2 = p1;
  AutoPtr<AutoPtrTestObj> p3 = p2;
  AutoPtr<AutoPtrTestObj> p4 = p1;
}
