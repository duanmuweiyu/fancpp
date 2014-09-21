#ifndef STREAM_H
#define STREAM_H

#include "cppfan/common.h"

CF_BEGIN_NAMESPACE

class Stream {
public:
  virtual cf_Error write(char *buf, size_t size) = 0;

  /**
   * @return the total number of bytes read into the buffer,
   * or -1 if there is no more data because the end of the stream has been reached.
   */
  virtual int read(char *buf, size_t size) = 0;
  virtual void flush() = 0;

  /*========================================================================
   * OutputStream
   */

  inline cf_Error writeInt8(int8_t out) {
    return write((char*)(&out), 1);
  }
  inline cf_Error writeInt16(int16_t out) {
  #ifdef CF_ENDIAN_SWAP
    cf_Endian_swap16p(out);
  #endif
    cf_Error err = write((char*)(&out), 2);
    return err;
  }
  inline cf_Error writeInt32(int32_t out) {
  #ifdef CF_ENDIAN_SWAP
    cf_Endian_swap32p(out);
  #endif
    return write((char*)(&out), 4);
  }
  inline cf_Error writeInt64(int64_t out) {
  #ifdef CF_ENDIAN_SWAP
    cf_Endian_swap64p(out);
  #endif
    return write((char*)(&out), 8);
  }

  inline cf_Error writeUInt8(uint8_t out) {
    return write((char*)(&out), 1);
  }
  inline cf_Error writeUInt16(uint16_t out) {
  #ifdef CF_ENDIAN_SWAP
    cf_Endian_swap16p(out);
  #endif
    return write((char*)(&out), 2);
  }
  inline cf_Error writeUInt32(uint32_t out) {
  #ifdef CF_ENDIAN_SWAP
    cf_Endian_swap32p(out);
  #endif
    return write((char*)(&out), 4);
  }
  inline cf_Error writeUInt64(uint64_t out) {
  #ifdef CF_ENDIAN_SWAP
    cf_Endian_swap64p(out);
  #endif
    return write((char*)(&out), 8);
  }

  inline cf_Error writeFloat(float out) {
    char *p = (char*)(&out);
  #ifdef CF_ENDIAN_SWAP
    cf_Endian_swap32p(p);
  #endif
    cf_assert(sizeof(float) == 4);
    return write(p, 4);
  }
  inline cf_Error writeDouble(double out) {
    char *p = (char*)(&out);
  #ifdef CF_ENDIAN_SWAP
    cf_Endian_swap64p(p);
  #endif
    cf_assert(sizeof(double) == 8);
    return write(p, 8);
  }


  /*========================================================================
   * InputStream
   */

  inline cf_Error readInt8(int8_t *out) {
    return read((char *)out, 1) == 1 ? cf_Error_ok : cf_Error_io;
  }
  inline cf_Error readInt16(int16_t *out) {
    cf_Error err = read((char *)out, 2) == 2 ? cf_Error_ok : cf_Error_io;;
  #ifdef CF_ENDIAN_SWAP
    cf_Endian_swap16p(out);
  #endif
    return err;
  }
  inline cf_Error readInt32(int32_t *out) {
    cf_Error err = read((char *)out, 4) == 4 ? cf_Error_ok : cf_Error_io;;
  #ifdef CF_ENDIAN_SWAP
    cf_Endian_swap32p(out);
  #endif
    return err;
  }
  inline cf_Error readInt64(int64_t *out) {
    cf_Error err = read((char *)out, 8) == 8 ? cf_Error_ok : cf_Error_io;;
  #ifdef CF_ENDIAN_SWAP
    cf_Endian_swap64p(out);
  #endif
    return err;
  }

  inline cf_Error readUInt8(uint8_t *out) {
    return read((char *)out, 1) == 1 ? cf_Error_ok : cf_Error_io;;
  }
  inline cf_Error readUInt16(uint16_t *out) {
    cf_Error err = read((char *)out, 2) == 2 ? cf_Error_ok : cf_Error_io;;
  #ifdef CF_ENDIAN_SWAP
    cf_Endian_swap16p(out);
  #endif
    return err;
  }
  inline cf_Error readUInt32(uint32_t *out) {
    cf_Error err = read((char *)out, 4) == 4 ? cf_Error_ok : cf_Error_io;;
  #ifdef CF_ENDIAN_SWAP
    cf_Endian_swap32p(out);
  #endif
    return err;
  }
  inline cf_Error readUInt64(uint64_t *out) {
    cf_Error err = read((char *)out, 8) == 8 ? cf_Error_ok : cf_Error_io;;
  #ifdef CF_ENDIAN_SWAP
    cf_Endian_swap64p(out);
  #endif
    return err;
  }

  inline cf_Error readFloat(float *out) {
    cf_Error err = read((char *)out, 4) == 4 ? cf_Error_ok : cf_Error_io;;
  #ifdef CF_ENDIAN_SWAP
    cf_Endian_swap32p(out);
  #endif
    cf_assert(sizeof(float) == 4);
    return err;
  }
  inline cf_Error readDouble(double *out) {
    cf_Error err = read((char *)out, 8) == 8 ? cf_Error_ok : cf_Error_io;;
  #ifdef CF_ENDIAN_SWAP
    cf_Endian_swap64p(out);
  #endif
    cf_assert(sizeof(double) == 8);
    return err;
  }
};

CF_END_NAMESPACE

#endif // STREAM_H
