#ifndef ACTOR_H
#define ACTOR_H

#include "cppfan/Object.h"

CF_BEGIN_NAMESPACE

class Actor : public Object {
protected:
  cf_Actor actor;
  cf_HashMapSP map;
  Actor(){}
public:
  void make(cf_Executor *executor, cf_Timer *timer);

  virtual cf_Error recevie(cf_ActorMessage *msg);

  virtual ~Actor();
};

typedef void (*ActorFunc)(Actor *acotr, cf_ActorMessage *msg);

#define CF_ACTOR_SLOT(Type, FuncName)   private: void _##FuncName(void *arg);\
  private: static\
    void static_##FuncName(Type *self, cf_ActorMessage *msg) {\
      self->_##FuncName(msg->arg);\
    }\
  public: void FuncName(void *arg) {\
      static bool regist = false;\
      if (!regist) {\
        cf_HashMapSP_set(&map, "_" #FuncName, (void*)static_##FuncName, NULL, NULL);\
        regist = true;\
      }\
      cf_ActorMessage msg;\
      msg.arg = arg;\
      msg.name = "_" #FuncName;\
      cf_Actor_send(&actor, &msg);\
    }

CF_END_NAMESPACE

#endif // ACTOR_H
