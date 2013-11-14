/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#if !defined(WIN32) && !defined(__ANDROID__)

#include "cfan/StackTrace.h"
#include "cfan/Error.h"

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cf_StackTrace_printTrace() {
  void *array[100];
  size_t size;
  char **strings;
  size_t i;
  size = backtrace (array, 100);
  strings = backtrace_symbols (array, size);
  if (NULL == strings) {
    perror("backtrace_synbols");
    exit(1);
  }

  printf ("Obtained %zd stack frames.\n", size);

  for (i = 0; i < size; i++) {
    printf ("%s\n", strings[i]);
  }
  free (strings);
}

char *cf_StackTrace_getTraceString() {
  void *array[100];
  size_t size;
  char **strings;
  size_t i;

  int strSize = 0;
  int strAlloc;
  int nameSize;
  char *str;
  char *name;

  str = (char*)malloc(256);
  if (str == NULL) {
    cf_abort("bad alloc");
  }
  strAlloc = 255;

  size = backtrace (array, 100);
  strings = backtrace_symbols (array, size);
  if (NULL == strings) {
    str[strSize] = 0;
    return str;
  }

  for (i = 0; i < size; i++) {
    name = strrchr(strings[i], '/');
    if (!name) continue;

    nameSize = strlen(name)+1;
    if (strSize + nameSize > strAlloc) {
      strAlloc = (strSize + nameSize)*3/2;
      str = (char*)realloc(str, strAlloc+1);
      if (str == NULL) {
        cf_abort("bad alloc");
      }
    }
    strcpy(str+strSize, name);
    strSize += nameSize;
    str[strSize-1] = ',';
    str[strSize] = '\0';
  }
  free (strings);

  str[strSize] = 0;
  return str;
}

#endif