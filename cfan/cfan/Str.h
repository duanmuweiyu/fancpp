/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-19  Jed Young  Creation
 */

#ifndef _CF_STR_H_
#define _CF_STR_H_

#include "cfan/macro.h"
#include "cfan/Error.h"
#include "cfan/Memory.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>

CF_BEGIN

#ifndef strcasecmp
extern int strcasecmp (__const char *__s1, __const char *__s2);
#endif

extern const char *cf_Str_empty;

/**
 * Return if this string contains the specified string.
 */
static inline bool cf_Str_contains(const char *self, const char *s) {
  return strstr(self, s) != NULL;
}

/**
 * Return the first occurance of the specified substring searching forward,
 * starting at the specified offset index.
 */
static inline long cf_Str_index(const char *self, const char *s) {
  const char *r = strstr(self, s);
  if (r == NULL) return -1;
  return r-self;
}

long cf_Str_lastIndex(const char *self, const char *s);

bool cf_Str_startsWith(const char *self, const char *s);

bool cf_Str_endsWith(const char *self, const char *s);

static inline bool cf_Str_equals(const char *self, const char *s) {
  return strcmp(self, s) == 0;
}

static inline bool cf_Str_equalsIgnoreCase(const char *self, const char *s) {
  return strcasecmp(self, s) == 0;
}

static inline bool cf_Str_isEmpty(const char *self) {
  return *self == '\0';
}

void cf_Str_replaceChar(char *self, char old, char new_);

void cf_Str_trimEnd(char *self);

void cf_Str_trimStart(char *self);

static inline void cf_Str_trim(char *self) {
  cf_Str_trimEnd(self);
  cf_Str_trimStart(self);
}

static inline char *cf_Str_dup(const char *self) {
  int n = strlen(self);
  char *s;
  s = (char *)cf_malloc(n+1);
  strcpy(s, self);
  return s;
}

static inline void cf_Str_free(char *self) {
  if (self == NULL) return;
  if (self != cf_Str_empty) {
    cf_free(self);
  }
}

/*========================================================================
 * Str Token
 */

typedef struct cf_StrToken_ {
  const char *delim;
  char *next;
  long delimLen;
} cf_StrToken;

static inline void cf_StrToken_make(cf_StrToken *self, char *src, const char *delim) {
  self->next = src;
  self->delim = delim;
  self->delimLen = strlen(delim);
}

char *cf_StrToken_next(cf_StrToken *self);


/*========================================================================
 * Uri
 */

//#ifdef CF_WIN
//  #define cf_StrUri_separatorChar '\\'
//#else
  #define cf_StrUri_separatorChar '/'
//#endif

void cf_StrUri_fromNative(char *self);
void cf_StrUri_toNative(char *self);

cf_Error cf_StrUri_getBaseName(const char *self, char *out, int bufferSize);

cf_Error cf_StrUri_getParentPath(const char *self, char *out, int bufferSize);

cf_Error cf_StrUri_getExtName(const char *self, char *out, int bufferSize);

CF_END

#endif

