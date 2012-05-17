SOURCES += main.cpp \
testbot.cpp
HEADERS += testbot.h

TEMPLATE = app

CONFIG -= release

CONFIG += debug

QT -= gui

INCLUDEPATH += ../qxtirc
DEPENDPATH += ../qxtirc

LIBS += -L../qxtirc \
  -lqxtirc

win32 {
    LIBS += -L../qxtirc/debug/
}

unix {
    TARGETDEPS += ../qxtirc/libqxtirc.so
}

TARGET = ../bin/testbot

