
TARGET   =  cfTest
TEMPLATE = app
CONFIG += console
CONFIG -= qt


QMAKE_CC += -std=c99 -v
#QMAKE_CC += -O3
QMAKE_CC += -rdynamic

DEFINES += DEBUG

SOURCES += \
    cfTest/StrBufTest.c \
    cfTest/MemoryTest.c \
    cfTest/main.c \
    cfTest/LogTest.c \
    cfTest/HashMapTest.c \
    cfTest/ConfigTest.c \
    cfTest/ArrayTest.c \
    cfTest/FileTest.c \
    cfTest/StrTest.c \
    cfTest/QueueTest.c \
    cfTest/ExecutorTest.c \
    cfTest/BlockingQueueTest.c \
    cfTest/MemoryPoolTest.c \
    cfTest/LinkedListTest.c \
    cfTest/ActorTest.c \
    cfTest/CacheTest.c \
    cfTest/FileLogListenerTest.c \
    cfTest/ArrayListTest.c \
    cfTest/HashTableTest.c \
    cfTest/LinkedQueueTest.c \
    cfTest/TimerTest.c

LIBS += -pthread

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../cfan/release/ -lcfan
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../cfan/debug/ -lcfan
else:symbian: LIBS += -lcfan
else:unix: LIBS += -L$$OUT_PWD/../cfan/ -lcfan

unix: LIBS += -lrt
macx: LIBS -= -lrt

INCLUDEPATH += $$PWD/../cfan
DEPENDPATH += $$PWD/../cfan

OTHER_FILES +=
