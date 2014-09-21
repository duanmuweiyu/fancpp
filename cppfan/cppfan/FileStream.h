#ifndef FILESTREAM_H
#define FILESTREAM_H

#include "cppfan/Stream.h"
#include <stdio.h>

CF_BEGIN_NAMESPACE

class FileStream : public Stream {
  FILE *file;
public:
  FileStream(FILE *file) : file(file) {
    cf_assert(file);
  }

  virtual int read(char *out, int n) {
    int c = fread(out, 1, n, file);
    return c;
  }

  virtual cf_Error write(char *m, int n) {
    int c = fwrite(m, n, 1, file);
    return c == 1 ? cf_Error_ok : cf_Error_error;
  }

  virtual void flush() {
    fflush(file);
  }

  bool close() {
    int c = fclose(file);
    return c == 0;
  }

  bool seek(int64_t pos) {
    //const fpos_t p = pos;
    int c = fseek(file, (long)pos, SEEK_SET);
    return c == 0;
  }

  int64_t position() {
    //fpos_t p;
    //fgetpos(file, &p);
    long p = ftell(file);
    return p;
  }

  bool toEnd() {
    int c = fseek(file, 0, SEEK_END);
    return c == 0;
  }

  void rewind() {
    ::rewind(file);
  }

  bool peek(int *c) {
    *c = fgetc(file);
    int nc = ungetc(*c, file);
    return (*c == nc && nc != EOF);
  }

  bool getError() {
    if (ferror(file)) {
      return false;
    }
    return true;
  }
};


CF_END_NAMESPACE

#endif // FILESTREAM_H
