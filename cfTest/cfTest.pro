TEMPLATE = app
CONFIG += console
CONFIG -= qt

QMAKE_CC += -std=c99

SOURCES += \
    cfTest/StrBufTest.c \
    cfTest/MemoryTest.c \
    cfTest/main.c \
    cfTest/LogTest.c \
    cfTest/HashMapTest.c \
    cfTest/ConfigTest.c \
    cfTest/ArrayTest.c



win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../cfan/release/ -lcfan
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../cfan/debug/ -lcfan
else:symbian: LIBS += -lcfan
else:unix: LIBS += -L$$OUT_PWD/../cfan/ -lcfan

INCLUDEPATH += $$PWD/../cfan
DEPENDPATH += $$PWD/../cfan
