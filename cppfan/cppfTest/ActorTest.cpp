#include "cppfan/cppfan.h"

CF_USING_NAMESPACE


class MyActor : public Actor {
  CF_ACTOR_SLOT(MyActor, foo)
};

void MyActor::_foo(void *arg) {
  printf("%s\n", (char *)arg);
  fflush(stdout);
}

CF_DEF_TEST(ActorTest_test){
  cf_Executor executor;
  MyActor actor;
  cf_Executor_make(&executor, 4, 300);
  actor.init(&executor, NULL);

  actor.foo((void*)"hi");

  cf_System_sleep(500);
  cf_Executor_dispose(&executor);
}

