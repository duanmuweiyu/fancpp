/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2013-1-6  Jed Young  Creation
 */

#include "cfan/FileStream.h"


cf_Error cf_FileOutputStream_write(cf_OutputStream *self, char *buf, size_t size) {
  FILE *file;
  CF_ENTRY_FUNC

  file = ((cf_FileOutputStream*)self)->file;
  fwrite(buf, size, 1, file);
  if (ferror(file)) {
    CF_EXIT_FUNC
    return cf_Error_io;
  }

  CF_EXIT_FUNC
  return cf_Error_ok;
}

cf_Error cf_FileInputStream_read(cf_InputStream *self, char *buf, size_t size) {
  FILE *file;
  CF_ENTRY_FUNC

  file = ((cf_FileInputStream*)self)->file;
  fread(buf, size, 1, file);

  if (feof(file)) {
    CF_EXIT_FUNC
    return cf_Error_eof;
  }
  CF_EXIT_FUNC
  return cf_Error_ok;
}
