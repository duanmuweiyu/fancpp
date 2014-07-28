#include "cfan/LinkedQueue.h"

cf_Error cf_LinkedQueue_make(cf_LinkedQueue *self) {
  int terr;
  cf_LinkedQueue_Node *dummyHead = cf_new(cf_LinkedQueue_Node);
  dummyHead->pointer = NULL;
  dummyHead->next = NULL;
  self->head = self->tail = dummyHead;
  terr = mtx_init(&self->mutex, mtx_plain);
  if (terr != thrd_success) {
    return cf_Error_thread;
  }
  return cf_Error_ok;
}

cf_Error cf_LinkedQueue_dequeue(cf_LinkedQueue *self, void **out) {
  cf_LinkedQueue_Node *dummyHead;
  if (self->head == self->tail) {
    return cf_Error_null;
  }
  dummyHead = self->head;
  cf_assert(dummyHead->next);
  self->head = dummyHead->next;
  *out = self->head->pointer;
  self->head->pointer = NULL;
  cf_free(dummyHead);
  return cf_Error_ok;
}

void cf_LinkedQueue_enqueue(cf_LinkedQueue *self, void *ptr) {
  cf_LinkedQueue_Node *node = cf_new(cf_LinkedQueue_Node);
  node->pointer = ptr;
  node->next = NULL;

  if (self->tail->next == node->next) {
    self->tail->next = node;
    self->tail = node;
  }
}

void cf_LinkedQueue_dispose(cf_LinkedQueue *self) {
  cf_assert(self->head == self->tail);
  cf_free(self->head);
  mtx_destroy(&self->mutex);
}

cf_Error cf_LinkedQueue_safeDequeue(cf_LinkedQueue *self, void **out) {
  cf_Error err;
  mtx_lock(&self->mutex);
  err = cf_LinkedQueue_dequeue(self, out);
  mtx_unlock(&self->mutex);
  return err;
}

void cf_LinkedQueue_safeEnqueue(cf_LinkedQueue *self, void *ptr) {
  mtx_lock(&self->mutex);
  cf_LinkedQueue_enqueue(self, ptr);
  mtx_unlock(&self->mutex);
}
