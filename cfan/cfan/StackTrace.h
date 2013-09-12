/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CF_STACKTRACE_H_
#define _CF_STACKTRACE_H_

#include "cfan/Log.h"

CF_BEGIN

/**
 * print stack trace
 */
void cf_StackTrace_printTrace();

/**
 * get stack trace string
 */
char *cf_StackTrace_getTraceString();

CF_END

#endif
