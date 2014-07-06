#include "cppfan/MemStream.h"

CF_USING_NAMESPACE

cf_Error MemStream::read(char *out, int n) {
  int size = data.size();
  if (n > size) return cf_Error_overflow;
  memcpy(out, data.getPointer() + size, n);
  data._setSize(size-n);
  return cf_Error_ok;
}

cf_Error MemStream::write(char *m, int n) {
  int size = data.size();
  bool ok = data.reserver(size+n);
  memcpy(data.getPointer()+size, m, n);
  return ok ? cf_Error_ok : cf_Error_error;
}
