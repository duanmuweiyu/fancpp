/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-6  Jed Young  Creation
 */

#ifndef _CF_OBJECT_H_
#define _CF_OBJECT_H_

#include "Error.h"

CF_BEGIN

typedef struct cf_VTable_ {
  int offset;

} cf_VTable;

typedef struct cf_Object_ {
  cf_VTable *vtable;
} cf_Object;

/**
 * get vtable
 */
#define CF_VTABLE(type, obj) ((type##VTable*)((cf_Object*)(obj))->vtable)

/**
 * interface call
 */
#define CF_ICALL(type, obj, func, ...) (CF_VTABLE(type, obj)->func(\
   (type*)(((char*)(obj))+((cf_Object*)(obj))->vtable->offset), ## __VA_ARGS__))

/**
 * virtual call
 */
#define CF_CALL(type, obj, func, ...) (CF_VTABLE(type, obj)->func((type*)obj, ## __VA_ARGS__))

CF_END

#endif