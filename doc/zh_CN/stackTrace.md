**************************************************************************
** Copyright (c) 2012-2013, chunquedong
** Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
**
** More info: https://bitbucket.org/chunquedong/fancpp
** Email: chunquedong_at_gmail_com
**************************************************************************

堆栈跟踪
============================

概述
-------
程序崩溃时打印函数调用堆栈能快速定位Bug，内存泄露时打印调用堆栈能定位泄露点。
也许某些调试程序能在Debug模式下进行堆栈跟踪，但是不能方便在程序中获得，有些时候也经常会跟踪出错。
以往的堆栈跟踪都借助于调试器，经常不靠谱。cfan解决了这一难题。

用法
-------
手动跟踪了函数调用堆栈，需要在函数进入时调用CF_ENTRY_FUNC，返回时调用CF_EXIT_FUNC。
此处需要注意函数会有多个return时，每个return都需要添加，例如：

    int foo() {
      CF_ENTRY_FUNC
      ...
      CF_EXIT_FUNC return 1;
      ...
      CF_EXIT_FUNC return 2;
    }
当使用C++时，还可以利用CF_CPP_FUNCTRACE析构函数自动调用退出函数，例如：
    int foo() {
      CF_CPP_FUNCTRACE
      ...
      return 1;
    }

打印调用堆栈
--------
可以在任何时候调用cf_FuncTrace_printStackTrace()打印调用堆栈。
如果想在系统出错推出的时候打印，则之需要在系统启动时调用cf_FuncTrace_traceOnExit()函数，系统在退出时会自动打印调用堆栈。
这在系统崩溃的情况下非常有用，可以直接看出出错位置。

性能统计
--------
cfan在跟踪调用堆栈的时候顺便统计了一下每个函数的调用时间和调用次数。这些信息非常有利于剖析程序性能。
调用cf_FuncTrace_printPerformance()打印性能统计。

其他
--------
堆栈跟踪机制默认只在Debug模式下有效。
为了不影响内存检测机制的运行，可以使用cf_FuncTrace_dispose()来清空堆栈跟踪机制所占用的内存。

