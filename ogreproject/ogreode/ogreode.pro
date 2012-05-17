######################################################################
# Automatically generated by qmake (2.01a) Tue Jul 28 23:04:58 2009
######################################################################

include(../config.pri)
TEMPLATE = lib
DESTDIR = ../bin/
DEPENDPATH += .
INCLUDEPATH += .

CONFIG += shared debug

# Input
SOURCES += src/OgreOdeBody.cpp \
           src/OgreOdeCollision.cpp \
           src/OgreOdeDebugContact.cpp \
           src/OgreOdeDebugObject.cpp \
           src/OgreOdeEigenSolver.cpp \
           src/OgreOdeEntityInformer.cpp \
           src/OgreOdeGeometry.cpp \
           src/OgreOdeJoint.cpp \
           src/OgreOdeMaintainedList.cpp \
           src/OgreOdeMass.cpp \
           src/OgreOdePrecompiledHeaders.cpp \
           src/OgreOdeSpace.cpp \
           src/OgreOdeStepper.cpp \
           src/OgreOdeTriangleMeshData.cpp \
           src/OgreOdeTriangleMeshDataManager.cpp \
           src/OgreOdeUtility.cpp \
           src/OgreOdeWorld.cpp

# Input
HEADERS += include/OgreOde_Core.h \
           include/OgreOdeBody.h \
           include/OgreOdeCollision.h \
           include/OgreOdeDebugContact.h \
           include/OgreOdeDebugObject.h \
           include/OgreOdeEigenSolver.h \
           include/OgreOdeEntityInformer.h \
           include/OgreOdeFrameListener.h \
           include/OgreOdeGeometry.h \
           include/OgreOdeJoint.h \
           include/OgreOdeMaintainedList.h \
           include/OgreOdeMass.h \
           include/OgreOdePrecompiledHeaders.h \
           include/OgreOdePreReqs.h \
           include/OgreOdeSpace.h \
           include/OgreOdeStepper.h \
           include/OgreOdeTriangleMeshData.h \
           include/OgreOdeTriangleMeshDataManager.h \
           include/OgreOdeUtility.h \
           include/OgreOdeWorld.h

#tinyxml
HEADERS += tinyxml/tinystr.h \
		tinyxml/tinyxml.h
SOURCES += tinyxml/tinystr.cpp \
		tinyxml/tinyxml.cpp \
		tinyxml/tinyxmlerror.cpp \
		tinyxml/tinyxmlparser.cpp

#loader
HEADERS += loader/include/OgreOdeDotLoader.h \
           loader/include/OgreOde_Loader.h \
           loader/include/OgreOde_LoaderPreReqs.h
SOURCES += loader/src/OgreOdeDotLoader.cpp

#prefab
HEADERS += prefab/include/OgreOdeObject.h \
           prefab/include/OgreOdePrefabObject.h \
           prefab/include/OgreOdeRagdoll.h \
           prefab/include/OgreOdeVehicle.h \
           prefab/include/OgreOde_Prefab.h \
           prefab/include/OgreOde_PrefabPreReqs.h

SOURCES += prefab/src/OgreOdeObject.cpp \
           prefab/src/OgreOdePrefabObject.cpp \
           prefab/src/OgreOdeRagdoll.cpp \
           prefab/src/OgreOdeVehicle.cpp


INCLUDEPATH += include loader/include tinyxml prefab/include
win32 {
	LIBS += $${OGRE}/bin/release/OgreMain.dll \
		$${OGRE}/lib/libode.a
}
unix {
	LIBS += -lode \
		-lOgreMain
	INCLUDEPATH +=  /usr/include/OGRE/ \
			/usr/include/ode/
}
