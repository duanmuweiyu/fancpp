**************************************************************************
** Copyright (c) 2012-2013, chunquedong
** Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
**
** More info: https://bitbucket.org/chunquedong/fancpp
** Email: chunquedong_at_gmail_com
**************************************************************************

通用容器
============================

概述
------------
cfan提供了Array动态数组和HashMap哈希表，因为这两个容器是最高效，最常用的。
暂时没有提供链表和树状数据结构。
这两个容器为了获得更好的性能采用模版技术实现，所以使用前先需要根据模版宏定义自己的类型。

Array定义
------------
Array采用模版技术实现，使用前需要先定义。
如定义一个const char*型的Array名称为MyArray，需要在头文件增加cf_ArrayTemplate(MyArray, const char*)
在实现文件中增加cf_ArrayTemplate_impl(MyArray, const char*)

系统预定义了一下类型的Array，有int型的cf_ArrayI, 字符串型的cf_ArrayS, 指针型的cf_ArrayP等。

HashMap定义
------------
HashMap提供快速的K-V对访问。定义方法类似与Array，当需要用户自定义哈希函数和比较函数。
这里举一个定义int->int的例子：

头文件：
    cf_HashMapTemplate(cf_HashMapII, int, int)

实现文件：
    #define cf_hashFunc(key) key
    #define cf_compFunc(v1, v2) ((v1)-(v2))

    cf_HashMapTemplate_impl(cf_HashMapII, int, int)

    #undef cf_hashFunc
    #undef cf_compFunc

用法
------------
一旦定义好就很容易使用的。具体的使用方法见cfTest下的测试用例:ArrayTest.c, HashMap.c.

快速排序和二分查找
--------------
标准中的qsort和bsearch采用函数指针回调的方法，函数不能内联，导致性能不佳。
这一点和C++的std::sort比起来常常被作为把柄嘲笑。
所以cfan采用模版技术重新实现了这两个函数。

性能
------------
经过和C++ STL库比较（测试代码见cfTest/PerformanceTest.cpp），得出如下结论。
Array的性能和std::vecotr持平，或者略好。
HashMap的性能好于std::map和stdext::hash_map。
sort的性能不如std::sort稳定（不得不承认std::sort的优化还是很给力的）。

