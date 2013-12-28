#ifndef _CF_SYSTEM_H_
#define _CF_SYSTEM_H_

#include "cfan/System.h"
#include "cfan/Error.h"

CF_BEGIN

/**
 * return the milliseconds from midnight, January 1, 1970 UTC.
 */
uint64_t cf_System_currentTimeMillis();

/**
 * returns a relative time.
 * Typically it is the number of nanosecond ticks which have elapsed since system startup.
 */
uint64_t cf_System_nanoTicks();

/**
 * sleep current thread millitm second
 */
void cf_System_sleep(long millitm);

/**
 * get current execable file path
 */
cf_Error cf_System_getSelfPath(char *selfname);

unsigned long cf_System_currentThreadId() {
#ifdef CF_WIN
  unsigned long GetCurrentThreadId(void);
  return GetCurrentThreadId();
#else
  unsigned long pthread_self(void);
  return pthread_self();
#endif
}

CF_END

#endif // _CF_SYSTEM_H_
