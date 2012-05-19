SOURCES += main.cpp \
 QOpenCVWidget.cpp \
 mainwindow.cpp
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt \
 debug
TARGET = opencvtest
DESTDIR = ../bin


QT += network


FORMS += MainWindow.ui

HEADERS += QOpenCVWidget.h \
mainwindow.h
CONFIG -= release

INCLUDEPATH += /usr/lib/glib-2.0/include/ \
  /usr/include/glib-2.0/ \
  /usr/include/unicap/ \
  /usr/include/opencv/

LIBS += -lgobject-2.0 \
  -lglib-2.0 \
  -lucil \
  -lunicap \
  -lhighgui \
  -lcv

