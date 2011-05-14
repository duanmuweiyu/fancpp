//
// Copyright (c) 2010, chunquedong
// Licensed under the Academic Free License version 3.0
//
// History:
//   2011-4-2  Jed Young  Creation
//

using build

abstract class BuildCpp : BuildScript
{

//////////////////////////////////////////////////////////////////////////
// Pod Meta-Data
//////////////////////////////////////////////////////////////////////////

  **
  ** Required name of the lib.
  **
  Str? name

  **
  ** description of pod
  **
  Str? summary

  **
  ** Required output type. Possible values are 'exe','dll','lib'
  **
  TargetType? outType

  **
  ** is debug mode
  **
  Bool debug := false

  **
  ** default following fantom version
  **
  Version version := Version(config("buildVersion", "0"))

  **
  ** msvc or gcc
  **
  Str compiler := "msvc"

  **
  ** depends lib
  **
  Str[] depends := [,]

  **
  ** Required list of directories to compile.  All Cpp source
  ** files in each directory will be compiled.
  **
  Uri[] srcDirs := [`cpp/`]

  **
  ** List of ext libraries to link to.
  **
  Str[] extLibs := [,]

  **
  ** List of ext include the head file
  **
  Uri[] extIncludeDirs := [,]

  **
  ** res will be copy to output directly
  **
  Uri[]? resDirs := null

  **
  ** output
  **
  Uri outDir := Env.cur.workDir.plus(`lib/cpp/`).uri


//////////////////////////////////////////////////////////////////////////
// Validate
//////////////////////////////////////////////////////////////////////////

  private Void validate()
  {
    if (name == null) throw fatal("Must set name")
    if (outType == null) throw fatal("Must set outType")
    if (summary == null) throw fatal("Must set summary")
  }

//////////////////////////////////////////////////////////////////////////
// Dump Env
//////////////////////////////////////////////////////////////////////////

  override Void dumpEnv()
  {
    super.dumpEnv

    oldLevel := log.level
    log.level = LogLevel.silent

    try
      log.out.printLine("  cppcHome:    ${VcCompiler(this).ccHome}")
    catch (Err e)
      log.out.printLine("  cppcHome:    $e")
    finally
      log.level = oldLevel
  }

//////////////////////////////////////////////////////////////////////////
// Compile
//////////////////////////////////////////////////////////////////////////

  **
  ** Compile C++ source into exe or dll
  **
  @Target { help = "Compile C++ source into exe or dll" }
  Void compile()
  {
    validate

    log.info("compile [${scriptDir.name}]")
    log.indent

    if (compiler == "msvc")
    {
      // compile source
      cc := VcCompiler(this)
      {
        it.outHome    = this.outDir.toFile
        it.outType = this.outType
        it.debug      = this.debug

        it.name       = this.name
        it.summary    = this.summary
        it.depends    = this.depends.map |s->Depend| { Depend.fromStr(s) }
        it.version    = this.version

        it.src        = this.resolveDirs(srcDirs)
        it.extLibs    = this.extLibs
        it.extIncludes   = this.resolveDirs(extIncludeDirs)

        if(resDirs != null)
        {
          it.res = this.resolveDirs(resDirs)
        }
      }
      cc.run
    }
    else if(compiler == "gcc")
    {
      //TODO
      log.info("TODO: GCC")
    }
    else
    {
      log.info("unsupperted compiler")
    }

    log.unindent
  }

//////////////////////////////////////////////////////////////////////////
// Clean
//////////////////////////////////////////////////////////////////////////

  **
  ** Delete all intermediate and target files
  **
  @Target { help = "Delete all intermediate and target files" }
  Void clean()
  {
    log.info("clean [${scriptDir.name}]")
    log.indent
    Delete(this, outDir.toFile+`$name-$version/`).run
    log.unindent
  }

//////////////////////////////////////////////////////////////////////////
// Full
//////////////////////////////////////////////////////////////////////////

  **
  ** Run clean, compile
  **
  @Target { help = "Run clean, compile" }
  Void full()
  {
    clean
    compile
  }

//////////////////////////////////////////////////////////////////////////
// Utils
//////////////////////////////////////////////////////////////////////////

  **
  ** Resolve a set of URIs to files relative to scriptDir.
  **
  internal File[] resolveFiles(Uri[] uris)
  {
    uris.map |uri->File|
    {
      f := scriptDir + uri
      if (!f.exists || f.isDir) throw fatal("Invalid file: $uri")
      return f
    }
  }

  **
  ** Resolve a set of URIs to directories relative to scriptDir.
  **
  internal File[] resolveDirs(Uri[] uris)
  {
    uris.map |uri->File|
    {
      f := scriptDir + uri
      if (!f.exists || !f.isDir) throw fatal("Invalid dir: $uri")
      return f
    }
  }

}

**************************************************************************
** target Type
**************************************************************************

** output type
enum class TargetType
{
  ** executable file
  exe,

  ** dynamic link library
  dll,

  ** static link library
  lib
}