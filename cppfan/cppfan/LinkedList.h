#ifndef _CPPF_LINKEDLIST_H_
#define _CPPF_LINKEDLIST_H_

#include "cppfan/Object.h"

CF_BEGIN_NAMESPACE

template<typename T>
class LinkedList : public Object {
  T *head;
  T *tail;
  int length;
public:
  LinkedList() : head(NULL), tail(NULL), length(0) {
  }

  int size() { return length; }

  LinkedList &add(T *val) {
    if (head == NULL) {
      head = val;
      head->previous = NULL;
    }
    if (tail == NULL) {
      tail = val;
      tail->next = NULL;
    } else {
      tail->next = val;
      val->previous = tail;
      val->next = NULL;
      tail = val;
    }
    ++length;
    return *this;
  }

  void insertFirst(T *val) {
    if (tail == NULL) {
      tail = val;
      tail->next = NULL;
    }
    if (head == NULL) {
      head = val;
      head->previous = NULL;
    } else {
      val->next = head;
      val->previous = NULL;
      head->previous = val;
      head = val;
    }
    ++length;
  }

  T *getAt(int index) {
    int i=0;
    T *t = head;
    while (t) {
      if (i == index) {
        return t;
      }
      t = t->next;
      ++i;
    }
    return NULL;
  }

  void insertBefore(T *val, T *pos) {
    cf_assert(head);
    if (pos == head) {
      insertFirst(val);
      return;
    }
    pos->previous->next = val;
    val->previous = pos->previous;
    val->next = pos;
    pos->previous = val;
    ++length;
  }

  bool remove(T *val) {
    if (val == head) {
      head = val->next;
      if (head) {
        head->previous = NULL;
      }
    }
    else if (val == tail) {
      tail = val->previous;
      if (tail) {
        tail->next = NULL;
      }
    }
    else if (val->previous && val->next) {
      val->previous->next = val->next;
      val->next->previous = val->previous;
    } else {
      return false;
    }

    val->previous = NULL;
    val->next = NULL;
    --length;
    return true;
  }

  T *first() { return head; }
  T *last() { return tail; }
};

CF_END_NAMESPACE

#endif // LINKEDLIST_H
