TEMPLATE = lib

CONFIG += dll \
debug
CONFIG -= release \
exceptions \
stl \
thread
QT -= gui

HEADERS += qxtircconnection.h \
 qxtircmessage.h \
 qxtircnick.h \
 qxtircchannel.h \
 qxtircbotmessage.h \
 qxtircconnection_p.h \
 qxtircchannel_p.h \
 qxtircbotmessage_p.h \
 qxtircmessage_p.h \
 qxtircnick_p.h

SOURCES += qxtircconnection.cpp \
 qxtircmessage.cpp \
 qxtircnick.cpp \
 qxtircchannel.cpp \
 qxtircbotmessage.cpp

QT += network

