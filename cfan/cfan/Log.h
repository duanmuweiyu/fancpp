/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#ifndef _CF_LOG_H_
#define _CF_LOG_H_

#include "cfan/macro.h"
#include "cfan/miss.h"

#include <stdarg.h>

CF_BEGIN

/**
 * logging level
 *
 */
typedef enum cf_LogLevel_ {
  cf_LogLevel_debug,
  cf_LogLevel_info,
  cf_LogLevel_warn,
  cf_LogLevel_err,
  cf_LogLevel_silent
} cf_LogLevel;

/**
 * Log listener function type.
 */
typedef void (*cf_Log_listener)(const char *tag, const char *file, const char *func, const unsigned int line
                          , const cf_LogLevel level, const char *msg, va_list args);

/**
 * Add listener. if success return id, if fail return -1.
 */
int cf_Log_addListener(cf_Log_listener listener);

/**
 * remove listener by id. if success return old func, else return NULL.
 */
cf_Log_listener cf_Log_removeListener(int id);

/**
 * do log
 *
 */
void cf_Log_doLog(const char *tag, const char *file, const char *func, const unsigned int line
  , const cf_LogLevel level, const char *msg, ...);

/*========================================================================
 * sys logging macro
 */

#define cf_Log_log(tag, level, msg, ...)\
  cf_Log_doLog(tag, __FILE__, __func__, __LINE__,  level, msg, ## __VA_ARGS__)

/**
 * convenience log macro.
 *
 */
#define cf_Log_error(tag, msg, ...) cf_Log_log(tag, cf_LogLevel_err, msg, ## __VA_ARGS__)
#define cf_Log_warn(tag, msg, ...)  cf_Log_log(tag, cf_LogLevel_warn, msg, ## __VA_ARGS__)
#define cf_Log_info(tag, msg, ...)  cf_Log_log(tag, cf_LogLevel_info, msg, ## __VA_ARGS__)

#ifdef CF_DEBUG
  #define cf_Log_debug(tag, msg, ...) cf_Log_log(tag, cf_LogLevel_debug, msg, ## __VA_ARGS__)
#else
  #define cf_Log_debug(tag, msg, ...)
#endif

#define cf_Log_tag "cf"


CF_END

#endif

