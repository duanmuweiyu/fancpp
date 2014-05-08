
TARGET   =  cppfTest
TEMPLATE = app
CONFIG += console
CONFIG -= qt


QMAKE_CC += -std=c99
#QMAKE_CC += -O3


DEFINES += DEBUG

SOURCES += \
    cppfTest/ArrayTest.cpp \
    cppfTest/HashMapTest.cpp \
    cppfTest/LinkedListTest.cpp \
    cppfTest/maincpp.cpp \
    cppfTest/MemoryTest.cpp \
    cppfTest/AutoPtrTest.cpp \
    cppfTest/StringTest.cpp

LIBS += -pthread

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../cfan/release/ -lcfan
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../cfan/debug/ -lcfan
else:symbian: LIBS += -lcfan
else:unix: LIBS += -L$$OUT_PWD/../cfan/ -lcfan
INCLUDEPATH += $$PWD/../cfan
DEPENDPATH += $$PWD/../cfan

unix: LIBS += -lrt

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../cppfan/release/ -lcppfan
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../cppfan/debug/ -lcppfan
else:symbian: LIBS += -lcppfan
else:unix: LIBS += -L$$OUT_PWD/../cppfan/ -lcppfan
INCLUDEPATH += $$PWD/../cppfan
DEPENDPATH += $$PWD/../cppfan

OTHER_FILES +=
