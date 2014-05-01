
#include "cfan/Object.h"
#include "cfan/Memory.h"

cf_Object *cf_Object_release(cf_Object *self) {
  cf_assert(self->refCount >= 0);
  if (--self->refCount < 0) {
    if (self->vtable && self->vtable->destructor) {
      self->vtable->destructor(self);
    }
    cf_free(self);
    return NULL;
  }
  return self;
}
