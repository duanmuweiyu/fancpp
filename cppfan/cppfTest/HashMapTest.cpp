#include "cppfan/cppfan.h"

CF_USING_NAMESPACE

class HashMapTestObj : public Object {
public:
  int val;

  HashMapTestObj() {
    val = 10;
  }

  bool operator== (HashMapTestObj &other) {
    return val == other.val;
  }
};

CF_DEF_TEST(HashMapTest_testObj) {
  HashMap<HashMapTestObj, HashMapTestObj> map(4);
  HashMapTestObj obj;
  obj.val = 10;
  map[obj] = obj;
  HashMapTestObj t = map[obj];

  cf_verify(t.val == 10);
}

CF_DEF_TEST(HashMapTest_testInt) {
  HashMap<int, int> map(4);
  map[1] = 10;
  int t = map[1];

  cf_verify(t == 10);
}

CF_DEF_TEST(HashMapTest_testRemove) {
  HashMap<int, int> map(4);
  map[1] = 10;

  cf_verify(map.contains(1));
  map.remove(1);
  cf_verify(!map.contains(1));
}
