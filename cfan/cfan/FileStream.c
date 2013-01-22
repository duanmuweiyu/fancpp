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
  size_t count;
  CF_ENTRY_FUNC

  file = ((cf_FileOutputStream*)self)->file;
  count = fwrite(buf, size, 1, file);
  if (count != 1) {
    CF_EXIT_FUNC
    return cf_Error_io;
  }
  return cf_Error_ok;
  CF_EXIT_FUNC
}

cf_Error cf_FileOutputStream_isError(cf_OutputStream *self) {
  if (ferror(((cf_FileOutputStream*)self)->file)) {
    return cf_Error_io;
  }
  return cf_Error_ok;
}

cf_Error cf_FileInputStream_read(cf_InputStream *self, char *buf, size_t size) {
  FILE *file;
  size_t count;
  CF_ENTRY_FUNC

  file = ((cf_FileInputStream*)self)->file;
  count = fread(buf, size, 1, file);
  if (count != 1) {
    CF_EXIT_FUNC
    return cf_Error_io;
  }
  return cf_Error_ok;
  CF_EXIT_FUNC
}

cf_Error cf_FileInputStream_isError(cf_InputStream *self) {
  if (feof(((cf_FileInputStream*)self)->file)) {
    return cf_Error_io;
  }
  return cf_Error_ok;
}
