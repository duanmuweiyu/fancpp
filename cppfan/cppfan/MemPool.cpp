#include "cppfan/MemPool.h"

CF_USING_NAMESPACE

#define pf_MemPool_headCheckCode 0xA9C4

MemPool::MemPool(int objSize, int size) : size(size), allocCount(0) {
  itemSize = sizeof(MemPoolElem)+objSize;
  itemSize = CF_ALIGN(itemSize);
  bufferedElemList = (MemPoolElem*)cf_malloc(itemSize*size);
  for (int i=0; i<size; ++i) {
    bufferedElemList[i].isBuffered = true;
    bufferedElemList[i].checkCode = pf_MemPool_headCheckCode;
    list.add(bufferedElemList+i);
  }
}

MemPool::~MemPool() {
  cf_assert(allocCount == 0);
  MemPoolElem *elem = list.first();
  for (int i=0; i<size; ++i) {
    check(elem);
    if (!elem[i].isBuffered) {
      cf_free(elem+i);
    }
  }
  cf_free(bufferedElemList);
}

char *MemPool::alloc() {
  MemPoolElem *elem;
  if (list.size() > 0) {
    elem = list.first();
  } else {
    elem = (MemPoolElem*)cf_malloc(itemSize);
    elem->checkCode = pf_MemPool_headCheckCode;
  }
  ++allocCount;
  check(elem);
  return (char*)(elem+1);
}

void MemPool::free(char *t) {
  MemPoolElem *elem = (MemPoolElem*)(((char*)t)-sizeof(MemPoolElem));
  check(elem);
  list.add(elem);
}

void MemPool::check(MemPoolElem *elem) {
  if (elem->checkCode != pf_MemPool_headCheckCode) {
    cf_abort("MemPool check fail");
  }
}
