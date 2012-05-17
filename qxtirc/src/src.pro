SOURCES += main.cpp \
 ircbot.cpp
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt
TARGET = qxtirc
DESTDIR = ../bin
QT -= gui
INCLUDEPATH += ../qxtirc
DEPENDPATH += ../qxtirc

LIBS += -L../qxtirc/ \
  -lqxtirc

win32 {
    LIBS += -L../qxtirc/debug/
}

unix {
    TARGETDEPS += ../qxtirc/libqxtirc.so
}

HEADERS += ircbot.h

