#include "cppfan/Actor.h"

CF_USING_NAMESPACE

static void staticReceive(struct cf_Actor_ *self, cf_ActorMessage *msg) {
  Actor *acotr = (Actor *)self->userData;
  acotr->recevie(msg);
}

void Actor::make(cf_Executor *executor, cf_Timer *timer) {
  cf_Actor_make(&actor, executor, staticReceive, timer);
  cf_HashMapSP_make(&map, 4);
  actor.userData = this;
}

cf_Error Actor::recevie(cf_ActorMessage *msg) {
  ActorFunc func;
  cf_Error err = cf_HashMapSP_get(&map, msg->name, NULL, (void**)&func);
  if (err != cf_Error_ok) {
    return err;
  }

  (*func)(this, msg);
  return cf_Error_ok;
}

Actor::~Actor() {
  cf_Actor_dispose(&actor);
  cf_HashMapSP_dispose(&map);
}
