#include "cppfan/cppfan.h"

CF_USING_NAMESPACE

class CacheTestObj : public Object {
};

CF_DEF_TEST(CacheTest_test) {
  Cache<StringRef, ObjectRef<CacheTestObj> > cache(2);
  StringRef str = "123";
  StringRef str2 = "124";
  StringRef str3 = "125";
  ObjectRef<CacheTestObj> obj = _new CacheTestObj();
  ObjectRef<CacheTestObj> obj2 = _new CacheTestObj();
  ObjectRef<CacheTestObj> obj3 = _new CacheTestObj();

  cache.set(str, obj);
  cache.set(str2, obj2);
  cache.set(str3, obj3);

  cf_verify(cache.size() == 2);
  cf_verify(cache.contains(str) == false);
  cf_verify(cache.contains(str2));
  cf_verify(cache.contains(str3));
}

CF_DEF_TEST(CacheTest_testRef) {
  Cache<StringRef, CacheTestObj* > cache(2);
  StringRef str = "123";
  StringRef str2 = "124";
  StringRef str3 = "125";
  CacheTestObj *obj = _new CacheTestObj();
  CacheTestObj *obj2 = _new CacheTestObj();
  CacheTestObj *obj3 = _new CacheTestObj();

  cache.set(str, obj);
  cache.set(str2, obj2);
  cache.set(str3, obj3);

  cf_verify(cache.size() == 2);
  cf_verify(cache.contains(str) == false);
  cf_verify(cache.contains(str2));
  cf_verify(cache.contains(str3));
}
