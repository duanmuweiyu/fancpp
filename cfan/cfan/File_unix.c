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

#ifndef WIN32

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <dirent.h>

cf_Error cf_File_loadInfo(cf_File *self) {
  struct stat stbuf;
  if (stat(self->path, &stbuf) == -1) {
    self->exists = false;
    return cf_Error_io;
  }
  self->size = stbuf.st_size;
  self->isDir = S_ISDIR(stbuf.st_mode);
  self->mtime = stbuf.st_mtime;
  self->exists = true;
  return cf_Error_ok;
}

cf_Error cf_File_createDir(cf_File *self) {
  if (mkdir(self->path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
    return cf_Error_io;
  }
  return cf_Error_ok;
}

cf_Error CF_File_delete(cf_File *self) {
  if (remove(self->path) == 0) {
    return cf_Error_ok;
  }
  return cf_Error_io;
}

cf_Error cf_DirIterator_make(cf_DirIterator *self, const char *path) {
  DIR *dir = opendir(path);
  self->first = dir;
  return dir == NULL ? cf_Error_io : cf_Error_ok;
}

const char* cf_DirIterator_next(cf_DirIterator *self) {
  DIR *dir = (DIR*)self->first;
  struct dirent *ent = NULL;
  if (NULL != (ent = readdir(dir))) {
    self->second = ent;
    return ent->d_name;
  }
  return NULL;
}

void cf_DirIterator_dispose(cf_DirIterator *self) {
  DIR *dir = (DIR*)self->first;
  closedir(dir);
}

#endif
