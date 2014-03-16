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

#include "cfan/Error.h"

CF_BEGIN

/**
 * Virtual table is a function pointer list for Object.
 * We using this implements polymorphism.
 */
typedef struct cf_VTable_ {
  int offset;
} cf_VTable;

/**
 * Object is a super class of all objects.
 */
typedef struct cf_Object_ {
  cf_VTable *vtable;
  int refCount;
} cf_Object;

#define cf_Object_addRef(self) (((cf_Object*)(self))->refCount++)
cf_Object *cf_Object_release(cf_Object *self);

/**
 * get vtable
 */
#define CF_VTABLE(type, obj) ((type##VTable*)((cf_Object*)(obj))->vtable)

/**
 * interface call.
 * the Interface have a offset at the object model.
 */
#define CF_ICALL(type, obj, func, ...) (CF_VTABLE(type, obj)->func(\
   (type*)(((char*)(obj))+((cf_Object*)(obj))->vtable->offset), ## __VA_ARGS__))

/**
 * virtual call
 */
#define CF_CALL(type, obj, func, ...) (CF_VTABLE(type, obj)->func((type*)obj, ## __VA_ARGS__))

CF_END

#endif
