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
class CppCompiler : Task
{

  ** Output repository
  File? outHome

  ** lib name
  Str? name

  ** description of pod
  Str? summary

  ** output file name
  Str? outFileName

  ** output file pod dir
  File? outPodDir

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
  Str[]? libName

  ** List of macro define
  Str[]? define

  ** List of include
  File[]? includeDir

  ** List of lib dir
  File[]? libDir

  ** List of source files or directories to compile
  File[]? src

  ** List of resource
  File[]? res := null

  ** Home directory for VC or GCC
  ** configured via config prop
  File? ccHome

  ** meta data
  [Str:Str]? meta

  ** compiler
  Str? compiler

  ** command maker
  private CommandMaker? commandMaker


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
        link(outType == TargetType.dll)
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

    outPodDir = outHome + ("$name-$version/").toUri
    outPodDir.create

    Uri dir := (outType == TargetType.lib) ? `lib/` : `bin/`
    outDir = (outPodDir + dir).create

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
  protected virtual Void compile()
  {
    cmd := commandMaker.getCommond(compiler + ".comp")
    Exec(script, cmd.split).run
  }

  ** link target to exe or dll
  protected virtual Void link(Bool isDll)
  {
    cmd := commandMaker.getCommond(compiler + (isDll ? ".dll" : ".exe"))
    Exec(script, cmd.split).run
  }

  ** make a lib file
  protected virtual Void makeLib()
  {
    cmd := commandMaker.getCommond(compiler + ".lib")
    Exec(script, cmd.split).run
  }

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
      includeDir.each
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
      objDir := outPodDir + `obj/`
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
      libName.each
      {
        libNames.add(it)
      }
      return libNames
  }

  protected File[] allLibPaths()
  {
      File[] paths := [,]
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
      includeDir := (outPodDir + `include/$name/`).create
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

    (outPodDir + `meta.props`).out.writeProps(meta)

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