#include "cppfan/cppfan.h"

CF_USING_NAMESPACE

class MemoryTestSuperObj : public Object {
public:
  CF_OVERRIDE_NEW
  int val;
  MemoryTestSuperObj() { puts("superObj ctor"); }
  ~MemoryTestSuperObj() { puts("superObj dector"); }
};

class MemoryTestObj : public MemoryTestSuperObj {
public:
  CF_OVERRIDE_NEW
  int val;
  MemoryTestObj() { puts("Obj ctor"); }
  ~MemoryTestObj() { puts("Obj dector"); }
};

CF_DEF_TEST(MemoryTest_testNew){
  MemoryTestObj *obj = new MemoryTestObj();
  MemoryTestObj *obj3 = CF_NEW MemoryTestObj();

  MemoryTestObj *obj4 = new MemoryTestObj[2];
  MemoryTestObj *obj6 = CF_NEW MemoryTestObj[2];
  cf_dumpMem();
  delete obj;
  delete obj3;
  delete[] obj4;
  delete[] obj6;
}

CF_DEF_TEST(MemoryTest_testPlacementNew){
  char buffer[256];
  char buffer2[256];
  MemoryTestObj *obj2 = new (buffer) MemoryTestObj();
  MemoryTestObj *obj5 = new (buffer2) MemoryTestObj[2];
  cf_dumpMem();
  puts("-------");
  obj2->~Object();
  puts("-------");
  obj5->~MemoryTestObj();
  puts("-------");
  (obj5+1)->~MemoryTestSuperObj();
  puts("-------");
}
