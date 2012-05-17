SOURCES += main.cpp \
 netfs.cpp \
 basefs.cpp \
 nullfs.cpp \
 symlinkfs.cpp \
 badfs.cpp \
 rpcthread.cpp \
 mapservice.cpp \
 netclient.cpp \
 smbfs.cpp
TEMPLATE = app
CONFIG += warn_on \
	  thread \
          qt
TARGET = netfs
DESTDIR = ../bin
QT -= gui
DEFINES += _FILE_OFFSET_BITS=64

HEADERS += netfs.h \
basefs.h \
nullfs.h \
symlinkfs.h \
badfs.h \
 rpcthread.h \
 mapservice.h \
 netclient.h \
 smbfs.h
INCLUDEPATH += /usr/include/qtrpc2/


QT += network

LIBS += -lsmbclient \
  -lqtrpc2 \
  -lfuse

