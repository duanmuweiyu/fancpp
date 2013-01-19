/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-6  Jed Young  Creation
 */

#include "cfan/Config.h"


cf_Error cf_Config_make(cf_Config *self, const char *path) {
  FILE *file;
  char c;//current char
  char key[1024];//key buffer
  char value[1024];//value buffer
  char *k;//key dup
  char *v;//value dup
  int pos;//current position
  int mode;//token mode, 0 is key, 1 is value;
  CF_ENTRY_FUNC

  //open file
  file = fopen(path, "r");
  if (!file) {
    CF_EXIT_FUNC
    return cf_Error_io;
  }

  //make map
  cf_HashMapSS_make(&self->map, 1024);

  c = fgetc(file);
  pos = 0;
  mode = 0;
  while (EOF != c) {
    if (c == '\r' || c == '\n') {
      if (mode == 1) {
        // deal new line
        mode = 0;
        value[pos] = 0;
        k = (char*)cf_malloc(strlen(key)+1);
        strcpy(k, key);
        v = (char*)cf_malloc(strlen(value)+1);
        strcpy(v, value);
        cf_HashMapSS_set(&self->map, k, v, NULL, NULL);
      }
      pos = 0;
    } else {
      if (mode == 0) {
        // read key
        if (c == '=') {
          //end read key
          mode = 1;
          key[pos] = 0;
          pos = 0;
        } else {
          key[pos++] = c;
        }
      } else {
        //read value
        value[pos++] = c;
      }
    }

    c = fgetc(file);
  }

  //last line
  if (pos > 0) {
    value[pos] = 0;
    k = (char*)cf_malloc(strlen(key)+1);
    strcpy(k, key);
    v = (char*)cf_malloc(strlen(value)+1);
    strcpy(v, value);
    cf_HashMapSS_set(&self->map, k, v, NULL, NULL);
  }

  fclose(file);

  CF_EXIT_FUNC
  return cf_Error_ok;
}
