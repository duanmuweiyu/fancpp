/*
 * Copyright (c) 2012-2013, chunquedong
 *
 * This file is part of fancpp project
 * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
 *
 * History:
 *   2012-12-23  Jed Young  Creation
 */

using fmake

class Build : BuildCpp
{
  new make()
  {
    name = "cfan"
    summary = "cfan is a base C library"
    outType = TargetType.lib
    version = Version("1.0.0")
    srcDirs = [`cfan/`]

    extConfigs["cflag"] = "-std=c99"
  }
}