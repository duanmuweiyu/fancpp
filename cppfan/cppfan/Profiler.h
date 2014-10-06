#ifndef PERFORPROFILE_H
#define PERFORPROFILE_H

#include "cppfan/Object.h"

CF_BEGIN_NAMESPACE

class Counter {
public:
  double min;
  double max;
  double average;
  unsigned long count;
  void add(double val) {
    if (min > val) {
      min = val;
    }
    if (max < val) {
      max = val;
    }
    double sum = average * count + val;
    ++count;
    average = sum / count;
  }
};

class PerforProfiler
{
  uint64_t begin;
  uint64_t tagPoint;
public:
  PerforProfiler() {
    start();
  }

  void start() {
    begin = cf_System_nanoTicks();
    tagPoint = begin;
  }

  /**
   * print and return elapsedTime
   */
  double tag(const char *name) {
    uint64_t now = cf_System_nanoTicks();
    double elapsedTime = ((now-tagPoint)/1E6f);
    cf_Log_debug("cf.proferProfile", "%s const: %.2fms, from start: %.2fms"
                 , name, elapsedTime, ((now-begin)/1E6f));
    tagPoint = cf_System_nanoTicks();
    return elapsedTime;
  }
};

CF_END_NAMESPACE
#endif // PERFORPROFILE_H
