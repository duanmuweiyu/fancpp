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

#include "macro.h"

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
 * logging statements
 *
 */
typedef struct cf_LogRec_ {
  const char *tag;
  const char *file;
  const char *func;
  const unsigned int line;
  const cf_LogLevel level;
  const char *msg;
} cf_LogRec;

/**
 * do log
 *
 */
void cf_Log_doLog(const char *tag, const char *file, const char *func, const unsigned int line
  , const cf_LogLevel level, const char *msg, ...);

/*************************************************************************
 * sys logging macro
 */

#define cf_Log_log(tag, level, msg, ...)\
  cf_Log_doLog(tag, __FILE__, __func__, __LINE__,  level, msg, ## __VA_ARGS__);

#define cf_Log_tag "cf"
#define cf_Log_cfError(msg, ...) cf_Log_log(cf_Log_tag, cf_LogLevel_err, msg, ## __VA_ARGS__)
#define cf_Log_cfWarn(msg, ...)  cf_Log_log(cf_Log_tag, cf_LogLevel_warn, msg, ## __VA_ARGS__)
#define cf_Log_cfInfo(msg, ...)  cf_Log_log(cf_Log_tag, cf_LogLevel_info, msg, ## __VA_ARGS__)
#ifdef _DEBUG
  #define cf_Log_cfDebug(msg, ...) cf_Log_log(cf_Log_tag, cf_LogLevel_debug, msg, ## __VA_ARGS__)
#else
  #define cf_Log_cfDebug
#endif


CF_END

#endif

