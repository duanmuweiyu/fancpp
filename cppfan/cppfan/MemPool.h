#ifndef _CPPF_MEMPOOL_H_
#define _CPPF_MEMPOOL_H_

#include "cppfan/Object.h"
#include "cppfan/LinkedList.h"

CF_BEGIN_NAMESPACE

class MemPool : public Object {
  struct MemPoolElem {
    MemPoolElem *next;
    MemPoolElem *previous;
    bool isBuffered;
    int size;
    int checkCode;
  };

  LinkedList<MemPoolElem> list;
  MemPoolElem *bufferedElemList;

  int size;
  int itemSize;
  int allocCount;

public:
  MemPool(int objSize, int size);

  ~MemPool();

  char *alloc();

  void free(char *t);

private:
  void check(MemPoolElem *elem);
};

CF_END_NAMESPACE
#endif // MEMPOOL_H
