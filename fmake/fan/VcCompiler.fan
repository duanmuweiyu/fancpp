//
// Copyright (c) 2010, chunquedong
// Licensed under the Academic Free License version 3.0
//
// History:
//   2011-4-3  Jed Young  Creation
//

using build

**
** MS VC++ compiler
**
class VcCompiler : CppCompiler
{

//////////////////////////////////////////////////////////////////////////
// Construction
//////////////////////////////////////////////////////////////////////////

  new make(BuildScript script)
    : super(script)
  {
  }

//////////////////////////////////////////////////////////////////////////
// Compile
//////////////////////////////////////////////////////////////////////////

  **
  ** compile code to .obj
  **
  override Void compile()
  {
      // build command
      ccExe := ccHome.toUri.toFile + `bin/cl.exe`
      cmd := [ccExe.osPath]

      // default paramaters
      cmd.add("/c")       //no link
      cmd.add("/EHsc")    //suppert exception
      cmd.add("/nologo")  //no logo

      cmd.add(debug ? "/Od" : "/O2") //optimizing
      if (debug) cmd.add("/Ob2")     //inline

      //there is ms vc
      cmd.add("/DWIN32")
      cmd.add("/D_WINDOWS")

      //debug define
      cmd.add(debug ? "/D_DEBUG": "/DNDEBUG")

      //compile mode
      cmd.add(compileMode)

      //include
      allIncludes.each
      {
        cmd.add("/I${it.osPath}")
      }

      //output
      objDir := (outPodDir + `obj/`).create
      objOut := objDir.osPath + "\\"
      cmd.add("/Fo$objOut")

      // src files/dirs
      srcCodes.each { cmd.add(it.osPath) }

      //run
      Exec(script, cmd).run
  }


  private Str compileMode()
  {/*
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
      */
      return (debug? "/MDd": "/MD")
  }

//////////////////////////////////////////////////////////////////////////
// Link
//////////////////////////////////////////////////////////////////////////

  **
  ** link objs and output .exe or .dll
  **
  override Void link(Bool isDll)
  {
      // build command
      linkExe := ccHome.toUri.toFile + `bin/link.exe`
      cmd := [linkExe.osPath]

      //version
      cmd.add("/VERSION:$version")

      //no logo
      cmd.add("/NOLOGO")

      //lib Paths
      allLibPaths.each
      {
        cmd.add("/LIBPATH:${it.osPath}")
      }

      //DLL
      if(outType == TargetType.dll)
      {
        cmd.add("/DLL")
      }

      //output
      cmd.add("/OUT:${output.osPath}")

      // objs files
      objFiles.each { cmd.add(it.osPath) }

      //depends libs
      allLibs.each { cmd.add(it) }

      //run
      Exec(script, cmd).run
  }

  **
  ** create a .lib file
  **
  override Void makeLib()
  {
      libExe := ccHome.toUri.toFile + `bin/lib.exe`
      cmd := [libExe.osPath]

      //no logo
      cmd.add("/NOLOGO")

      //lib Paths
      allLibPaths.each
      {
        cmd.add("/LIBPATH:${it.osPath}")
      }

      //output
      cmd.add("/OUT:${output.osPath}")

      // objs files
      objFiles.each { cmd.add(it.osPath) }

      //TODO: add metaData and remove static link
      //depends libs
      allLibs.each { cmd.add(it) }

      //run
      Exec(script, cmd).run
  }
}