**************************************************************************
** Copyright (c) 2012-2013, chunquedong
** Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
**
** More info: https://bitbucket.org/chunquedong/fancpp
** Email: chunquedong_at_gmail_com
**************************************************************************

编码约定
============================

命名规范
-------

1. 采用类似于Java的三段式命名法，混合Pascal，Camel和下划线命名。例如：
    cf_BlockingQueue_isEmpty
第一段是命名空间，第二段是类名，第三段是方法或者字段。

 * 类型使用大写驼峰式，例如“FooBar”
 * 成员（包括方法和字段，甚至常量字段）使用小写驼峰式，例如“fooBar”
 * 命名空间使用小写驼峰式

2. 宏（Macro）命名不采用全大写命名方法，当函数类似与函数或者变量时采用函数或者变量的命名规范。
只有当需要特别醒目的，且用法又不像函数或者变量时才采用全大写。

3. 拒绝一切匈牙利命名法，不要使用任何性质的前缀或者后缀。不要缩写单词（很长的单词除外）。
4. 更喜欢readXml而不是readXML。
5. 方法第一个参数名约定为self。

其他代码风格
----------
1. 括号后不换行，采用紧凑型编码风格。
2. 参数之间、符号和标示符个之间都加一个空格。
3. 采用两空格缩进，而不是tab。
4. 源文件采用UTF-8编码。
5. 构造函数约定为make，析构函数约定为dispose。
6. 私有成员以下划线结尾。
7. 每个类放在一个文件中，类的结构体定义和函数定义放在一起。头文件和实现文件同名。

注释约定
----------
文件头注释：
    /*
     * Copyright (c) 2012-2013, chunquedong
     *当然你可以把他们挪作他用
     * This file is part of fancpp project
     * Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
     *
     * History:
     *   2013-1-19  Jed Young  Creation
     */
文档注释:
    /**
     * something.
     */
块注释：
    /*======================================================================*/

其他注释使用/**/或者//



