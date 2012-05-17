/***************************************************************************
 *   Copyright (C) 2009 by Brendan Powers   *
 *   brendan@resara.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QApplication>
#include <QDebug>
#include <QStringList>
#include "Ogre.h"
#include "OgreOde_Core.h"
#include "ode/ode.h"
#include "ode/collision_trimesh.h"
#include "OgreQuake3Level.h"
#include "application.h"
#include <sstream>
#include "testconnection.h"
#include "ExampleFrameListener.h"
#include "ExampleLoadingBar.h"
#include "ExampleApplication.h"

#define CUSTOM_SHININESS 1
#define CUSTOM_DIFFUSE 2
#define CUSTOM_SPECULAR 3

/*

struct Vert
{
	float x;
	float y;
	float z;
	float w;
};

enum ObjectType
{
	SphereType=101,
	CubeType=102
};

enum WeaponType
{
	CubeGun = 0,
	GravGun
};

void createSphere(const std::string& strName, const float r, const int nRings = 16, const int nSegments = 16)
{
	MeshPtr pSphere = MeshManager::getSingleton().createManual(strName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	SubMesh *pSphereVertex = pSphere->createSubMesh();

	pSphere->sharedVertexData = new VertexData();
	VertexData* vertexData = pSphere->sharedVertexData;

	// define the vertex format
	VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
	size_t currOffset = 0;
	// positions
	vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_POSITION);
	currOffset += VertexElement::getTypeSize(VET_FLOAT3);
	// normals
	vertexDecl->addElement(0, currOffset, VET_FLOAT3, VES_NORMAL);
	currOffset += VertexElement::getTypeSize(VET_FLOAT3);
	// two dimensional texture coordinates
	vertexDecl->addElement(0, currOffset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);
	currOffset += VertexElement::getTypeSize(VET_FLOAT2);

	// allocate the vertex buffer
	vertexData->vertexCount = (nRings + 1) * (nSegments+1);
	HardwareVertexBufferSharedPtr vBuf = HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), vertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	VertexBufferBinding* binding = vertexData->vertexBufferBinding;
	binding->setBinding(0, vBuf);
	float* pVertex = static_cast<float*>(vBuf->lock(HardwareBuffer::HBL_DISCARD));

	// allocate index buffer
	pSphereVertex->indexData->indexCount = 6 * nRings * (nSegments + 1);
	pSphereVertex->indexData->indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_16BIT, pSphereVertex->indexData->indexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	HardwareIndexBufferSharedPtr iBuf = pSphereVertex->indexData->indexBuffer;
	unsigned short* pIndices = static_cast<unsigned short*>(iBuf->lock(HardwareBuffer::HBL_DISCARD));

	float fDeltaRingAngle = (Math::PI / nRings);
	float fDeltaSegAngle = (2 * Math::PI / nSegments);
	unsigned short wVerticeIndex = 0 ;

	// Generate the group of rings for the sphere
	for( int ring = 0; ring <= nRings; ring++ ) {
		float r0 = r * sinf (ring * fDeltaRingAngle);
		float y0 = r * cosf (ring * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for(int seg = 0; seg <= nSegments; seg++) {
			float x0 = r0 * sinf(seg * fDeltaSegAngle);
			float z0 = r0 * cosf(seg * fDeltaSegAngle);

			// Add one vertex to the strip which makes up the sphere
			*pVertex++ = x0;
			*pVertex++ = y0;
			*pVertex++ = z0;

			Vector3 vNormal = Vector3(x0, y0, z0).normalisedCopy();
			*pVertex++ = vNormal.x;
			*pVertex++ = vNormal.y;
			*pVertex++ = vNormal.z;

			*pVertex++ = (float) seg / (float) nSegments;
			*pVertex++ = (float) ring / (float) nRings;

			if (ring != nRings) {
                               // each vertex (except the last) has six indices pointing to it
				*pIndices++ = wVerticeIndex + nSegments + 1;
				*pIndices++ = wVerticeIndex;
				*pIndices++ = wVerticeIndex + nSegments;
				*pIndices++ = wVerticeIndex + nSegments + 1;
				*pIndices++ = wVerticeIndex + 1;
				*pIndices++ = wVerticeIndex;
				wVerticeIndex ++;
			}
		}; // end for seg
	} // end for ring

	// Unlock
	vBuf->unlock();
	iBuf->unlock();
	// Generate face list
	pSphereVertex->useSharedVertices = true;

	// the original code was missing this line:
	pSphere->_setBounds( AxisAlignedBox( Vector3(-r, -r, -r), Vector3(r, r, r) ), false );
	pSphere->_setBoundingSphereRadius(r);
        // this line makes clear the mesh is loaded (avoids memory leaks)
	pSphere->load();
}


float bounce = 0.1;
WeaponType weapon = CubeGun;
TestConnection *conn;


void rayColideCallback(void *data, dGeomID g1, dGeomID g2)
{
	dContactGeom *curcontact = (dContactGeom *)data;

	if ((dGeomGetClass(g1) == dRayClass) || (dGeomGetClass(g2) == dRayClass))
	{
		dContactGeom contact;
		if (dCollide(g1, g2, 1, &contact, sizeof(dContactGeom)) == 1)
		{
			if (contact.depth < curcontact->depth)
			{
				*curcontact = contact;
			}
		}
	}

	return(false);
}

dContactGeom rayCollide(dSpaceID space, Vector3 pos, Vector3 direction, float length)
{
	dContactGeom contact;
	contact.depth = FLT_MAX;
	contact.g1 = 0;
	contact.g2 = 0;

	dGeomID ray = dCreateRay(space, length);
	dGeomRaySet(ray, pos.x, pos.y, pos.z, direction.x, direction.y, direction.z);

	dSpaceCollide(space, &contact, rayColideCallback);

	dGeomDestroy(ray);

	return(contact);
}


class CollisionListener : public OgreOde::CollisionListener
{
public:
	CollisionListener(void) {}
	virtual ~CollisionListener(void) {}

	bool virtual collision(OgreOde::Contact *contact)
	{
		// Check for collisions between things that are connected and ignore them
		OgreOde::Geometry * const g1 = contact->getFirstGeometry();
		OgreOde::Geometry * const g2 = contact->getSecondGeometry();

		if (g1 && g2)
		{
			const OgreOde::Body * const b1 = g1->getBody();
			const OgreOde::Body * const  b2 = g2->getBody();
			if (b1 && b2 && OgreOde::Joint::areConnected(b1, b2))
				return true;
		}

		if((g1->getUserData() == SphereType) || (g2->getUserData() == SphereType))
		{
			contact->setCoulombFriction(OgreOde::Utility::Infinity);
			contact->setBouncyness(0.8);
		}
		else
		{
			contact->setCoulombFriction(OgreOde::Utility::Infinity);
			contact->setBouncyness(bounce);
		}


		// Set the friction at the contact




		//contact->setCoulombFriction(0.01);
		//contact->setFrictionMode(OgreOde::Contact::Flag_BothFrictionPyramids);

		//contact->setCoulombFriction(OgreOde::Utility::Infinity);

		// Yes, this collision is valid
		//qDebug() << "COLLISION!";
		return true;

	}


};


class TutorialFrameListener : public ExampleFrameListener, public OIS::MouseListener, public OIS::KeyListener, public OgreConsoleListener
{
public:
	TutorialFrameListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr, OgreOde::World *w, OgreOde::Space *s, OgreOde::StepHandler *st)
			: ExampleFrameListener(win, cam, true, true), mWorld(w), mSpace(s), mStepper(st), mSceneMgr(sceneMgr), mCamera(cam)
	{
		num = 0;
		console = false;
		mMouse->setEventCallback(this);
		mKeyboard->setEventCallback(this);
		ggunenabled = false;
	}

	Camera				     *mCamera;
	OgreOde::World                       *mWorld;
	OgreOde::Space                       *mSpace;
	OgreOde::StepHandler                 *mStepper;
	SceneManager 			     *mSceneMgr;
	int 				     num;
	bool				     console;
	bool				     ggunenabled;
	dBodyID			             gbody;


	bool frameStarted(const FrameEvent &evt)
	{
		Ogre::Real time = 0.1;
		if (mStepper->step(evt.timeSinceLastFrame))
		{
			mWorld->synchronise();
		}

		mMouse->capture();
		mKeyboard->capture();

		//bool currMouse2 = mMouse->getMouseState().buttonDown(OIS::MB_Right);
		//if (currMouse2) spawnBox();

		if (ggunenabled)
		{
			float powerfactor = 100;
			float maxvel = 1000;
			float distance = 100;

			Vector3 holdpos = mCamera->getPosition() + (mCamera->getDirection() * distance);
			dReal *tmp = dBodyGetPosition(gbody);
			Vector3 targetpos;
			targetpos.x = tmp[0];
			targetpos.y = tmp[1];
			targetpos.z = tmp[2];

			Vector3 v = holdpos - targetpos;
			v *= powerfactor;

			if (sqrt(v.x * v.x + v.y * v.y + v.z * v.z) > maxvel)
			{
				v.normalise();
				v *= maxvel;
			}

			dBodyEnable(gbody);
			dBodySetLinearVel(gbody, v.x, v.y, v.z);


			tmp = dBodyGetAngularVel(gbody);
			Vector3 angv;
			angv.x = tmp[0];
			angv.y = tmp[1];
			angv.z = tmp[2];
			angv*= -500000.0;

			dBodyAddTorque(gbody, angv.x, angv.y, angv.z);

		}

		return ExampleFrameListener::frameStarted(evt);
	}

	bool processUnbufferedKeyInput(const FrameEvent& evt)
	{
		if (console) return(true);
		return(ExampleFrameListener::processUnbufferedKeyInput(evt));
	}

	bool processUnbufferedMouseInput(const FrameEvent& evt)
	{
		if (console) return(true);
		return(ExampleFrameListener::processUnbufferedMouseInput(evt));
	}

	bool mouseMoved(const OIS::MouseEvent &e)
	{
		return(true);
	}

	bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
	{
		switch (id)
		{
			case OIS::MB_Left:
				if (weapon == CubeGun)
				{
					spawnBox();
				}
				else if (weapon == GravGun)
				{
					if (ggunenabled)
					{
						dBodySetLinearVel(gbody, mCamera->getDirection().x*500, mCamera->getDirection().y*500, mCamera->getDirection().z*500);
						dBodySetGravityMode(gbody, 1);
						ggunenabled = false;
						gbody = NULL;
					}
					else
					{
						dContactGeom contact = rayCollide(mSpace->getID(), mCamera->getPosition(), mCamera->getDirection(), FLT_MAX);
						dGeomID geom = 0;
						if ((contact.g1 == 0) || (contact.g2 == 0)) return(true);
						if (dGeomGetClass(contact.g1) != dRayClass) geom = contact.g1;
						else if (dGeomGetClass(contact.g2) != dRayClass) geom = contact.g2;

						if (geom == NULL) return(true);
						dBodyID body = dGeomGetBody(geom);
						if (body == NULL) return(true);
						OgreOde::Body *obody = (OgreOde::Body *)dBodyGetData(body);

						dBodyEnable(body);
						dBodySetLinearVel(body, mCamera->getDirection().x*500, mCamera->getDirection().y*500, mCamera->getDirection().z*500);
					}
				}
				break;
			case OIS::MB_Right:
			{
				if (weapon == CubeGun)
				{
					spawnSphere();
				}
				else if (weapon == GravGun)
				{
					if (ggunenabled)
					{
						dBodySetGravityMode(gbody, 1);
						ggunenabled = false;
						gbody = NULL;
					}
					else
					{
						dContactGeom contact = rayCollide(mSpace->getID(), mCamera->getPosition(), mCamera->getDirection(), FLT_MAX);
						dGeomID geom = 0;
						if ((contact.g1 == 0) || (contact.g2 == 0)) return(true);
						if (dGeomGetClass(contact.g1) != dRayClass) geom = contact.g1;
						else if (dGeomGetClass(contact.g2) != dRayClass) geom = contact.g2;

						if (geom == NULL) return(true);
						dBodyID body = dGeomGetBody(geom);
						if (body == NULL) return(true);
						OgreOde::Body *obody = (OgreOde::Body *)dBodyGetData(body);

						dBodyEnable(body);
						dBodySetGravityMode(body, 0);

						ggunenabled = true;
						gbody = body;
					}
				}
			}
			break;
			case OIS::MB_Middle:
			{
			}
			break;
			default:
				break;
		}
		return(true);
	}

	bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
	{
		switch (id)
		{
			case OIS::MB_Left:
				break;
			case OIS::MB_Right:
				if (weapon == CubeGun)
				{

				}
				else if (weapon == GravGun)
				{

				}
				break;
			case OIS::MB_Middle:
			{

			}
			break;
			default:
				break;
		}
		return(true);
	}

	bool keyPressed(const OIS::KeyEvent &e)
	{

		switch (e.key)
		{
			case OIS::KC_GRAVE:
				console = !console;
				break;
			case OIS::KC_SPACE:
			case OIS::KC_1:
				qDebug() << "You selected CubeGun";
				weapon = CubeGun;
				break;
			case OIS::KC_2:
				qDebug() << "You selected GravityGun";
				weapon = GravGun;
				break;
			default:
				break;
		}

		return(true);
	}

	bool keyReleased(const OIS::KeyEvent &e)
	{
		switch (e.key)
		{
			default:
				break;
		}
		return(true);
	}

	void consoleCommand(QStringList& command)
	{
		if (command.size() == 0) return;
		if (command[0] == "quit") exit(0);
		else if (command[0] == "gravity")
		{
			if (command.size() != 4) return;
			float arg1 = command[1].toFloat();
			float arg2 = command[1].toFloat();
			float arg3 = command[1].toFloat();

			mWorld->setGravity(Vector3(arg1, arg2, arg3));
		}
		else if (command[0] == "bounce")
		{
			if (command.size() != 2) return;
			bounce=command[1].toFloat();
		}

	}

	void spawnBox()
	{
		qDebug() << "Mouse Pressed!" << num;
		OgreOde::BoxMass               mMass;
		Ogre::SceneNode                *mNode;
		Ogre::Entity                   *mEntity;
		OgreOde::Body                  *mBody;
		OgreOde::Geometry              *mGeom;
		char entname[100];
		snprintf(entname, 100, "crate%i", num);
		mEntity = mSceneMgr->createEntity(entname, "cube.mesh");
		mEntity->setQueryFlags(1 << 2);
		char entname2[100];
		snprintf(entname2, 100, "crateent%i", num);
		mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(entname2);
		mNode->attachObject(mEntity);
		//mEntity->setNormaliseNormals(true);
		mEntity->setCastShadows(true);
		mEntity->setMaterialName("Colors/Blue");
		mBody = new OgreOde::Body(mWorld);
		mNode->attachObject(mBody);
		Vector3 size(40.0, 40.0, 40.0);
		mMass = OgreOde::BoxMass(1000, size);
		//mMass.setDensity(0, size);
		mGeom = (OgreOde::Geometry*)new OgreOde::BoxGeometry(size, mWorld, mSpace);
		mGeom->setUserData(CubeType);
		mNode->setScale(size.x * 0.01, size.y * 0.01, size.z * 0.01);
		mBody->setMass(mMass);
		mGeom->setBody(mBody);
		mEntity->setUserObject(mGeom);
		mBody->setOrientation(Quaternion(Radian(5.0), Ogre::Vector3(0, 0.5, 0)));
		mBody->setPosition(mCamera->getPosition());
		mBody->setLinearVelocity(mCamera->getDirection()*1000);
		num++;
	}

	void spawnSphere()
	{
		qDebug() << "Mouse Pressed!" << num;
		OgreOde::SphereMass               mMass;
		Ogre::SceneNode                *mNode;
		Ogre::Entity                   *mEntity;
		OgreOde::Body                  *mBody;
		OgreOde::Geometry              *mGeom;
		char entname[100];
		snprintf(entname, 100, "crate%i", num);
		mEntity = mSceneMgr->createEntity(entname, "spheremesh");
		mEntity->setQueryFlags(1 << 2);
		char entname2[100];
		snprintf(entname2, 100, "crateent%i", num);
		mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(entname2);
		mNode->attachObject(mEntity);
		//mEntity->setNormaliseNormals(true);
		mEntity->setCastShadows(true);
		mEntity->setMaterialName("Colors/Green");
		float size=10.0;
		mBody = new OgreOde::Body(mWorld);
		mNode->attachObject(mBody);
		mMass = OgreOde::SphereMass(1000, size/2);
		//mMass.setDensity(0, size);
		mGeom = (OgreOde::Geometry*)new OgreOde::SphereGeometry(size, mWorld, mSpace);
		mGeom->setUserData(SphereType);
		mNode->setScale(size * 0.05, size * 0.05, size * 0.05);
		mBody->setMass(mMass);
		mGeom->setBody(mBody);
		mEntity->setUserObject(mGeom);
		mBody->setOrientation(Quaternion(Radian(5.0), Ogre::Vector3(0, 0.5, 0)));
		mBody->setPosition(mCamera->getPosition());
		mBody->setLinearVelocity(mCamera->getDirection()*1000);
		num++;
	}

};


class TutorialApplication : public ExampleApplication
{
protected:
public:
	TutorialApplication()
	{
	}

	~TutorialApplication()
	{
	}
protected:
	String mQuakePk3;
	String mQuakeLevel;
	ExampleLoadingBar mLoadingBar;
	OgreOde::World				*mWorld;
	OgreOde::Space				*mSpace;
	OgreOde::StepHandler			*mStepper;
	Quake3Level				*level;
	dWorldID					worldid;
	size_t					spaceid;

	void loadResources(void)
	{
		mLoadingBar.start(mWindow, 1, 1, 0.75);

		// Turn off rendering of everything except overlays
		mSceneMgr->clearSpecialCaseRenderQueues();
		mSceneMgr->addSpecialCaseRenderQueue(RENDER_QUEUE_OVERLAY);
		mSceneMgr->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_INCLUDE);

		// Set up the world geometry link
		ResourceGroupManager::getSingleton().linkWorldGeometryToResourceGroup(
		    ResourceGroupManager::getSingleton().getWorldResourceGroupName(),
		    mQuakeLevel, mSceneMgr);

		// Initialise the rest of the resource groups, parse scripts etc
		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		ResourceGroupManager::getSingleton().loadResourceGroup(
		    ResourceGroupManager::getSingleton().getWorldResourceGroupName(),
		    false, true);

		// Back to full rendering
		mSceneMgr->clearSpecialCaseRenderQueues();
		mSceneMgr->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_EXCLUDE);

		mLoadingBar.finish();
	}



	// Override resource sources (include Quake3 archives)
	void setupResources(void)
	{

		// Load Quake3 locations from a file
		ConfigFile cf;

		cf.load("./quake3settings.cfg");

		mQuakePk3 = cf.getSetting("Pak0Location");
		mQuakeLevel = cf.getSetting("Map");

		ExampleApplication::setupResources();
		ResourceGroupManager::getSingleton().addResourceLocation(
		    mQuakePk3, "Zip", ResourceGroupManager::getSingleton().getWorldResourceGroupName(), true);

	}

	void convertVertex(bsp_vertex_t vertex, Vector3 &vert)
	{
		vert.x = vertex.point[0];
		vert.y = vertex.point[1];
		vert.z = vertex.point[2];
	}

	void createScene(void)
	{


		level = new Quake3Level();
		DataStreamPtr stream =
		    ResourceGroupManager::getSingleton().openResource("maps/chiropteradm.bsp",
		            ResourceGroupManager::getSingleton().getWorldResourceGroupName());
		level->loadFromStream(stream);

		fprintf(stderr, "NUMBER OF MODELS: %i\n", level->mNumModels);
		fprintf(stderr, "NUMBER OF ENTITIES: %i\n", level->mNumEntities);

		mWorld = new OgreOde::World(mSceneMgr); //Create world object
		mWorld->setGravity(Vector3(0, 0, -98.0665)); //Set gravity
		//mWorld->setCFM(10e-5); //set fruction and whatnot
		mWorld->setCFM(10e-55);
		mWorld->setERP(0.8);
		mWorld->setAutoSleep(true); //what does this stuff do?
		mWorld->setAutoSleepAverageSamplesCount(10);
		mWorld->setContactCorrectionVelocity(1.0);
		//mSpace = new OgreOde::OgreSceneManagerSpace(Vector3(0, 0, 0), Vector3(10000, 10000, 10000), 10, mSceneMgr, mWorld);
		//mWorld->setDefaultSpace(mSpace);
		mSpace = mWorld->getDefaultSpace();
		mWorld->setContactCorrectionVelocity(10.0);
		mWorld->setCollisionListener(new CollisionListener());

		worldid = mWorld->getWorldID();
		spaceid = mSpace->getID();

		//set up stepper, need to look into this later
		const Ogre::Real _time_step = 0.5;
		const Ogre::Real time_scale = Ogre::Real(1.7);
		const Ogre::Real max_frame_time = Ogre::Real(1.0 / 4);
		mStepper = new OgreOde::StepHandler(mWorld, OgreOde::StepHandler::QuickStep, _time_step, max_frame_time,
		                                    time_scale);

		//Add models
		for (int i = 0; i < level->mNumModels; i++)
		{
			QVector<OgreOde::TriangleIndex> indexes;
			QVector<Vector3> verts;
			OgreOde::TriangleIndex *indexarray = NULL;
			Vector3 *vertsarray = NULL;
			bsp_model_t model = level->mModels[i];

			OgreOde::TriangleIndex index = 0;
			for (int j = model.face_start; j < (model.face_start + model.face_count); j++)
			{
				bsp_face_t face = level->mFaces[j];

				int zero = verts.size();

				for (int k = face.vert_start; k < (face.vert_start + (face.vert_count)); k++)
				{
					Vector3 v;
					convertVertex(level->mVertices[k], v);
					verts << v;
				}

				for (int k = zero + 2; k < verts.size(); k++)
				{
					indexes << k;
					indexes <<  k - 1;
					indexes << zero;

				}
				qDebug() << "VERTS:" << face.vert_count;
			}

			qDebug() << "INDEXES:" << indexes;

			indexarray = new OgreOde::TriangleIndex[indexes.count()];
			memcpy(indexarray, indexes.constData(), sizeof(OgreOde::TriangleIndex)*indexes.size());
			vertsarray = new Vector3[verts.count()];
			memcpy(vertsarray, verts.constData(), sizeof(Vector3)*verts.size());

			//qDebug() << "Size of dVector3:" << sizeof(dVector3) << sizeof(float) << sizeof(Vector3);
			//dTriMeshDataID did = dGeomTriMeshDataCreate();
			//dGeomTriMeshDataBuildSingle(did,vertsarray,sizeof(dVector3),verts.count(),indexarray,indexes.count(),sizeof(int)*3);
			//qDebug() << "BEFORE";
			//qDebug() << "ID:" << dCreateTriMesh((dSpaceID)spaceid,did,NULL,NULL,NULL);
			//qDebug() << "AFTER";

			OgreOde::TriangleMeshGeometry *geom = new OgreOde::TriangleMeshGeometry(vertsarray, verts.count(), indexarray, indexes.count(), mWorld, mSpace);
		}

		qDebug() << "ENTITIES" << (char *)level->mEntities;


		mSceneMgr->setAmbientLight(ColourValue(0.0, 0.0, 0.0));
		mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);

		createSphere("spheremesh",20,16,16);

		//Ogre::DataStreamPtr stream2 = Ogre::ResourceGroupManager::getSingleton().openResource("crosshair.overlay");
		//OverlayManager::getSingleton().parseScript(stream2,"overlay");

		Ogre::Overlay *crosshair = OverlayManager::getSingleton().getByName("MyOverlays/ANewOverlay");
		crosshair->show();


		Entity *ent1 = mSceneMgr->createEntity("Robot", "robot.mesh");
		ent1->setCastShadows(true);
		SceneNode *node1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("RobotNode");
		node1->attachObject(ent1);

		OgreOde::EntityInformer *inf = new OgreOde::EntityInformer(ent1);
		//inf->createStaticTriangleMesh(mWorld,mSpace);

		Entity *ent2 = mSceneMgr->createEntity("Robot2", "robot.mesh");
		ent2->setCastShadows(true);
		SceneNode *node2 = node1->createChildSceneNode("RobotNode2", Vector3(50, 10, 0));
		node2->attachObject(ent2);

		node1->setPosition(Vector3(0, 10, 0));
		node2->scale(0.5, 0.5, 0.5);
		node1->yaw(Degree(90));
		node1->roll(Degree(90));

		Plane plane(Vector3::UNIT_Y, 0);
		MeshManager::getSingleton().createPlane("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
		                                        1500, 1500, 20, 20, true, 1, 5, 5, Vector3::UNIT_Z);
		Entity *pent = mSceneMgr->createEntity("GroundEntity", "ground");
		pent->setMaterialName("Examples/Rockwall");
		pent->setCastShadows(false);
		SceneNode *pnode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		pnode->attachObject(pent);
		pnode->pitch(Degree(90));
		node1->setPosition(Vector3(0,10,10));

		Light *light = mSceneMgr->createLight("Light1");
		light->setType(Light::LT_POINT);
		light->setPosition(Vector3(0, 150, 250));
		light->setDiffuseColour(1, 1, 1);
		light->setSpecularColour(1, 1, 1);

		light = mSceneMgr->createLight("Light2");
		light->setType(Light::LT_DIRECTIONAL);
		light->setDiffuseColour(0.25, 0.25, 0);
		light->setSpecularColour(0.25, 0.25, 0);
		light->setDirection(Vector3(0, -1, 1));

		light = mSceneMgr->createLight("Light3");
		light->setType(Light::LT_SPOTLIGHT);
		light->setDiffuseColour(0, 0, 1);
		light->setSpecularColour(0, 0, 1);
		light->setDirection(-1, -1, 0);
		light->setPosition(Vector3(300, 300, 0));
		light->setSpotlightRange(Degree(35), Degree(50));

		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		mSceneMgr->setSkyDome(true, "Examples/CloudySky", 65, 8);

		mCamera->setFixedYawAxis(true, Vector3::UNIT_Z);
		ViewPoint vp = mSceneMgr->getSuggestedViewpoint(true);
		//mCamera->setPosition(vp.position);
		mCamera->setPosition(Vector3(400, -200, 200));
		mCamera->pitch(Degree(90));
		mCamera->rotate(vp.orientation);

		OgreOde::BoxMass               mMass;
		Ogre::SceneNode                *mNode;
		Ogre::Entity                   *mEntity;
		OgreOde::Body                  *mBody;
		OgreOde::Geometry              *mGeom;
		mEntity = mSceneMgr->createEntity("crate", "cube.mesh");
		mEntity->setQueryFlags(1 << 2);
		mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("crate");
		mNode->attachObject(mEntity);
		//mEntity->setNormaliseNormals(true);
		mEntity->setCastShadows(true);
		mBody = new OgreOde::Body(mWorld);
		mNode->attachObject(mBody);
		Vector3 size(10.0, 10.0, 10.0);
		mMass = OgreOde::BoxMass(0.5, size);
		//mMass.setDensity(5.0, size);
		mGeom = (OgreOde::Geometry*)new OgreOde::BoxGeometry(size, mWorld, mSpace);
		mNode->setScale(size.x * 0.01, size.y * 0.01, size.z * 0.01);
		mBody->setMass(mMass);
		mGeom->setBody(mBody);
		mEntity->setUserObject(mGeom);
		mBody->setOrientation(Quaternion(Radian(5.0), Ogre::Vector3(0, 0.5, 0)));
		mBody->setPosition(Vector3(100, 0, 120));
		mWorld->setShowDebugGeometries(true);
	}


	void chooseSceneManager(void)
	{
		mSceneMgr = mRoot->createSceneManager("BspSceneManager");
	}



	virtual void createCamera()
	{
		mCamera = mSceneMgr->createCamera("PlayerCam");
		mCamera->setPosition(Vector3(0, 10, 500));
		mCamera->lookAt(Vector3(0, 0, 0));
		mCamera->setNearClipDistance(5);
	}

	virtual void createViewports()
	{
		Ogre::Viewport *vp = mWindow->addViewport(mCamera);
		vp->setBackgroundColour(ColourValue(0, 1, 0));
	}

	void createFrameListener(void)
	{
		mFrameListener = new TutorialFrameListener(mWindow, mCamera, mSceneMgr, mWorld, mSpace, mStepper);
		mRoot->addFrameListener(mFrameListener);
	}

};

*/

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

