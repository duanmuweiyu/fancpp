QT       -= core gui

TARGET = cppfan
TEMPLATE = lib
CONFIG   += staticlib
VERSION = 1.0.0

QMAKE_CC += -std=c99
#QMAKE_CC += -O3

DEFINES += DEBUG

HEADERS += \
    cppfan/String.h \
    cppfan/Object.h \
    cppfan/LinkedList.h \
    cppfan/HashMap.h \
    cppfan/common.h \
    cppfan/AutoPtr.h \
    cppfan/Array.h \
    cppfan/Test.h \
    cppfan/cppfan.h

SOURCES += \
    cppfan/String.cpp \
    cppfan/Object.cpp

LIBS += -pthread

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../cfan/release/ -lcfan
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../cfan/debug/ -lcfan
else:symbian: LIBS += -lcfan
else:unix: LIBS += -L$$OUT_PWD/../cfan/ -lcfan

unix: LIBS += -lrt

INCLUDEPATH += $$PWD/../cfan
DEPENDPATH += $$PWD/../cfan
