SOURCES += main.cpp \
    mainwindow.cpp \
    logwidget.cpp \
    channelwidget.cpp \
    channelbase.cpp \
    channelmodel.cpp \
    commandedit.cpp \
 passworddialog.cpp \
 nickdialog.cpp \
 privatewidget.cpp
TEMPLATE = app
CONFIG += warn_on \
    thread \
    qt \
 debug
TARGET = mobileirc
DESTDIR = ../bin
QT += network
LIBS += -lqxtirc
unix : INCLUDEPATH += /usr/include/qxtirc/
win32 {
    INCLUDEPATH += ../../qxtirc/qxtirc/

    # INCLUDEPATH += "E:/Program Files/Microsoft Visual Studio 8/SmartDevices/SDK/PocketPC2003/Include"
    LIBS += -L../../qxtirc/qxtirc/debug/
    LIBS += -lcellcore
}
FORMS += MainWindow.ui \
    LogWidget.ui \
    ChannelWidget.ui \
 PasswordEntry.ui \
 NickDialog.ui
HEADERS += mainwindow.h \
    logwidget.h \
    channelwidget.h \
    channelbase.h \
    channelmodel.h \
    commandedit.h \
 passworddialog.h \
 nickdialog.h \
 privatewidget.h
#RESOURCES += Icons.qrc
CONFIG -= release

