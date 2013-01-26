
#include <stdlib.h>
#include <vector>
#include <map>
#include <algorithm>
#include <stdio.h>
#include <hash_map>

#ifdef _WIN32
  typedef __int64 Int64;
#else
  typedef long long Int64;
#endif

#ifdef __GNUC__
  #include <ext/hash_map>
#else
  #include <hash_map>
#endif

namespace stdext
{
  using namespace __gnu_cxx;
}

#ifdef _WIN32
  #include <windows.h>
  Int64 nowTicks()
  {
    LARGE_INTEGER m_nBeginTime;
    LARGE_INTEGER m_nFreq;
    QueryPerformanceFrequency(&m_nFreq);
    QueryPerformanceCounter(&m_nBeginTime);
    return (m_nBeginTime.QuadPart*1000000)/m_nFreq.QuadPart;
    //return m_nBeginTime.QuadPart;
  }
#else
  #include <time.h>
  Int64 nowTicks()
  {
    return clock();
  }
#endif

#include "cfan/cfan.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#define array_size 1000
int testData[array_size];

Int64 cfanArrayMakeTime = 0;
Int64 cfanArraySortTime = 0;
Int64 stlArrayMakeTime = 0;
Int64 stlArraySortTime = 0;

Int64 cfanMapMakeTime = 0;
Int64 cfanMapQueryTime = 0;
Int64 stlMapMakeTime = 0;
Int64 stlMapQueryTime = 0;

void testCfanArray()
{
  cf_ArrayI array;
  int i;
  int n;

  cf_ArrayI_make(&array, 0, array_size);

  Int64 t1 = nowTicks();
  for (i=0; i<array_size; ++i)
  {
    n = testData[i];
    cf_ArrayI_addCopy(&array, n);
  }
  Int64 t2 = nowTicks();
  cfanArrayMakeTime += (t2-t1);

  Int64 t3 = nowTicks();
  cf_ArrayI_sort(&array);
  Int64 t4 = nowTicks();
  cfanArraySortTime += (t4-t3);

  cf_ArrayI_dispose(&array);
}

void testStlVector()
{
  std::vector<int> list;
  int i;
  int n;

  Int64 t1 = nowTicks();
  for (i=0; i<array_size; ++i)
  {
    n = testData[i];
    list.push_back(n);
  }
  Int64 t2 = nowTicks();
  stlArrayMakeTime += (t2-t1);

  Int64 t3 = nowTicks();
  std::sort(list.begin(), list.end());
  Int64 t4 = nowTicks();
  stlArraySortTime += (t4-t3);
}

void testCfanMap()
{
  cf_HashMapII map;
  int i;
  int n;
  int v;
  cf_HashMapII_make(&map, array_size);

  Int64 t1 = nowTicks();
  for (i=0; i<array_size; ++i)
  {
    n = testData[i];
    cf_HashMapII_set(&map, n, n, NULL, NULL);
  }
  Int64 t2 = nowTicks();
  cfanMapMakeTime += (t2-t1);

  Int64 t3 = nowTicks();
  for (i=0; i<array_size; ++i)
  {
    cf_HashMapII_get(&map, i, &v, &n);
  }
  Int64 t4 = nowTicks();
  cfanMapQueryTime += (t4-t3);

  cf_HashMapII_dispose(&map);
}

void testStlMap()
{
  stdext::hash_map<int,int> map;
  int i;
  int n;
  int v;

  Int64 t1 = nowTicks();
  for (i=0; i<array_size; ++i)
  {
    n = testData[i];
    map[n] = n;
  }
  Int64 t2 = nowTicks();
  stlMapMakeTime += (t2-t1);

  Int64 t3 = nowTicks();
  for (i=0; i<array_size; ++i)
  {
    v = map[i];
  }
  Int64 t4 = nowTicks();
  stlMapQueryTime += (t4-t3);
}

void testPerformance()
{
  int i;
  for (i=0; i<array_size; ++i)
  {
    testData[i] = rand();//(((double)rand())/RAND_MAX)*100;
  }
  testStlVector();
  testCfanArray();
  testStlMap();
  testCfanMap();
}

int main2()
{
  int i;

  for (i=0; i<1000; ++i)
  {
    testPerformance();
  }
  printf("array make STL-CFan %lld\n", (stlArrayMakeTime - cfanArrayMakeTime));
  printf("array sort STL-CFan %lld\n", (stlArraySortTime - cfanArraySortTime));
  printf("map make STL-CFan %lld\n", (stlMapMakeTime - cfanMapMakeTime));
  printf("map query STL-CFan %lld\n", (stlMapQueryTime - cfanMapQueryTime));

  //getchar();
  return 0;
}