**************************************************************************
** Copyright (c) 2012-2013, chunquedong
** Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
**
** More info: https://bitbucket.org/chunquedong/fancpp
** Email: chunquedong_at_gmail_com
**************************************************************************

简介
============================

cfan是开源的ANSI C实现的基础库。讲究代码整洁，性能和跨平台。
包括内存检测，内存分配池，日志记录，堆栈跟踪，字符串处理，
动态数组，哈希表，链表，队列，单元测试，线程池，文件，流等, 缓存（不断增长中）。
cfan在纯C基础上采用面向对象和模版编程技术。

使用说明
============================

内存管理
-------
使用fc_malloc/cf_realloc/cf_free等代替标准的内存管理函数，在Debug下可以进行内存泄露检测和内存溢出检测等。
调用void cf_Memory_dumpMem()函数将打印未释放的内存和分配时的文件行数。
另外提供了加速内存分配的内存池。

日志记录
-------
cf_Log_log(tag, level, msg, ...) 用来统一记录日志，其中tag参数是自定义的字符串，用来过滤日志。
调用int cf_Log_addListener(cf_Log_listener listener)可以添加一个自定义的日志监听器。
cf_Log_cfDebug记录的日志在非Debug模式下将被忽略。

堆栈跟踪
-------
程序崩溃时打印函数调用堆栈能快速定位Bug，内存泄露时打印调用堆栈能定位泄露点。
也许某些调试程序能在Debug模式下进行堆栈跟踪，但是不能方便在程序中获得，有些时候也经常会跟踪出错。
在这里我们手动跟踪了函数调用堆栈，在函数进入时调用CF_ENTRY_FUNC，返回时调用CF_EXIT_FUNC。
调用cf_FuncTrace_printStackTrace()打印调用堆栈，调用cf_FuncTrace_printPerformance()打印性能统计。

字符串
------
StrBuf是可以动态扩增的字符串缓冲，可以进行快速的字符串连接操作。
Str包含对标准库补充的字符串处理函数。

通用容器
------
Array/ArrayList是动态数组容器，能通过索引快速访问。其中还实现了快速排序和二分查找算法。
HashMap/HashMap是哈希表，提供K-V对的快速访问。其中HashMap采用模版编程技术，最大限度提高效率。
Queue是固定容量的先进先出队列实现。
LinkedList是链表

单元测试
------
Test是简单的单元测试框架。
使用前调用cf_Test_init()初始化，使用cf_Test_add(name)添加一个单元测试用例。
调用void cf_Test_run(const char *nameFilter, const int attrFilter)运行测试用例。

多线程
------
BlockingQueue是一个消息队列，当在队列满时增加会被阻塞，当在队列为空时提取会阻塞。
Executor是采用生产者消费者模式的线程池。
Actor 是一个Actor模式的并发机制。

文件
-----
File是跨平台的文件和文件夹操作库。
Config用来读取简单的K=V配置文件。

流
-----
Object是所有对象的基类。
Stream定义了一个多态的流对象。

其他
------
LRU缓存
数学相关函数
时间相关函数


从源码构建
============================

cfan采用两个独立的构建系统，fmake和QMake，凭自己的爱好任选一个就行。

QMake
-------
QMake是Qt-Creator使用的构建系统。
可以使用qmake命令将.pro构建文件转化为GNUMakefile或者Microsoft Visual Studio工程文件等。
或者直接使用Qt-Creator打开fancpp.pro。

fmake
-------
fmake是新型的基于约定大于配置的C++/C构建系统，源码随cfan一起分发/fmake文件夹下。
fmake采用Fantom语言开发，所以需要先装Java和Fantom才能使用。
fmake的主要优点是解决了多个项目之间依赖的问题。之需要添加所依赖的库名和版本就能直接使用第三方库。
使用方法是用fan命名执行build.fan构建脚本即可。配置方法见/fmake/pod.fandoc文档。