include(../shared/shared.pri)
include(../config.pri)
DEFINES += SERVER_SIDE

TEMPLATE = app

TARGET = server

DESTDIR = ../bin/

SOURCES += main.cpp

CONFIG -= release
CONFIG += debug console


unix {

INCLUDEPATH += /usr/include/OIS/ \
                /usr/include/OGRE/

    LIBS += -L../bin/ \
		-lOgreMain \
		-ltorque


}
win32 {
    LIBS += $${OGRE}/bin/release/OgreMain.dll \
        ../bin/torque.dll

    INCLUDEPATH += C:/OgreSDK/include/
}

QMAKE_CXXFLAGS_DEBUG += -fpermissive
QMAKE_CXXFLAGS_RELEASE += -fpermissive



INCLUDEPATH += ../shared \
  ../torque \
  ../includes/

DEFINES += TNL_DEBUG \
        TNL_ENABLE_LOGGING
