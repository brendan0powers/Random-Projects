# config.pri should declare OGRE... Example:
# OGRE = C:/OgreSDK
include(../shared/shared.pri)
include(../config.pri)
DEFINES += CLIENT_SIDE

TEMPLATE = app

TARGET = client

DESTDIR = ../bin/

SOURCES += main.cpp \
 OgreQuake3Level.cpp \
 OgreQuake3Shader.cpp \
 OgreQuake3ShaderManager.cpp \
 application.cpp \
 ogreqtoverlay.cpp \
 agentitem.cpp \
 testform.cpp \
 consoledialog.cpp \
 commandedit.cpp

HEADERS += application.h \
 ogreqtoverlay.h \
 agentitem.h \
 testform.h \
 consoledialog.h \
 commandedit.h

QT += opengl \
 webkit

CONFIG -= release
CONFIG += debug


unix {
    INCLUDEPATH += /usr/include/OIS/ \
		/usr/include/OGRE/


    DEFINES += CLIENT_SIDE

    LIBS += -L../bin/ \
  -ltorque \
  -logreode \
  -lode \
  -lOIS \
  -lOgreMain

}
win32 {
    LIBS += $${OGRE}/bin/release/OgreMain.dll \
        $${OGRE}/lib/libode.a \
        $${OGRE}/bin/release/OIS.dll \
        ../bin/ogreode.dll \
        ../bin/torque.dll

    INCLUDEPATH += $${OGRE}/include/
}

QMAKE_CXXFLAGS_DEBUG += -fpermissive
QMAKE_CXXFLAGS_RELEASE += -fpermissive





DEFINES += TNL_DEBUG \
        TNL_ENABLE_LOGGING
FORMS += TestForm.ui \
ConsoleDialog.ui
INCLUDEPATH += ../torque \
  . \
  ../shared \
  ../includes/ \
  ../ogreode/include/ \
  ../ogreode/loader/include/ \
  ../ogreode/prefab/include/

