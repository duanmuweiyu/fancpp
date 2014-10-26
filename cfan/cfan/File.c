/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-6  Jed Young  Creation
 */

#include "cfan/File.h"
#include "cfan/Trace.h"

cf_Error cf_File_inputStream(cf_File *self, cf_FileInputStream *in) {
  CF_ENTRY_FUNC

  in->file = fopen(self->path, "rb");
  if (!in->file) {
    CF_EXIT_FUNC
    return cf_Error_io;
  }
  CF_EXIT_FUNC
  return cf_Error_ok;
}

cf_Error cf_File_outputStream(cf_File *self, cf_FileOutputStream *out, bool append) {
  const char *mode;
  CF_ENTRY_FUNC

  mode = append ? "ab" : "wb";
  out->file = fopen(self->path, mode);
  if (!out->file) {
    CF_EXIT_FUNC
    return cf_Error_io;
  }
  CF_EXIT_FUNC
  return cf_Error_ok;
}


cf_Error cf_File_mkdirs(const char* path){
  int len = strlen(path);
  char temp[CF_PATH_MAX];
  int lastSep = 0;
  int i;
  cf_Error err;
  cf_File file;

  strcpy(temp, path);
  for (i = 2; i < len; ++i) {
    if (path[i] == '/' || path[i] == '\\'){
      lastSep = i;
      if(i > 0){
        temp[i] = 0;
        cf_File_make(&file, temp);
        err = cf_File_createDir(&file);
      }
      temp[i] = '/';
    }
  }

  //last dir
  if (lastSep + 1 < len) {
    cf_File_make(&file, temp);
    err = cf_File_createDir(&file);
  }
  return err;
}

#include "cfan/File_unix.inc"
#include "cfan/File_win.inc"
