/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-6  Jed Young  Creation
 */

#ifndef _CF_INPUTSTREAM_H_
#define _CF_INPUTSTREAM_H_

#include "Object.h"
#include <stdio.h>

CF_BEGIN

enum cf_Endian {
  cf_Endian_big,
  cf_Endian_little
};

/*************************************************************************
 * OutputStream
 */

typedef struct cf_OutputStream_ {
  cf_Object super;
} cf_OutputStream;

typedef struct cf_OutputStreamVTable_ {
  cf_VTable super;

  cf_Error (*write)(cf_OutputStream *self, char *buf, size_t size);

} cf_OutputStreamVTable;

inline cf_Error writeInt8(cf_OutputStream *self, int8_t out) {
  return CF_ICALL(cf_OutputStream, self, write, (char*)(&out), 1);
}
inline cf_Error writeInt16(cf_OutputStream *self, int16_t out) {
  return CF_ICALL(cf_OutputStream, self, write, (char*)(&out), 2);
}
inline cf_Error writeInt32(cf_OutputStream *self, int32_t out) {
  return CF_ICALL(cf_OutputStream, self, write, (char*)(&out), 4);
}
inline cf_Error writeInt64(cf_OutputStream *self, int64_t out) {
  return CF_ICALL(cf_OutputStream, self, write, (char*)(&out), 8);
}

inline cf_Error writeUInt8(cf_OutputStream *self, uint8_t out) {
  return CF_ICALL(cf_OutputStream, self, write, (char*)(&out), 1);
}
inline cf_Error writeUInt16(cf_OutputStream *self, uint16_t out) {
  return CF_ICALL(cf_OutputStream, self, write, (char*)(&out), 2);
}
inline cf_Error writeUInt32(cf_OutputStream *self, uint32_t out) {
  return CF_ICALL(cf_OutputStream, self, write, (char*)(&out), 4);
}
inline cf_Error writeUInt64(cf_OutputStream *self, uint64_t out) {
  return CF_ICALL(cf_OutputStream, self, write, (char*)(&out), 8);
}

inline cf_Error writeFloat(cf_OutputStream *self, float out) {
  return CF_ICALL(cf_OutputStream, self, write, (char*)(&out), 4);
}
inline cf_Error writeDouble(cf_OutputStream *self, double out) {
  return CF_ICALL(cf_OutputStream, self, write, (char*)(&out), 8);
}

/*************************************************************************
 * InputStream
 */

typedef struct cf_InputStream_ {
  cf_Object super;
} cf_InputStream;

typedef struct cf_InputStreamVTable_ {
  cf_VTable super;

  cf_Error (*read)(cf_InputStream *self, char *buf, size_t size);

} cf_InputStreamVTable;

inline cf_Error readInt8(cf_OutputStream *self, int8_t *out) {
  return CF_ICALL(cf_InputStream, self, read, (char *)out, 1);
}
inline cf_Error readInt16(cf_OutputStream *self, int16_t *out) {
  return CF_ICALL(cf_InputStream, self, read, (char *)out, 2);
}
inline cf_Error readInt32(cf_OutputStream *self, int32_t *out) {
  return CF_ICALL(cf_InputStream, self, read, (char *)out, 4);
}
inline cf_Error readInt64(cf_OutputStream *self, int64_t *out) {
  return CF_ICALL(cf_InputStream, self, read, (char *)out, 8);
}

inline cf_Error readUInt8(cf_OutputStream *self, uint8_t *out) {
  return CF_ICALL(cf_InputStream, self, read, (char *)out, 1);
}
inline cf_Error readUInt16(cf_OutputStream *self, uint16_t *out) {
  return CF_ICALL(cf_InputStream, self, read, (char *)out, 2);
}
inline cf_Error readUInt32(cf_OutputStream *self, uint32_t *out) {
  return CF_ICALL(cf_InputStream, self, read, (char *)out, 4);
}
inline cf_Error readUInt64(cf_OutputStream *self, uint64_t *out) {
  return CF_ICALL(cf_InputStream, self, read, (char *)out, 8);
}

inline cf_Error readFloat(cf_OutputStream *self, float *out) {
  return CF_ICALL(cf_InputStream, self, read, (char *)out, 4);
}
inline cf_Error readDouble(cf_OutputStream *self, double *out) {
  return CF_ICALL(cf_InputStream, self, read, (char *)out, 8);
}

CF_END

#endif