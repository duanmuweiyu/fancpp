#include "cfan/Error.h"

#ifndef CF_WIN

#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#ifdef __IOS__
  #include <mach/mach.h>
  #include <mach/mach_time.h>
  #include <sys/_types/_timespec.h>
  #include <mach/mach.h>
  #include <mach/clock.h>

  uint64_t cf_System_nanoTicks(void) {
      clock_serv_t cclock;
      mach_timespec_t mts;

      host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
      clock_get_time(cclock, &mts);
      mach_port_deallocate(mach_task_self(), cclock);

      return (mts.tv_sec * 1E9) + mts.tv_nsec;
  }
#else
  #include <sys/timeb.h>

  uint64_t cf_System_nanoTicks(void) {
  //  return clock() / (CLOCKS_PER_SECOND * 1000);
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1E9) + ts.tv_nsec;
  }
#endif

uint64_t cf_System_currentTimeMillis() {
  struct timeb val;
  ftime(&val);
  return val.time * 1000 + val.millitm;
//  struct timeval tv={0};
//  struct timezone tz={0};
//  gettimeofday(&tv,&tz);
}

void cf_System_sleep(long millitm) {
  usleep(millitm * 1000);
}

cf_Error cf_System_getSelfPath(char *selfname) {
  const char *sysfile = "/proc/self/exe";
  int  namelen = 256;
  //memset(selfname, 0, 256);
  if ( -1 != readlink( sysfile, selfname, namelen) ) {
    return cf_Error_ok;
  }
  return cf_Error_error;
}

#else
/*========================================================================
 * Windows
 */

#include <Windows.h>

uint64_t cf_System_nanoTicks() {
  LARGE_INTEGER m_nBeginTime;
  LARGE_INTEGER m_nFreq;
  QueryPerformanceFrequency(&m_nFreq);
  QueryPerformanceCounter(&m_nBeginTime);
  return (m_nBeginTime.QuadPart*1E9)/m_nFreq.QuadPart;
}

uint64_t cf_System_currentTimeMillis() {
  SYSTEMTIME st;
  FILETIME ft,ft19700101;
  LARGE_INTEGER *pli,*pft,ms_from_19700101;

  st.wYear=1970;
  st.wMonth=1;
  st.wDay=1;
  st.wHour=0;
  st.wMinute=0;
  st.wSecond=0;
  st.wMilliseconds=0;
  SystemTimeToFileTime(&st,&ft19700101);
  pft=(LARGE_INTEGER *)&ft19700101;

  GetLocalTime(&st);
  SystemTimeToFileTime(&st,&ft);
  pli=(LARGE_INTEGER *)&ft;
  ms_from_19700101.QuadPart=(pli->QuadPart - pft->QuadPart)/10000;
  return ms_from_19700101.QuadPart;
}

void cf_System_sleep(long millitm) {
  Sleep(millitm);
}

cf_Error cf_System_getSelfPath(char *selfname) {
  TCHAR szFileName[MAX_PATH];
  GetModuleFileName(NULL,szFileName,MAX_PATH);
}

#endif
