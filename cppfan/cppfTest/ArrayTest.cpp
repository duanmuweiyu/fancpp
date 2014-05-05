#include "cppfan/cppfan.h"

CF_USING_NAMESPACE

class ArrayTestObj : public Object {
public:
  int val;
};

CF_DEF_TEST(ArrayTest_testInit){
  Array<ArrayTestObj> list(10);
  Array<int> list2(2);
  Array<ArrayTestObj*> list3(2);

  ArrayTestObj obj;
  list.add(obj);
  list2.add(123);
  list3.add(&obj);
}

CF_DEF_TEST(ArrayTest_testReserver){
  Array<int> list(1);
  list.add(123);
  list.add(124);
  cf_checkAllMem();
  cf_verify(list.size() == 2);
}

CF_DEF_TEST(ArrayTest_testRemove){
  Array<int> list(1);
  list.add(123);
  list.add(124);
  list.add(125);
  list.remove(1);
  cf_verify(list.size() == 2);
  cf_verify(list[0] == 123);
  cf_verify(list.get(1) == 125);
}
