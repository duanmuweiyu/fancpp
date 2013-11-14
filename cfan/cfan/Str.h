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

/**
 * Return if this string contains the specified string.
 */
static inline bool cf_StrBuf_contains(const char *self, const char *s) {
  return strstr(self, s) != NULL;
}

/**
 * Return the first occurance of the specified substring searching forward,
 * starting at the specified offset index.
 */
static inline long cf_StrBuf_index(const char *self, const char *s) {
  const char *r = strstr(self, s);
  if (r == NULL) return -1;
  return r-self;
}

static inline long cf_StrBuf_lastIndex(const char *self, const char *s) {
  const char *pos;
  int len = strlen(self);
  int n = strlen(s);
  int i;

  //TODO optimize this
  for (i = len-1; i != -1; --i)
  {
    if (self[i] == *s) {
      pos = self + i;
      if (memcmp(pos, s, n) == 0) {
        return i;
      }
    }
  }
  return -1;
}

static inline bool cf_Str_startsWith(const char *self, const char *s) {
  int i;
  if (*s == '\0') return true;
  for (i = 0; self[i] == s[i] && s[i] != '\0'; ++i ) {
  }

  if (s[i] == '\0') {
    return true;
  } else if (self[i] == '\0') {
    return false;
  }
  return false;
}

static inline bool cf_Str_endsWith(const char *self, const char *s) {
  int i = strlen(self)-1;
  int j = strlen(s)-1;
  for (; i != -1; --i, --j ) {
    if (j == -1) {
      return true;
    }
    if (self[i] != s[j]) {
      return false;
    }
  }
  return false;
}

static inline bool cf_Str_equals(const char *self, const char *s) {
  return strcmp(self, s) == 0;
}

static inline bool cf_Str_equalsIgnoreCase(const char *self, const char *s) {
  return strcasecmp(self, s) == 0;
}

static inline bool cf_Str_isEmpty(const char *self) {
  return *self == '\0';
}

static inline void cf_Str_replaceChar(char *self, char old, char new_) {
  for (; *self != '\0'; ++self) {
    if (*self == old) {
      *self = new_;
    }
  }
}

static inline void cf_Str_trimEnd(char *self) {
  int i = strlen(self)-1;
  if (!isspace(self[i])) {
    return;
  }
  for (; i != -1; --i) {
    if (!isspace(self[i])) {
      self[i+1] = '\0';
      return;
    }
  }
}

static inline void cf_Str_trimStart(char *self) {
  char *pos = self;
  while (isspace(*pos)) {
    ++pos;
  }
  if (pos == self) return;
  memmove(self, pos, strlen(pos)+1);
}

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

static inline char *cf_StrToken_next(cf_StrToken *self) {
  char *current = self->next;
  char *last;
  if (current == NULL) return NULL;
  last = strstr(self->next, self->delim);
  if (last == NULL) {
    self->next = NULL;
    return current;
  }
  self->next = last + self->delimLen;
  *last = '\0';
  return current;
}

CF_END

#endif

