#include "cppfan/cppfan.h"

CF_USING_NAMESPACE

class TestObj : public Object {
public:
  CF_OVERRIDE_NEW
  int val;
};

CF_DEF_TEST(MemoryTest_test){
  TestObj *obj = CF_NEW TestObj();
  cf_dumpMem();
  delete obj;
}
