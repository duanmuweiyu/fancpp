#include "cppfan/String.h"

CF_USING_NAMESPACE

bool String::equals(const Object& r) const {
  if (this->hashCode() != r.hashCode()) return false;

  const String &s = (const String &)r;
  return strcmp(str(), s.str()) == 0;
}

long String::hashCode() const {
  if (_hashCode == 0) {
    _hashCode = cf_HashMap_strHash(str());
    if (_hashCode == 0) {
      _hashCode = -1;
    }
  }
  return _hashCode;
}

String &String::addStr(const char *s) {
  int len = strlen(s);
  addCapacity(len);
  strcpy(str()+size(), s);
  _hashCode = 0;
  return *this;
}
