#include "cppfan/Actor.h"

CF_USING_NAMESPACE

static void SimpleActor_staticReceive(struct cf_Actor_ *self, cf_ActorMessage *msg) {
  Actor *acotr = (Actor *)self->userData;
  acotr->recevie(msg);
}

static bool SimpleActor_mergeMessage(struct cf_Actor_ *self, cf_ActorMessage *newMsg, cf_ActorMessage *oldMsg) {
  Actor *acotr = (Actor *)self->userData;
  return acotr->mergeMessage(newMsg, oldMsg);
}

void SimpleActor::initSimple(int threadSize)
{
  executor = cf_new(cf_Executor);
  cf_Executor_make(executor, 100, threadSize);
  init(executor, NULL);
}

void SimpleActor::init(cf_Executor *executor, cf_Timer *timer) {
  cf_Actor_make(&actor, executor, SimpleActor_staticReceive, timer);
  actor.userData = this;
  actor.mergeMessage = SimpleActor_mergeMessage;
}

SimpleActor::~SimpleActor() {
  cf_Actor_dispose(&actor);
  if (executor) {
    cf_Executor_dispose(executor);
  }
}

//////////////////////////////////////////////////////////////////////////

Actor::Actor() {
  cf_HashMapSP_make(&map, 4);
}

void Actor::recevie(cf_ActorMessage *msg) {
  ActorFunc func;
  cf_Error err = cf_HashMapSP_get(&map, msg->name, NULL, (void**)&func);
  if (err != cf_Error_ok) {
    return;
  }

  (*func)(this, msg);
}

Actor::~Actor() {
  cf_HashMapSP_dispose(&map);
}

