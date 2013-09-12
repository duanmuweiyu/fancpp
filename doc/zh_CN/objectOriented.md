**************************************************************************
** Copyright (c) 2012-2013, chunquedong
** Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
**
** More info: https://bitbucket.org/chunquedong/fancpp
** Email: chunquedong_at_gmail_com
**************************************************************************

面向对象
============================

个人理解，面向对象更多的是分析问题，看待问题的方法。与具体语言无关，C语言照样可以面向对象编程。
面向对象三要素（封装，继承，多态），我觉得只有封装才是本质，其他都是可选的。
在C中多态会把代码弄的很乱，所以不要轻易使用。

面向对象
-----
cfan通过函数命名约定来实现面向对象，即例如：
  cf_Executor_addTask(self)是Executor类的addTask方法，'this'指针约定名称为self。

继承
-----
如果需要虚方法，声明一个函数指针即可。如果多个虚方法，需要增加一个虚表（VTable）。
如果真的需要虚表，cfan提供了Object对象，作为所有类的基类来使用。Object对象的头部会放置一个虚表。

继承的写法如下：
  struct pf_Node_ {
    cf_Object super;
    ...
  };
默认约定基类名称为super。


多继承
-----
如果是接口继承可以合并虚表，使用对象时同时分开传递虚表和对象本身，内存布局如下：
  vtab* | data
    |
    V
  Vtab1 | Vtab2

如果不是接口继承，在虚表里面标明相对于起始位置的偏移，内存布局可能如下：
  vtab1* | data1 | vtab2* | data2
    |               |
    V               V
  Vtab1            Vtab2
  offset=0         offset=sizeof(data1)