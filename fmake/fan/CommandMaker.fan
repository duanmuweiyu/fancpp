//
// Copyright (c) 2010, chunquedong
// Licensed under the Academic Free License version 3.0
//
// History:
//   2011-4-3  Jed Young  Creation
//

using build

**
** CommandMaker
**
class CommandMaker
{
  ** List of ext librarie to link in
  Str[] libName := [,]

  ** List of macro define
  Str[] define := [,]

  ** List of include
  Str[] includeDir := [,]

  ** List of lib dir
  Str[] libDir := [,]

  ** List of source files or directories to compile
  Str[] srcList := [,]

  ** output file
  Str outFile

  ** List of obj file
  Str[] objFile := [,]

  ** config map
  Str:Str config


  new make(|This| f) { f(this) }

  private Str getValue(Str key)
  {
    config[key] ?: throw Err("not find config: $key")
  }

  Str getCommond(Str cmd)
  {
    config.each |v, k|
    {
      config[k] = fillParam(v)
    }

    Str commond := getValue(cmd)
    config.each |v, k|
    {
      commond = commond.replace(k, v)
    }
    return commond
  }

  private Str fillParam(Str cmd)
  {
    Str result := ""
    this.typeof.fields.each |f|
    {
      if (f.parent == this.typeof)
      {
        Str name := "{$f.name}"
        if (cmd.contains(name))
        {
          if (f.type != Str#)
          {
            Str[] strs := f.get(this)
            strs.each |item|
            {
              result += cmd.replace(name, item) + " "
            }
          }
          else
          {
            result = cmd.replace(name, f.get(this).toStr)
          }
        }
      }
    }
    return result
  }
}