//
// Copyright (c) 2010, chunquedong
// Licensed under the Academic Free License version 3.0
//
// History:
//   2011-4-3  Jed Young  Creation
//

using build

**
** Run the Cpp compiler
**
abstract class CppCompiler : Task
{

  ** Output file created by the compiler.
  File? outHome

  ** lib name
  Str? name

  ** description of pod
  Str? summary

  ** output file name
  Str? outFileName

  ** output home dir name
  File? outPod

  ** output file dir
  File? outDir

  ** lib depends
  Depend[]? depends

  ** lib depends
  Version? version

  ** Output target type
  TargetType? outType

  ** is debug mode
  Bool debug := false

  ** List of ext librarie to link in
  Str[]? extLibs

  ** List of include
  File[]? extIncludes

  ** List of source files or directories to compile
  File[]? src

  ** List of resource
  File[]? res := null

  ** Home directory for VC or GCC
  ** configured via config prop
  File? ccHome

  **
  ** why need this? /C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib/
  **
  File? winSdk

  ** meta data
  [Str:Str]? meta


  ** ctor
  new make(BuildScript script)
    : super(script)
  {}

//////////////////////////////////////////////////////////////////////////
// Run
//////////////////////////////////////////////////////////////////////////

  **
  ** Run the cc task
  **
  override Void run()
  {
    log.info("CompileCpp")

    try
    {
      init
      compile
      if (outType == TargetType.lib)
        makeLib
      else
        link
      install
    }
    catch (Err err)
    {
      throw fatal("CompileCpp failed")
    }
  }

  ** init. since dump test
  protected virtual Void init()
  {
    outFileName = debug? "${name}-d.$outType" : "${name}.$outType"

    outPod = outHome + ("$name-$version/").toUri
    outPod.create

    Uri dir := (outType == TargetType.lib) ? `lib/` : `bin/`
    outDir = (outPod + dir).create

    meta =
    [
      "pod.name" : name,
      "pod.version" : version.toStr,
      "pod.depends" : depends.map { it.toStr } ->join(";"),
      "pod.summary" : summary,
    ]
  }

  ** output file path
  protected File output()
  {
    outDir + outFileName.toUri
  }

  ** compile the source code
  protected abstract Void compile()

  ** link target to exe or dll
  protected abstract Void link()

  ** make a lib file
  protected abstract Void makeLib()

//////////////////////////////////////////////////////////////////////////
// Compile
//////////////////////////////////////////////////////////////////////////


  protected File[] srcCodes()
  {
      File[] srcs := [,]
      src.each |File f|
      {
        if (f.isDir){
          f.listFiles.each {
            ext := it.ext
            if (ext == "cpp" || ext == "c" || ext == "cc" || ext == "cxx") {
              srcs.add(it)
            }
          }
        }else{
          srcs.add(f)
        }
      }
      return srcs
  }

  protected File[] allIncludes()
  {
      File[] incs := [,]

      //compiler include
      compilerInc := `${ccHome}include/`.toFile
      incs.add(compilerInc)

      //platform include
      platformInc := `${winSdk}Include/`.toFile
      incs.add(platformInc)

      //user include
      extIncludes.each
      {
        incs.add(it)
      }

      //depends include
      depends.each
      {
        dep := outHome + `${it.name}-${it.version}/include/`
        if (!dep.exists) throw fatal("don't find the depend $it")
        incs.add(dep)
      }

      return incs
  }

//////////////////////////////////////////////////////////////////////////
// Link
//////////////////////////////////////////////////////////////////////////

  protected File[] objFiles()
  {
      File[] objs := [,]
      objDir := outPod + `obj/`
      objDir.listFiles.each
      {
        if (it.ext == "obj")
        {
          objs.add(it)
        }
      }
      return objs
  }

  protected Str[] allLibs()
  {
      Str[] libNames := [,]

      //depend libs
      depends.each
      {
        dep := outHome + `${it.name}-${it.version}/lib/`
        count := 0
        dep.listFiles.each
        {
          if (it.ext == "lib" || it.ext == "a")
          {
            libNames.add(it.name)
            count++
          }
        }
        if (count == 0)
          throw fatal("don't find any lib in ${it.name}-${it.version}/lib/")
      }

      //user libs
      extLibs.each
      {
        libNames.add(it)
      }
      return libNames
  }
/*
//TODO: cache .obj
//usning `dependsLibs`

  private File[] dependsObjs()
  {
      File[] objs := [,]
      depends.each
      {
        dep := outHome + `${it.name}-${it.version}/obj/`
        dep.listFiles.each
        {
          if (it.ext == "obj")
          {
            objs.add(it)
          }
        }
      }
      return objs
  }
*/

  protected File[] allLibPaths()
  {
      File[] paths := [,]

      //compiler path
      compilerPath := `${ccHome}lib/`.toFile
      paths.add(compilerPath)

      //platform path
      platformPath := `${winSdk}Lib/`.toFile
      paths.add(platformPath)

      //depend libs path
      depends.each
      {
        dep := outHome + `${it.name}-${it.version}/lib/`
        paths.add(dep)
      }
      return paths
  }

//////////////////////////////////////////////////////////////////////////
// install
//////////////////////////////////////////////////////////////////////////

  **
  ** copy include head and res
  **
  Void install()
  {
    if (res != null)
    {
      copyInto(res, outDir, false, ["overwrite":true])
    }

    if (outType != TargetType.exe)
    {
      //copy include files
      includeDir := (outPod + `include/$name/`).create
      copyInto(src, includeDir, true,
        [
          "overwrite":true,
          "exclude":|File f->Bool|
          {
            if (f.isDir) return false
            return f.ext != "h" && f.ext != "hpp"
          }
        ])
    }

    (outPod + `meta.props`).out.writeProps(meta)

    log.info("outFile: " + output.osPath)
  }

  private Void copyInto(File[] src, File dir, Bool flatten, [Str:Obj]? options := null)
  {
    src.each |File f|
    {
      if (f.isDir && flatten)
      {
        f.listFiles.each
        {
          it.copyInto(dir, options)
        }
      }else{
        f.copyInto(dir, options)
      }
    }
  }

}