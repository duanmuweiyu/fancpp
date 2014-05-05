#include "cppfan/cppfan.h"

CF_USING_NAMESPACE

class LinkedListTestObj : public Object {
public:
  int val;
  LinkedListTestObj *previous;
  LinkedListTestObj *next;
};

CF_DEF_TEST(LinkedListTest_test){
  LinkedList<LinkedListTestObj> list;

  LinkedListTestObj obj;
  list.add(&obj);

  cf_verify(list.size() == 1);
}
