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

#include "cfan/Object.h"
#include "cfan/Endian.h"
#include <stdio.h>

#ifdef CF_ENDIAN_LITTLE
  #define CF_ENDIAN_SWAP
#endif

CF_BEGIN

/*========================================================================
 * OutputStream
 */

typedef struct cf_OutputStream_ {
  cf_Object super;
} cf_OutputStream;

typedef struct cf_OutputStreamVTable_ {
  cf_VTable super;

  cf_Error (*write)(cf_OutputStream *self, char *buf, size_t size);

  cf_Error (*isError)(cf_OutputStream *self);

} cf_OutputStreamVTable;

static inline cf_Error cf_OutputStream_writeInt8(cf_OutputStream *self, int8_t out) {
  return CF_ICALL(cf_OutputStream, self, write, (char*)(&out), 1);
}
static inline cf_Error cf_OutputStream_writeInt16(cf_OutputStream *self, int16_t out) {
#ifdef CF_ENDIAN_SWAP
  cf_Endian_swap16p(out);
#endif
  cf_Error err = CF_ICALL(cf_OutputStream, self, write, (char*)(&out), 2);
  return err;
}
static inline cf_Error cf_OutputStream_writeInt32(cf_OutputStream *self, int32_t out) {
#ifdef CF_ENDIAN_SWAP
  cf_Endian_swap32p(out);
#endif
  return CF_ICALL(cf_OutputStream, self, write, (char*)(&out), 4);
}
static inline cf_Error cf_OutputStream_writeInt64(cf_OutputStream *self, int64_t out) {
#ifdef CF_ENDIAN_SWAP
  cf_Endian_swap64p(out);
#endif
  return CF_ICALL(cf_OutputStream, self, write, (char*)(&out), 8);
}

static inline cf_Error cf_OutputStream_writeUInt8(cf_OutputStream *self, uint8_t out) {
  return CF_ICALL(cf_OutputStream, self, write, (char*)(&out), 1);
}
static inline cf_Error cf_OutputStream_writeUInt16(cf_OutputStream *self, uint16_t out) {
#ifdef CF_ENDIAN_SWAP
  cf_Endian_swap16p(out);
#endif
  return CF_ICALL(cf_OutputStream, self, write, (char*)(&out), 2);
}
static inline cf_Error cf_OutputStream_writeUInt32(cf_OutputStream *self, uint32_t out) {
#ifdef CF_ENDIAN_SWAP
  cf_Endian_swap32p(out);
#endif
  return CF_ICALL(cf_OutputStream, self, write, (char*)(&out), 4);
}
static inline cf_Error cf_OutputStream_writeUInt64(cf_OutputStream *self, uint64_t out) {
#ifdef CF_ENDIAN_SWAP
  cf_Endian_swap64p(out);
#endif
  return CF_ICALL(cf_OutputStream, self, write, (char*)(&out), 8);
}

static inline cf_Error cf_OutputStream_writeFloat(cf_OutputStream *self, float out) {
  char *p = (char*)(&out);
#ifdef CF_ENDIAN_SWAP
  cf_Endian_swap32p(p);
#endif
  cf_assert(sizeof(float) == 4);
  return CF_ICALL(cf_OutputStream, self, write, p, 4);
}
static inline cf_Error cf_OutputStream_writeDouble(cf_OutputStream *self, double out) {
  char *p = (char*)(&out);
#ifdef CF_ENDIAN_SWAP
  cf_Endian_swap64p(p);
#endif
  cf_assert(sizeof(double) == 8);
  return CF_ICALL(cf_OutputStream, self, write, p, 8);
}

/*========================================================================
 * InputStream
 */

typedef struct cf_InputStream_ {
  cf_Object super;
} cf_InputStream;

typedef struct cf_InputStreamVTable_ {
  cf_VTable super;

  cf_Error (*read)(cf_InputStream *self, char *buf, size_t size);

  cf_Error (*isError)(cf_InputStream *self);

} cf_InputStreamVTable;

static inline cf_Error cf_InputStream_readInt8(cf_OutputStream *self, int8_t *out) {
  return CF_ICALL(cf_InputStream, self, read, (char *)out, 1);
}
static inline cf_Error cf_InputStream_readInt16(cf_OutputStream *self, int16_t *out) {
  cf_Error err = CF_ICALL(cf_InputStream, self, read, (char *)out, 2);
#ifdef CF_ENDIAN_SWAP
  cf_Endian_swap16p(out);
#endif
  return err;
}
static inline cf_Error cf_InputStream_readInt32(cf_OutputStream *self, int32_t *out) {
  cf_Error err = CF_ICALL(cf_InputStream, self, read, (char *)out, 4);
#ifdef CF_ENDIAN_SWAP
  cf_Endian_swap32p(out);
#endif
  return err;
}
static inline cf_Error cf_InputStream_readInt64(cf_OutputStream *self, int64_t *out) {
  cf_Error err = CF_ICALL(cf_InputStream, self, read, (char *)out, 8);
#ifdef CF_ENDIAN_SWAP
  cf_Endian_swap64p(out);
#endif
  return err;
}

static inline cf_Error cf_InputStream_readUInt8(cf_OutputStream *self, uint8_t *out) {
  return CF_ICALL(cf_InputStream, self, read, (char *)out, 1);
}
static inline cf_Error cf_InputStream_readUInt16(cf_OutputStream *self, uint16_t *out) {
  cf_Error err = CF_ICALL(cf_InputStream, self, read, (char *)out, 2);
#ifdef CF_ENDIAN_SWAP
  cf_Endian_swap16p(out);
#endif
  return err;
}
static inline cf_Error cf_InputStream_readUInt32(cf_OutputStream *self, uint32_t *out) {
  cf_Error err = CF_ICALL(cf_InputStream, self, read, (char *)out, 4);
#ifdef CF_ENDIAN_SWAP
  cf_Endian_swap32p(out);
#endif
  return err;
}
static inline cf_Error cf_InputStream_readUInt64(cf_OutputStream *self, uint64_t *out) {
  cf_Error err = CF_ICALL(cf_InputStream, self, read, (char *)out, 8);
#ifdef CF_ENDIAN_SWAP
  cf_Endian_swap64p(out);
#endif
  return err;
}

static inline cf_Error cf_InputStream_readFloat(cf_OutputStream *self, float *out) {
  cf_Error err = CF_ICALL(cf_InputStream, self, read, (char *)out, 4);
#ifdef CF_ENDIAN_SWAP
  cf_Endian_swap32p(out);
#endif
  cf_assert(sizeof(float) == 4);
  return err;
}
static inline cf_Error cf_InputStream_readDouble(cf_OutputStream *self, double *out) {
  cf_Error err = CF_ICALL(cf_InputStream, self, read, (char *)out, 8);
#ifdef CF_ENDIAN_SWAP
  cf_Endian_swap64p(out);
#endif
  cf_assert(sizeof(double) == 8);
  return err;
}

CF_END

#endif
