/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-9-14  Jed Young  Creation
 */

#ifndef _CF_FILELOGLISTENER_H_
#define _CF_FILELOGLISTENER_H_

#include "cfan/Error.h"
#include "cfan/Log.h"
#include "cfan/HashMap.h"

CF_BEGIN

typedef struct cf_FileLogListener_ {
  cf_Log_Listener super;
  cf_HashMapSP map;
  const char *path;
} cf_FileLogListener;

void cf_FileLogListener_make(cf_FileLogListener *self, const char *path);

void cf_FileLogListener_dispose(cf_FileLogListener *self);

CF_END

#endif
