//
// Copyright (c) 2010, chunquedong
// Licensed under the Academic Free License version 3.0
//
// History:
//   2011-4-3  Jed Young  Creation
//

using build

**
** Run the Cpp compiler to produce an exe or dll.
**
class VcCompiler : Task
{

  ** Output file created by the compiler.
  File? outHome

  ** lib name
  Str? name

  ** output file name
  Str? outFileName

  ** output dir name
  File? outPod

  ** lib depends
  Depend[]? depends

  ** lib depends
  Version? version

  ** Output target type
  Str? targetType

  ** is debug mode
  Bool debug := false

  ** List of librarie Paths to link in
  File[]? libPaths

  ** List of include
  File[]? includes

  ** List of source files or directories to compile
  File[]? src

  ** List of resource
  File[]? res := null

  ** Home directory for VC or GCC
  ** configured via config prop
  File ccHome

  **
  ** why need this? /C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib/
  **
  File winSdk

//////////////////////////////////////////////////////////////////////////
// Construction
//////////////////////////////////////////////////////////////////////////

  new make(BuildScript script)
    : super(script)
  {
    ccHome = script.configDir("vcHome") ?:
      throw fatal("Must config build prop 'vcHome'")
    winSdk = script.configDir("winSdk") ?:
      throw fatal("Must config build prop 'winSdk'")
  }

  private Void init()
  {
    outFileName = debug? "${name}-d.$targetType" : "${name}.$targetType"
    outPod = outHome + ("$name-$version.toStr/").toUri
    outPod.create
  }

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
      if (targetType == "lib")
        makeLib
      else
        link
      install
    }
    catch (Err err)
    {
      log.info("err: $err.msg")
      throw fatal("CompileCpp failed")
    }
  }

//////////////////////////////////////////////////////////////////////////
// Compile
//////////////////////////////////////////////////////////////////////////

  **
  ** compile code to .obj
  **
  Void compile()
  {
      // build command
      ccExe := ccHome + `bin/cl.exe`
      cmd := [ccExe.osPath]

      // default paramaters
      cmd.add("/c")//no link
      cmd.add("/EHsc")//suppert exception

      //compile mode
      cmd.add(compileMode)

      //include
      allIncludes.each
      {
        cmd.add("/I${it.osPath}")
      }

      //output
      objDir := (outPod + `obj/`).create
      objOut := objDir.osPath + "\\"
      cmd.add("/Fo$objOut")

      // src files/dirs
      srcCodes.each { cmd.add(it.osPath) }

      //run
      Exec(script, cmd).run
  }

  private File[] srcCodes()
  {
      File[] srcs := [,]
      src.each |File f|
      {
        if (f.isDir){
          f.listFiles.each {
            ext := it.ext
            if (ext == "cpp" || ext == "c" || ext == "cc") {
              srcs.add(it)
            }
          }
        }else{
          srcs.add(f)
        }
      }
      return srcs
  }

  private Str compileMode()
  {
      if (targetType == "exe" || targetType == "lib")
      {
        return (debug? "/MTd": "/MT")
      }
      else if(targetType == "dll")
      {
        return (debug? "/MDd": "/MD")
      }
      else
      {
        throw fatal("unknow targetType")
      }
  }

  private File[] allIncludes()
  {
      File[] incs := [,]

      //compiler include
      compilerInc := `${ccHome}include/`.toFile
      incs.add(compilerInc)

      //platform include
      platformInc := `${winSdk}Include/`.toFile
      incs.add(platformInc)

      //user include
      includes.each
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

  **
  ** link objs and output .exe or .dll
  **
  Void link()
  {
      // build command
      linkExe := ccHome + `bin/link.exe`
      cmd := [linkExe.osPath]

      //lib Paths
      allLibPaths.each
      {
        cmd.add("/LIBPATH:${it.osPath}")
      }

      //DLL
      if(targetType == "dll")
      {
        cmd.add("/DLL")
      }

      //output
      cmd.add("/OUT:${output.osPath}")

      // objs files
      objFiles.each { cmd.add(it.osPath) }

      //depends libs
      dependsLibs.each { cmd.add(it.osPath) }

      //run
      Exec(script, cmd).run
  }

  **
  ** create a .lib file
  **
  Void makeLib()
  {
      libExe := ccHome + `bin/lib.exe`
      cmd := [libExe.osPath]

      //lib Paths
      allLibPaths.each
      {
        cmd.add("/LIBPATH:${it.osPath}")
      }

      //output
      cmd.add("/OUT:${output.osPath}")


      // objs files
      objFiles.each { cmd.add(it.osPath) }

      //depends libs
      dependsLibs.each { cmd.add(it.osPath) }

      //run
      Exec(script, cmd).run
  }

  private File[] objFiles()
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

  private File[] dependsLibs()
  {
      File[] libs := [,]
      depends.each
      {
        dep := outHome + `${it.name}-${it.version}/bin/`
        count := 0
        dep.listFiles.each
        {
          if (it.ext == "lib")
          {
            libs.add(it)
            count++
          }
        }
        if (count == 0)
          throw fatal("don't find any lib in ${it.name}-${it.version}/bin/")
      }
      return libs
  }

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

  private File output()
  {
    outDir := (outPod + `bin/`).create
    return outDir + outFileName.toUri
  }

  private File[] allLibPaths()
  {
      File[] paths := [,]

      //compiler path
      compilerPath := `${ccHome}lib/`.toFile
      paths.add(compilerPath)

      //platform path
      platformPath := `${winSdk}Lib/`.toFile
      paths.add(platformPath)

      //user path
      libPaths.each
      {
        paths.add(it)
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
      copyInto(res, outPod + `bin/`, false, ["overwrite":true])
    }

    if (targetType != "exe")
    {
      //copy include files
      includeDir := (outPod + `include/$name/`).create
      copyInto(src, includeDir, true,
        [
          "overwrite":true,
          "exclude":|File f->Bool|
          {
            if (f.isDir) return false
            return f.ext != "h"
          }
        ])
    }

    log.info("outFile: " + (outPod + `bin/$outFileName`).osPath)
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