int main(int argc, char **argv);

INT WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR command_line, INT show_command)
{
	int    argc;
	char** argv;

	char*  arg;
	int    index;
	int    result;

	// count the arguments

	argc = 1;
	arg  = command_line;

	while (arg[0] != 0)
	{

		while (arg[0] != 0 && arg[0] == ' ')
		{
			arg++;
		}

		if (arg[0] != 0)
		{

			argc++;

			while (arg[0] != 0 && arg[0] != ' ')
			{
				arg++;
			}

		}

	}

	// tokenize the arguments

	argv = (char**)malloc(argc * sizeof(char*));

	arg = command_line;
	index = 1;

	while (arg[0] != 0)
	{

		while (arg[0] != 0 && arg[0] == ' ')
		{
			arg++;
		}

		if (arg[0] != 0)
		{

			argv[index] = arg;
			index++;

			while (arg[0] != 0 && arg[0] != ' ')
			{
				arg++;
			}

			if (arg[0] != 0)
			{
				arg[0] = 0;
				arg++;
			}

		}

	}

	// put the program name into argv[0]

	char filename[_MAX_PATH];

	GetModuleFileNameA(NULL, filename, _MAX_PATH);
	argv[0] = filename;

	// call the user specified main function

	result = main(argc, argv);

	free(argv);
	return result;

}

#endif

void messageHandler(QtMsgType type, const char *msg)
{
	switch (type)
	{
		case QtDebugMsg:
			LogManager::getSingleton().logMessage(msg, LML_NORMAL);
			break;
		case QtWarningMsg:
			LogManager::getSingleton().logMessage(msg, LML_NORMAL);
			break;
		case QtCriticalMsg:
			LogManager::getSingleton().logMessage(msg, LML_CRITICAL);
			break;
		case QtFatalMsg:
			LogManager::getSingleton().logMessage(msg, LML_CRITICAL);
			abort();
	}

}

int main(int argc, char **argv)
{
	QApplication qapp(argc,argv);
	// Create application object
	Application app;
	qInstallMsgHandler(messageHandler);
	
	try
	{
		app.go();
	}
	catch (Exception& e)
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		MessageBoxA(NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
		fprintf(stderr, "An exception has occurred: %s\n",
		        e.what());
#endif
	}

	return 0;
}
