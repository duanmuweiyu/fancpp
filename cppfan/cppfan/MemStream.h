#ifndef MEMSTREAM_H
#define MEMSTREAM_H

#include "cppfan/Stream.h"
#include "cppfan/Array.h"

CF_BEGIN_NAMESPACE

class MemStream : public Stream {
  Array<char> data;
public:
  MemStream(int size) : data(size) {}

  virtual int read(char *out, size_t n);

  virtual cf_Error write(char *m, size_t n);

  virtual void flush() {
    //do nothing
  }

  bool seek(int64_t pos) {
    data.resize(pos);
    return true;
  }

  int64_t position() {
    return data.size();
  }

  bool toEnd() {
    data._setSize(data._getCapacity());
    return true;
  }

  void rewind() {
    data._setSize(0);
  }
};

CF_END_NAMESPACE

#endif // MEMSTREAM_H
