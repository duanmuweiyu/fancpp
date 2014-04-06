QT       -= core gui

TARGET = cfan
TEMPLATE = lib
CONFIG   += staticlib
VERSION = 1.0.0

QMAKE_CC += -std=c99
#QMAKE_CC += -O3

DEFINES += CF_EXPORT
DEFINES += DEBUG

HEADERS += \
    cfan/Stream.h \
    cfan/StrBuf.h \
    cfan/Object.h \
    cfan/miss.h \
    cfan/Memory.h \
    cfan/macro.h \
    cfan/Log.h \
    cfan/HashMap.h \
    cfan/FileStream.h \
    cfan/File.h \
    cfan/Error.h \
    cfan/Config.h \
    cfan/cfan.h \
    cfan/Array.h \
    cfan/Str.h \
    cfan/Test.h \
    tinyCThread/tinycthread.h \
    cfan/Queue.h \
    cfan/Executor.h \
    cfan/BlockingQueue.h \
    cfan/Sort.h \
    cfan/FuncTrace.h \
    cfan/MemoryPool.h \
    cfan/LinkedList.h \
    cfan/Actor.h \
    cfan/StackTrace.h \
    cfan/Trace.h \
    cfan/Cache.h \
    cfan/FileLogListener.h \
    cfan/ArrayList.h \
    cfan/System.h \
    cfan/Math.h \
    cfan/Util.h \
    cfan/HashTable.h

SOURCES += \
    cfan/StrBuf.c \
    cfan/Memory.c \
    cfan/Log.c \
    cfan/HashMap.c \
    cfan/FileStream.c \
    cfan/File.c \
    cfan/Config.c \
    cfan/Array.c \
    cfan/File_unix.c \
    cfan/File_win.c \
    cfan/Test.c \
    tinyCThread/tinycthread.c \
    cfan/Executor.c \
    cfan/Sort.c \
    cfan/FuncTrace.c \
    cfan/MemoryPool.c \
    cfan/Actor.c \
    cfan/StackTrace.c \
    cfan/Trace.c \
    cfan/Cache.c \
    cfan/FileLogListener.c \
    cfan/ArrayList.c \
    cfan/System.c \
    cfan/Str.c \
    cfan/HashTable.c \
    cfan/Object.c \
    cfan/LinkedList.c



