#include "cppfan/Test.h"

CF_USING_NAMESPACE

Test *Test::instance = NULL;

Test::Test() : list(100), errorCount(0) {
  printf("Test int\n");
  fflush(stdout);
}

Test &Test::getInstance() {
  if (instance == NULL) {
    instance = new Test();
  }
  return *instance;
}

void Test::add(void (*func)(), int attr, const char*name, const char *file, unsigned int line) {
  TestElem elem;
  elem.func = func;
  elem.attr = attr;
  elem.name = name;
  elem.file = file;
  elem.line = line;
  list.add(elem);
}

void Test::run(const char *name) {
  int i;
  int snapshot;
  int count = 0;

  cf_Log_log("test", cf_LogLevel_info, "start run test");

  for (i=0; i<list.size(); ++i) {
    cf_Log_log("test", cf_LogLevel_info, "run: %s", list.get(i).name);
    snapshot = errorCount;
    list.get(i).func();
    if (errorCount != snapshot) {
      ++errorCount;
    }
    ++count;
    cf_Log_log("test", cf_LogLevel_info, "end: %s", list.get(i).name);
  }

  cf_Log_log("test", cf_LogLevel_info
             , "test end: fail %d, total %d, runed %d", errorCount, list.size(), count);
  if (errorCount > 0) {
    cf_Log_log("test", cf_LogLevel_info, "========test FAIL========");
  } else {
    cf_Log_log("test", cf_LogLevel_info, "========test SUCCESS========");
  }
}
