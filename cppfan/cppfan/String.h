#ifndef _CPPF_STRING_H_
#define _CPPF_STRING_H_

#include "cppfan/Object.h"
#include "cppfan/AutoPtr.h"
#include <string.h>
#include <stdlib.h>

CF_BEGIN_NAMESPACE

class String {
  AutoPtr<char> data;
  int length;
public:
  String() : length(0) {
  }

  String(const char *cstr) {
    length = strlen(cstr);
    char *s = (char*)cf_malloc(length+1);
    strcpy(s, cstr);
    data.setRawPtr(s);
    data.isDelete = false;
  }

  int size() const { return length; }
  bool isEmpty() const { return length == 0; }

  const char *str() const { return ((String*)this)->data.getRawPtr(); }
  char *str() { return data.getRawPtr(); }

  int toInt() const { return atoi(str()); }

  double toDouble() const { return atof(str()); }
};

static inline bool operator== (const String& l, const String& r) {
  return strcmp(l.str(), r.str()) == 0;
}


CF_END_NAMESPACE
#endif // STRING_H
