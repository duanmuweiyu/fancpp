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

//////////////////////////////////////////////////////////////////////////
// Construction
//////////////////////////////////////////////////////////////////////////

  new make(BuildScript script)
    : super(script)
  {
  }

  **
  ** Initialize the environment
  **
  private This init()
  {
      ccHomeDir = script.configDir("vcHome") ?:
        throw fatal("Must config build prop 'vcHome'")
      winSdk = script.configDir("winSdk") ?:
        throw fatal("Must config build prop 'winSdk'")

      outName = debug? "${name}-d.$targetType" : "${name}.$targetType"

      return this
  }

//////////////////////////////////////////////////////////////////////////
// Run
//////////////////////////////////////////////////////////////////////////

  **
  ** Run the cc task
  **
  override Void run()
  {
    init
    log.info("CompileCpp")

    try
    {
      compile
      if (targetType == "lib"){
        lib
      }else{
        link
      }
      install
    }
    catch (Err err)
    {
      log.info("err: $err.msg")
      throw fatal("CompileCpp failed")
    }
  }

  Void compile()
  {
      // build command
      ccExe := ccHomeDir + `bin/cl.exe`
      cmd := [toOsPath(ccExe)]

      // default paramaters
      cmd.add("/c")//no link
      cmd.add("/EHsc")//suppert exception

      //compile mode
      if (targetType == "exe" || targetType == "lib")
      {
        cmd.add(debug? "/MTd": "/MT")
      }
      else if(targetType == "dll")
      {
        cmd.add(debug? "/MDd": "/MD")
      }
      else
      {
        throw fatal("unknow targetType")
      }

      //include
      include := toOsPath(`${ccHomeDir}include/`.toFile)
      cmd.add("/I$include")
      includes.each
      {
        cmd.add("/I${toOsPath(it)}")
      }

      //depends include
      depends.each
      {
        dep := output + `$it/include/`
        if (!dep.exists) throw fatal("don't find the depend $it")
        depInclude := toOsPath(dep)
        cmd.add("/I$depInclude")
      }

      //output
      objDir := (output + `$name/obj/`).create
      objOut := objDir.osPath + "\\"
      cmd.add("/Fo$objOut")

      // src files/dirs
      src.each |File f|
      {
        if (f.isDir){
          f.listFiles.each
          {
            if (it.ext == "cpp" || it.ext == "c")
            {
              cmd.add(toOsPath(it))
            }
          }
        }else{
          cmd.add(toOsPath(f))
        }
      }

      //run
      log.debug(cmd.join(" "))
      r := Process(cmd).run.join
      if (r != 0) throw Err.make
  }

  Void link()
  {
      // build command
      linkExe := ccHomeDir + `bin/link.exe`
      cmd := [toOsPath(linkExe)]

      //libs
      sysLib := toOsPath(`${ccHomeDir}lib/`.toFile)
      cmd.add("/LIBPATH:$sysLib")
      cmd.add("/LIBPATH:${winSdk.osPath}")
      libs.each
      {
        cmd.add("/LIBPATH:${toOsPath(it)}")
      }

      //DLL
      if(targetType == "dll")
      {
        cmd.add("/DLL")
      }

      //output
      outDir := (output + `$name/bin/`).create
      binOut := toOsPath(outDir + outName.toUri)
      cmd.add("/OUT:${binOut}")

      // obj files/dirs
      objDir := output + `$name/obj/`
      objDir.listFiles.each
      {
        if (it.ext == "obj")
        {
          cmd.add(toOsPath(it))
        }
      }

      //depends obj
      depends.each
      {
        dep := output + `$it/obj/`
        dep.listFiles.each
        {
          if (it.ext == "obj")
          {
            cmd.add(toOsPath(it))
          }
        }
      }

      //run
      log.debug(cmd.join(" "))
      r := Process(cmd).run.join
      if (r != 0) throw Err.make
  }

  Void lib()
  {
      libExe := ccHomeDir + `bin/lib.exe`
      cmd := [toOsPath(libExe)]

      //libs
      sysLib := toOsPath(`${ccHomeDir}lib/`.toFile)
      cmd.add("/LIBPATH:$sysLib")
      cmd.add("/LIBPATH:${winSdk.osPath}")
      libs.each
      {
        cmd.add("/LIBPATH:${toOsPath(it)}")
      }

      //output
      outDir := (output + `$name/bin/`).create
      binOut := toOsPath(outDir + outName.toUri)
      cmd.add("/OUT:${binOut}")

      // obj files/dirs
      objDir := output + `$name/obj/`
      objDir.listFiles.each
      {
        if (it.ext == "obj")
        {
          cmd.add(toOsPath(it))
        }
      }

      //depends obj
      depends.each
      {
        dep := output + `$it/obj/`
        dep.listFiles.each
        {
          if (it.ext == "obj")
          {
            cmd.add(toOsPath(it))
          }
        }
      }

      //run
      log.debug(cmd.join(" "))
      r := Process(cmd).run.join
      if (r != 0) throw Err.make
  }

  Void install()
  {
    if (res != null)
    {
      copyInto(res, output + `$name/bin/`, false, ["overwrite":true])
    }

    if (targetType != "exe")
    {
      //copy include files
      includeDir := (output + `$name/include/$name/`).create
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

    log.info("outFile: " + toOsPath(output + `$name/bin/$outName`))
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

  static Str toOsPath(File f)
  {
    //"\"" + f.osPath + "\""
    f.osPath
  }

//////////////////////////////////////////////////////////////////////////
// Fields
//////////////////////////////////////////////////////////////////////////

  ** Home directory for VC or GCC
  ** configured via config prop
  File? ccHomeDir

  **
  ** why need this? /C:/Program Files/Microsoft SDKs/Windows/v6.0A/Lib/
  **
  File? winSdk

  ** Output file created by the compiler.
  File? output

  ** lib name
  Str? name

  ** output file name
  Str? outName

  ** lib version
  Str[] depends := Str[,]

  ** Output target type
  Str? targetType

  ** is debug mode
  Bool debug := false

  ** List of libraries to link in
  File[]? libs

  ** List of include
  File[]? includes

  ** List of source files or directories to compile
  File[] src := File[,]

  ** List of resource
  File[]? res := null

}