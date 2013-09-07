**************************************************************************
** Copyright (c) 2012-2013, chunquedong
** Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
**
** More info: https://bitbucket.org/chunquedong/fancpp
** Email: chunquedong_at_gmail_com
**************************************************************************

并发
============================

概述
-------
cfan对并发编程提供了支持，主要功能是阻塞队列，线程池和Actor模式。
自定义的内存分配函数cf_malloc/cf_realloc/cf_free默认也是线程安全的。
cfan使用第三方库tinycthread代替pthread接口，以实现跨平台。tinycthread是C11标准的一个现实。

线程池
-------
Executor是采用生产者消费者模式的线程池, 调用Executor的addTask方法将任务添加到线程池，任务会异步的执行。

Actor模式
-------
Actor是一个Actor模式的并发机制，对象之间通过发送消息进行通信。
Acotr模式从较高层次上指导并发编程，将人类从复杂容易出错的多线程编程中解脱。
Actor并不是运行在一个线程，而是运行在一个背后的线程池，
但Acotr会确保发送给它的消息按照顺序执行，从而看起来像是Actor独立运行在一个线程。

Actor模式规范，即：
  1.通过发送消息进行通信，而不是直接调用；
  2.消息体是不可变的，不通过任何方式共享可变数据，必要时进行拷贝后传递；
如果应用程序能保证遵守Acotr模式规范，那么程序将是完全线程安全的。

使用的方法是在Actor处给出receive方法的函数指针。通过send发送消息后，在receive函数进行消息处理。
消息的name约定为要调用的方法名，arg约定为调用参数，当然你可以把他们挪作他用。