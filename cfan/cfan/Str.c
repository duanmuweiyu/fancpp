
#include "cfan/Str.h"


long cf_Str_lastIndex(const char *self, const char *s) {
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

bool cf_Str_startsWith(const char *self, const char *s) {
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

bool cf_Str_endsWith(const char *self, const char *s) {
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


void cf_Str_replaceChar(char *self, char old, char new_) {
  for (; *self != '\0'; ++self) {
    if (*self == old) {
      *self = new_;
    }
  }
}

void cf_Str_trimEnd(char *self) {
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

void cf_Str_trimStart(char *self) {
  char *pos = self;
  while (isspace(*pos)) {
    ++pos;
  }
  if (pos == self) return;
  memmove(self, pos, strlen(pos)+1);
}


char *cf_StrToken_next(cf_StrToken *self) {
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
