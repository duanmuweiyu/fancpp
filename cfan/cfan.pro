QT       -= core gui

TARGET = cfan
TEMPLATE = lib
CONFIG   += staticlib
VERSION = 1.0.0

QMAKE_CC += -std=c99

DEFINES += CF_EXPORT

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
    cfan/Array.h

SOURCES += \
    cfan/StrBuf.c \
    cfan/Memory.c \
    cfan/Log.c \
    cfan/HashMap.c \
    cfan/FileStream.c \
    cfan/File.c \
    cfan/Config.c \
    cfan/Array.c



