#ifndef _CPPF_STRING_H_
#define _CPPF_STRING_H_

#include "cppfan/Object.h"
#include "cppfan/ObjectRef.h"
#include "cppfan/Array.h"
#include <string.h>
#include <stdlib.h>

CF_BEGIN_NAMESPACE

class String : public Array<char> {
public:
  String() : Array<char>(16) {
    str()[0] = 0;
  }

  String(const char *cstr) {
    int len = strlen(cstr);
    reserver(len+1);
    strcpy(str(), cstr);
    _setSize(len);
  }

  //int size() const { return data.size(); }
  bool isEmpty() const { return size() == 0; }

  const char *str() const { return ((String*)this)->str(); }
  char *str() { return getPointer(); }

  int toInt() const { return atoi(str()); }

  double toDouble() const { return atof(str()); }

  bool equals(const String& r) const { return strcmp(str(), r.str()) == 0; }

  String &addStr(const char *s) {
    int len = strlen(s);
    addCapacity(len);
    strcpy(str()+size(), s);
    return *this;
  }
};

class StringRef : public ObjectRef<String> {
public:
  StringRef() : ObjectRef<String>(new String()){}
  StringRef(const char *cstr) : ObjectRef<String>(new String(cstr)) {
  }

  StringRef &operator+=(const char *s) {
    getRawPtr()->addStr(s);
    return *this;
  }
};

static inline bool operator== (StringRef& l, StringRef& r) {
  return l->equals(*r.getRawPtr());
}

CF_END_NAMESPACE
#endif // STRING_H
