#include "cfan/Error.h"

#ifndef CF_WIN

#include <sys/types.h>
#include <sys/timeb.h>
#include <time.h>
#include <unistd.h>

uint64_t cf_System_currentTimeMillis() {
  struct timeb val;
  ftime(&val);
  return val.time * 1000 + val.millitm;
//  struct timeval tv={0};
//  struct timezone tz={0};
//  gettimeofday(&tv,&tz);
}

uint64_t cf_System_nanoTicks() {
//  return clock() / (CLOCKS_PER_SECOND * 1000);
  struct timespec ts;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
  return ts.tv_sec * 10E9 + ts.tv_nsec;
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
  return (m_nBeginTime.QuadPart*10E9)/m_nFreq.QuadPart;
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
