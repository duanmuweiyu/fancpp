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
