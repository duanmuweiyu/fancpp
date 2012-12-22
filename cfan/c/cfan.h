/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CF_CFAN_H_
#define _CF_CFAN_H_

#include <stddef.h>

#include "macro.h"
#include "type.h"
#include "miss.h"
#include "math.h"
#include "error.h"
#include "memory.h"
#include "Log.h"
#include "Array.h"

typedef union _cf_Value {
  void *pointer;
  char charV;
  short shortV;
  int intV;
  long longV;
  long long longlongV;
  float floatV;
  double doubleV;
} cf_Value;


#endif