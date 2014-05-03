#ifndef _CPPF_TEST_H_
#define _CPPF_TEST_H_

#include "cppfan/common.h"
#include "cppfan/Array.h"

CF_BEGIN_NAMESPACE

#define CF_DEF_TEST(name) \
  void name(void);\
  class __##name##_class {\
  public:\
    __##name##_class() {\
      Test::getInstance().add(name, 0, #name , __FILE__ , __LINE__ );\
    }\
  };\
  __##name##_class __##name##_instance;\
  void name(void)

class Test {
  struct TestElem {
    void (*func)();
    int attr;
    const char *name;
    const char *file;
    unsigned int line;
  };

  Array<TestElem> list;
  int errorCount;

private:
  static Test *instance;

private:
  Test();

public:
  static Test &getInstance();

  void add(void (*func)(), int attr, const char*name, const char *file, unsigned int line);

  void run(const char *name);
};

CF_END_NAMESPACE
#endif // _CPPF_TEST_H_
