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
#include "cfan/Memory.h"

#ifdef CF_WIN

#include <Windows.h>

cf_Error cf_File_loadInfo(cf_File *self) {
  HANDLE hFile;
  FILETIME ftCreate, ftAccess, ftWrite;
  DWORD lpFileSizeHigh;
  DWORD lpFileSizeLow;
  DWORD rc;
  __int64 ll;

  rc = GetFileAttributes(self->path);
  if (rc == INVALID_FILE_ATTRIBUTES) {
    goto error;
  }

  if (rc == FILE_ATTRIBUTE_DIRECTORY) {
    self->isDir = true;
  } else {
    self->isDir = false;
  }


  hFile = CreateFile(self->path, GENERIC_READ, FILE_SHARE_READ, NULL,
          OPEN_EXISTING, 0, NULL);

  if(hFile == INVALID_HANDLE_VALUE) {
    goto error;
  }

  // Retrieve the file times for the file.
  if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite)) {
    goto error;
  }

  lpFileSizeLow = GetFileSize(hFile, &lpFileSizeHigh);
  if (lpFileSizeLow == INVALID_FILE_SIZE) {
    goto error;
  }

  self->size = (size_t)lpFileSizeLow;

  //FILETIME to time_t
  ll = ftWrite.dwHighDateTime << 32 + ftWrite.dwLowDateTime;
  self->mtime = (time_t)((__int64)(ll - 116444736000000000) / 10000000);
  self->exists = true;

  CloseHandle(hFile);
  return cf_Error_ok;

error:
  self->exists = false;
  return cf_Error_file;
}

cf_Error cf_File_createDir(cf_File *self) {
  if (CreateDirectory(self->path, NULL) != NULL) {
    return cf_Error_ok;
  }
  return cf_Error_file;
}

cf_Error CF_File_delete(cf_File *self) {
  if (DeleteFile(self->path)) {
    return cf_Error_ok;
  }
  return cf_Error_file;
}

cf_Error cf_DirIterator_make(cf_DirIterator *self, const char *path) {
  WIN32_FIND_DATA *findFileData;
  HANDLE hFind;
  findFileData = (WIN32_FIND_DATA *)cf_malloc(sizeof(WIN32_FIND_DATA)*2);
  hFind = FindFirstFile(path, findFileData);
  if (hFind == INVALID_HANDLE_VALUE)
  {
    cf_free(findFileData);
    self->second = NULL;
    printf ("FindFirstFile failed (%d)\n", GetLastError());
    return cf_Error_file;
  }
  self->first = (void*)hFind;
  self->second = (void*)findFileData;
  return cf_Error_ok;
}

const char* cf_DirIterator_next(cf_DirIterator *self) {
  WIN32_FIND_DATA *findFileData;
  HANDLE hFind;

  hFind = (HANDLE)self->first;
  findFileData = (WIN32_FIND_DATA *)self->second;

  if (hFind == 0) return NULL;
  findFileData[1] = findFileData[0];

  if (!FindNextFile(hFind, findFileData))
  {
    FindClose(hFind);
    self->first = 0;
  }
  return findFileData[1].cFileName;
}

void cf_DirIterator_dispose(cf_DirIterator *self) {
  WIN32_FIND_DATA *findFileData;
  HANDLE hFind;

  hFind = (HANDLE)self->first;
  findFileData = (WIN32_FIND_DATA *)self->second;

  if (hFind) FindClose(hFind);
  if (findFileData) cf_free(findFileData);
}

#endif
