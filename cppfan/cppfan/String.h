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
  String(const char *cstr) {
    data = (char*)(cstr);
    length = strlen(cstr);
  }

  int size() const { return length; }

  const char *str() const { return ((String*)this)->data.getRawPtr(); }
  char *str() { return data.getRawPtr(); }

  int toInt() const { return atoi(str()); }

  double toDouble() const { return atof(str()); }
};

bool operator== (const String& l, const String& r) {
  return strcmp(l.str(), r.str()) == 0;
}

String operator+ (const String& l, const String& r) {
  int len = l.size() + r.size();
  char *d = (char*)malloc(len+1);
  strcpy(d, l.str());
  strcat(d, r.str());
  return d;
}

CF_END_NAMESPACE
#endif // STRING_H
