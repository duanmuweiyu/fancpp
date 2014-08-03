/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-20  Jed Young  Creation
 */

#include "cfan/Timer.h"
#include "cfan/System.h"
#include <limits.h>
#include "cfan/Memory.h"

uint64_t cf_Timer_doEvent(cf_Timer *self) {
  uint64_t now = cf_System_nanoTicks();
  cf_TimerEvent *event, *temp;
  uint64_t nextTime, delta;
  bool moved;
  event = (cf_TimerEvent*)self->list.head;
  nextTime = INT64_MAX;
  while (event) {
      moved = false;
      if (event->time_ < now) {
        (*event->func)(event->arg);

        event->time_ = now + event->interval;

        if (event->repeat > 0) {
          --event->repeat;
          if (event->repeat == 0) {
            temp = event;
            event = (cf_TimerEvent*)event->super.next;
            cf_LinkedList_remove(&self->list, &temp->super);
            cf_free(temp);
            moved = true;
          }
        }
      }

      if (!moved) {
        delta = event->time_ - now;
        if (delta < nextTime) {
          nextTime = delta;
        }

        event = (cf_TimerEvent*)event->super.next;
      }
  }
  return nextTime;
}

static int cf_Timer_run(void *arg) {
  cf_Timer *self = (cf_Timer *)arg;
  int err;
  mtx_lock(&self->mutex);
  while (!self->cancel) {
    uint64_t waitTime = cf_Timer_doEvent(self);
    uint64_t seconds = waitTime/1E9;
    struct timespec t = { time(NULL) + seconds, waitTime-(seconds*1E9) };
    err = cnd_timedwait(&self->condition, &self->mutex, &t);
    CF_UNUSED(err);
  }
  mtx_unlock(&self->mutex);
  return 0;
}

cf_Error cf_Timer_make(cf_Timer *self) {
  int terr;
  cf_LinkedList_make(&self->list);

  self->cancel = false;

  terr = mtx_init(&self->mutex, mtx_plain);
  if (terr != thrd_success) {
    return cf_Error_error;
  }

  terr = cnd_init(&self->condition);
  if (terr != thrd_success) {
    mtx_destroy(&self->mutex);
    return cf_Error_error;
  }

  terr = thrd_create(&self->thread, cf_Timer_run, self);
  if (terr != thrd_success) {
    mtx_destroy(&self->mutex);
    cnd_destroy(&self->condition);
  }
  return cf_Error_ok;
}

void cf_Timer_add(cf_Timer *self, cf_TimerEvent *event) {
  uint64_t now = cf_System_nanoTicks();
  mtx_lock(&self->mutex);
  event->time_ = now + event->delay;
  cf_LinkedList_add(&self->list, &event->super);
  cnd_signal(&self->condition);
  mtx_unlock(&self->mutex);
}

void cf_Timer_dispose(cf_Timer *self) {
  mtx_lock(&self->mutex);
  self->cancel = true;
  cnd_signal(&self->condition);
  mtx_unlock(&self->mutex);
  thrd_join(self->thread, NULL);

  cf_LinkedList_freeLinkedElem(&self->list, NULL);
  mtx_destroy(&self->mutex);
  cnd_destroy(&self->condition);
}
