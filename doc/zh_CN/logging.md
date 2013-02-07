**************************************************************************
** Copyright (c) 2012-2013, chunquedong
** Licensed under the GNU LESSER GENERAL PUBLIC LICENSE version 3.0
**
** More info: https://bitbucket.org/chunquedong/fancpp
** Email: chunquedong_at_gmail_com
**************************************************************************

日志记录
============================

概述
-------
程序执行过程是不可见的，为了能直观的了解程序的运行状态，或者为了获取调试信息。经常需要打印日志信息。
printf是最原始的日志工具，但是输出内容不容易重定向，格式太随意，不便于结构化分析。cfan提供了日志框架来帮助统一处理日志。

使用
-------
cf_Log_log(tag, level, msg, ...) 用来统一记录日志，其中tag参数是自定义的字符串，用来过滤日志。
此外还有预定义的一些标准宏，如：cf_Log_cfDebug记录的日志在非Debug模式下将被忽略。

日志等级
-------
debug: log everything
info: log everything but debug
warn: log everything but debug, info
err: log only errors
silent: log nothing

日志监听器
-------
可以添加或者移除日志监听器自日志系统。
调用int cf_Log_addListener(cf_Log_listener listener)可以添加一个自定义的日志监听器。

