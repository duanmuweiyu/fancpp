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

CF_BEGIN

enum cf_Endian {
  cf_Endian_big;
  cf_Endian_little;
};

/*************************************************************************
 * OutputStream
 */

typedef struct cf_OutputStreamVTable_ {
  cf_VTable super;

  cf_Error (*write)(char *buf, size_t size);

  cf_Error (*writeInt8)(int8_t out);
  cf_Error (*writeInt16)(int8_t out);
  cf_Error (*writeInt32)(int8_t out);
  cf_Error (*writeInt64)(int8_t out);

  cf_Error (*writeUInt8)(uint8_t out);
  cf_Error (*writeUInt16)(uint8_t out);
  cf_Error (*writeUInt32)(uint8_t out);
  cf_Error (*writeUInt64)(uint8_t out);

  cf_Error (*writeFloat)(float out);
  cf_Error (*writeDouble)(double out);

} cf_OutputStreamVTable;

typedef struct cf_OutputStream_ {
  cf_Object super;
} cf_OutputStream;

/*************************************************************************
 * InputStream
 */
typedef struct cf_InputStreamVTable_ {
  cf_VTable super;

  cf_Error (*read)(char *buf, size_t size);

  cf_Error (*readInt8)(int8_t *out);
  cf_Error (*readInt16)(int8_t *out);
  cf_Error (*readInt32)(int8_t *out);
  cf_Error (*readInt64)(int8_t *out);

  cf_Error (*readUInt8)(uint8_t *out);
  cf_Error (*readUInt16)(uint8_t *out);
  cf_Error (*readUInt32)(uint8_t *out);
  cf_Error (*readUInt64)(uint8_t *out);

  cf_Error (*readFloat)(float *out);
  cf_Error (*readDouble)(double *out);

} cf_InputStreamVTable;

typedef struct cf_InputStream_ {
  cf_Object super;
} cf_InputStream;


CF_END

#endif