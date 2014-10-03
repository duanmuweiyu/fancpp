#include "cppfan/Stream.h"

CF_USING_NAMESPACE

cf_Error Stream::writeTo(Stream *out, int *succSize) {
  int size = 0;
  int all = 0;
  char buffer[1024];
  cf_Error err;

  while (true) {
    size = this->read(buffer, 1024);
    if (size <= 0) {
      break;
    }
    err = out->write(buffer, size);
    if (err != cf_Error_ok) {
      return err;
    }
    all += size;
  }

  if (succSize) {
    *succSize = all;
  }
  return cf_Error_ok;
}
