#include "cppfan/MemStream.h"

CF_USING_NAMESPACE

int MemStream::read(char *out, size_t n) {
  int size = data.size();
  unsigned int remains = data._getCapacity() - size;
  if (remains <= 0) return -1;
  if (n > remains) n = remains;

  memcpy(out, data.getPointer() + size, n);
  data._setSize(size+n);
  return n;
}

cf_Error MemStream::write(char *m, size_t n) {
  int size = data.size();
  bool ok = data.reserver(size+n);
  memcpy(data.getPointer()+size, m, n);
  data._setSize(size + n);
  return ok ? cf_Error_ok : cf_Error_error;
}
