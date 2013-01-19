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

void cf_FileTest_test(void)
{
  const char *path = "/home/jed";
  cf_DirIterator iter;
  const char *name;
  CF_ENTRY_FUNC

  cf_DirIterator_make(&iter, path);

  while (name = cf_DirIterator_next(&iter)) {
    printf("%s\n", name);
  }

  cf_DirIterator_dispose(&iter);

  CF_EXIT_FUNC
}

