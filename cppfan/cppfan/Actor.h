#ifndef _CPPF_ACTOR_H
#define _CPPF_ACTOR_H

#include "cppfan/Object.h"

CF_BEGIN_NAMESPACE

class SimpleActor : public Object {
protected:
  cf_Actor actor;
  cf_Executor *executor;
public:
  SimpleActor() : executor(0) {}

  void initSimple(int threadSize);

  void init(cf_Executor *executor, cf_Timer *timer);

  void send(cf_ActorMessage *msg) {
    cf_Actor_send(&actor, msg);
  }

  /**
   * return false refused merge message.
   * if return true please delete oldMsg->arg.
   * only the same name message can be merged.
   */
  virtual bool mergeMessage(cf_ActorMessage *newMsg, cf_ActorMessage *oldMsg) {
    CF_UNUSED(newMsg);
    CF_UNUSED(oldMsg);
    return false;
  }

  virtual void recevie(cf_ActorMessage *msg) = 0;

  virtual ~SimpleActor();
};

/**
 * auto distribute the message by message name
 */
class Actor : public SimpleActor {
protected:
  cf_HashMapSP map;
public:
  Actor();

  virtual void recevie(cf_ActorMessage *msg);

  virtual ~Actor();
};

typedef void (*ActorFunc)(Actor *acotr, cf_ActorMessage *msg);

#define CF_ACTOR_SLOT(Type, FuncName)   private: void _##FuncName(void *arg);\
  private: static\
    void static_##FuncName(Type *self, cf_ActorMessage *msg) {\
      self->_##FuncName(msg->arg);\
    }\
  public: void FuncName(void *arg) {\
      cf_ActorMessage msg;\
      msg.arg = arg;\
      msg.name = "_" #FuncName;\
      cf_Actor_send(&actor, &msg);\
    }

#define CF_ACTOR_REGISTER(FuncName) \
  cf_HashMapSP_set(&map, "_" #FuncName, (void*)static_##FuncName, NULL, NULL)

CF_END_NAMESPACE

#endif // ACTOR_H
