/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

#include <stdio.h>
#include "cfan/cfan.h"

CF_BEGIN

#ifdef CF_WIN
  const char *path = "C:\\Windows";
#else
  const char *path = "/home";
#endif

void cf_FileTest_test(void) {
  cf_File file;
  cf_Error err;

  CF_ENTRY_FUNC

  cf_File_make(&file, path);
  err = cf_File_loadInfo(&file);

  cf_verify(err);
  cf_verify(cf_File_isDir(&file));

  CF_EXIT_FUNC
}

void cf_FileTest_dirIterator(void) {

  cf_DirIterator iter;
  const char *name;
  cf_Error err;
  int count = 0;
  CF_ENTRY_FUNC

  err = cf_DirIterator_make(&iter, path);
  if (err) {
    printf("file %s notfount\n", path);
    cf_verify(false);
    CF_EXIT_FUNC
    return;
  }

  while ((name = cf_DirIterator_next(&iter)) != NULL) {
    printf("%s\n", name);
    ++count;
  }

  cf_DirIterator_dispose(&iter);

  cf_verify(count > 2);
  CF_EXIT_FUNC
}

void cf_FileTest_register(void) {
  cf_Test_add(cf_FileTest_test);
  cf_Test_add(cf_FileTest_dirIterator);
}

CF_END
