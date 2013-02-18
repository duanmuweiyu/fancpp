**************************************************************************
** Copyright (c) 2012-2013, chunquedong
** Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
**
** More info: https://bitbucket.org/chunquedong/fancpp
** Email: chunquedong_at_gmail_com
**************************************************************************

内存管理
============================

概述
-------
检查内存泄露检测和内存溢出等。

用法
-------
使用cf_malloc/cf_realloc/cf_free等代替标准的内存管理函数，使用方法和标准库对应的函数完全一样。
调用void cf_Memory_dumpMem()函数将打印未释放的内存和对应的分配时文件行数。
如果同时开启了堆栈跟踪机制，可以打印泄露内存分配时的调用堆栈。

受检内存分配
-------
cf_checkedMalloc/cf_checkedRealloc系列函数会在内存分配失败后终止程序，而不是返回NULL。

内存分配池
-------
在大量分配小对象时使用内存池能提供内存分配的效率，减少内存碎片。
使用cf_MemoryPool是一个简单的固定大小内存池。

其他
-------
内存检查机制默认在Debug模式下有效。

