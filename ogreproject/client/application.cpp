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



#include <QFile>
#include <QDebug>
#include <QGraphicsProxyWidget>

#include "application.h"


Application *Application::_app = NULL;

using namespace Ogre;


#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>

// This function will locate the path to our application on OS X,
// unlike windows you can not rely on the curent working directory
// for locating your configuration files and resources.
std::string macBundlePath()
{
	char path[1024];
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	assert(mainBundle);

	CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
	assert(mainBundleURL);

	CFStringRef cfStringRef = CFURLCopyFileSystemPath(mainBundleURL, kCFURLPOSIXPathStyle);
	assert(cfStringRef);

	CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);

	CFRelease(mainBundleURL);
	CFRelease(cfStringRef);

	return std::string(path);
}
#endif



Application::Application() : LogConsumer()
{
	//set up static var
	_app = this;

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
	_resourcepath = macBundlePath() + "/Contents/Resources/";
#else
	_resourcepath = "";
#endif

	String pluginspath;
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	pluginspath = _resourcepath + "plugins-win32.cfg";
#else
	pluginspath = _resourcepath + "plugins-linux.cfg";
#endif

	_root = OGRE_NEW Root(pluginspath, _resourcepath + "ogre.cfg", _resourcepath + "Ogre.log");

	setupResources();

	qDebug() << "LEVEL NAME1:" << QString::fromStdString(_levelname);

	//Set Up Render Window
	if (QFile::exists(QString::fromStdString(_resourcepath + "ogre.cfg")))
	{
		_root->restoreConfig();
	}
	else
	{
		if (!_root->showConfigDialog()) return;
	}

	_window = _root->initialise(true);
	_scene = _root->createSceneManager("BspSceneManager");
	_camera = _scene->createCamera("PlayerCam");
	_viewport = _window->addViewport(_camera);

	// Set default mipmap level (NB some APIs ignore this)
	TextureManager::getSingleton().setDefaultNumMipmaps(5);

	qDebug() << "LEVEL NAME:" << QString::fromStdString(_levelname);

	loadResources();

	setupInput();

	_root->addFrameListener(this);

	_consolevisible = false;

	//torque shit
	/*
	TNLLogEnable(LogConnectionProtocol, true);
	TNLLogEnable(LogNetConnection, true);
	TNLLogEnable(LogEventConnection, true);
	TNLLogEnable(LogGhostConnection, true);
	TNLLogEnable(LogNetInterface, true);
	TNLLogEnable(LogPlatform, true);
	*/
	
	_overlay = new OgreQtOverlay(_root, _window, _window->getWidth(), _window->getHeight(), NULL);
	_root->addFrameListener(_overlay);

	//Init Console
	createConsole();
	loadMap(_levelname.c_str());
	
	
	_tnlInterface = new TNL::NetInterface(Address(IPProtocol, Address::Any, 0));
	_state = Connecting;
	_conn = new TestConnection();
	_conn->connect(_tnlInterface, Address("127.0.0.1:12000"));
	
	_debugoverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
	_debugoverlay->show();
}

void Application::setupResources()
{
	ConfigFile cf;
	cf.load(_resourcepath + "resources.cfg");

	// Go through all sections & settings in the file
	ConfigFile::SectionIterator seci = cf.getSectionIterator();

	String secName, typeName, archName;
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		ConfigFile::SettingsMultiMap *settings = seci.getNext();
		ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
			// OS X does not set the working directory relative to the app,
			// In order to make things portable on OS X we need to provide
			// the loading with it's own bundle path location
			ResourceGroupManager::getSingleton().addResourceLocation(
			    String(macBundlePath() + "/" + archName), typeName, secName);
#else
			ResourceGroupManager::getSingleton().addResourceLocation(
			    archName, typeName, secName);
#endif
		}
	}




	// Load Quake3 locations from a file
	ConfigFile q3cf;

	q3cf.load("quake3settings.cfg");

	String pk3 = q3cf.getSetting("Pak0Location");
	_levelname = q3cf.getSetting("Map");

	ResourceGroupManager::getSingleton().setWorldResourceGroupName(MAP_RESOURCE_GROUP);

	ResourceGroupManager::getSingleton().addResourceLocation(
	    pk3, "Zip", ResourceGroupManager::getSingleton().getWorldResourceGroupName(), true);
}

void Application::createConsole()
{
	_browser = new TestForm(NULL);
	QGraphicsProxyWidget *widget = new QGraphicsProxyWidget(NULL,Qt::Window);
	widget->setWidget(_browser);
	widget->setPos(0,0);
	_overlay->addItem(widget);
	_browser->hide();

	_console = new ConsoleDialog(NULL);
	widget = new QGraphicsProxyWidget(0, Qt::Window);
	widget->setWidget(_console);
	//widget->setPos(_overlay->width() - 400, 100)
	widget->setPos(_overlay->width()-_console->width(),0);
	_overlay->addItem(widget);
	_console->setListener(this);
	_console->hide();
}

Application::~Application()
{
	WindowEventUtilities::removeWindowEventListener(_window, this);
	windowClosed(_window);

	if (_root)
		OGRE_DELETE _root;
}


void Application::go()
{
	qDebug() << "Starting Rendering";
	_root->startRendering();
}

void Application::loadResources()
{
	// Initialise, parse scripts etc
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	///TODO Understand this better
	///TODO Fix loading bar
	//mLoadingBar.start(mWindow, 1, 1, 0.75);

	// Turn off rendering of everything except overlays
	//_scene->clearSpecialCaseRenderQueues();
	//_scene->addSpecialCaseRenderQueue(RENDER_QUEUE_OVERLAY);
	//_scene->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_INCLUDE);

	// Set up the world geometry link

	// Initialise the rest of the resource groups, parse scripts etc
	ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	

	// Back to full rendering
	//_scene->clearSpecialCaseRenderQueues();
	//_scene->setSpecialCaseRenderQueueMode(SceneManager::SCRQM_EXCLUDE);

	//mLoadingBar.finish();
}

//********************************************************************************************************
// *** WindowListener Functions
//********************************************************************************************************
//This is aparently for OIS, not for ode
void Application::windowResized(RenderWindow* rw)
{
	unsigned int width, height, depth;
	int left, top;

	_window->getMetrics(width, height, depth, left, top);

	const OIS::MouseState &ms = _mouse->getMouseState();
	ms.width = width;
	ms.height = height;

}

void Application::windowClosed(RenderWindow* rw)
{
	//Make sure the window that closed is the one we care about
	if (rw == _window)
	{
		if (_inputmanager) //make sure we have a valid inputmanager
		{
			_inputmanager->destroyInputObject(_mouse);
			_inputmanager->destroyInputObject(_keyboard);

			OIS::InputManager::destroyInputSystem(_inputmanager);
			_inputmanager = 0;
		}
	}
}


//********************************************************************************************************
// *** Input Functions
//********************************************************************************************************
void Application::setupInput()
{
	OIS::ParamList pl;
	size_t handle = 0;
	std::ostringstream handlestr;

	//Get Window handle, and put it in a parameter structure
	_window->getCustomAttribute("WINDOW", &handle);
	handlestr << handle;
	pl.insert(std::make_pair(std::string("WINDOW"), handlestr.str()));

	//initialize manager
	_inputmanager = OIS::InputManager::createInputSystem(pl);

	//Create devices
	_keyboard = static_cast<OIS::Keyboard*>(_inputmanager->createInputObject(OIS::OISKeyboard, true));
	_mouse = static_cast<OIS::Mouse*>(_inputmanager->createInputObject(OIS::OISMouse, true));

	windowResized(_window);
	WindowEventUtilities::addWindowEventListener(_window, this);
	_mouse->setEventCallback(this);
	_keyboard->setEventCallback(this);
}

bool Application::mouseMoved(const OIS::MouseEvent &e)
{
	_overlay->mouseMoved(e);
	
	return(true);
}

bool Application::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	_overlay->mousePressed(e, id);
	return(true);
}

bool Application::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	_overlay->mouseReleased(e, id);
	return(true);
}

bool Application::keyPressed(const OIS::KeyEvent &e)
{
	if (e.key == OIS::KC_GRAVE)
	{
		_consolevisible = !_consolevisible;
		_browser->setVisible(_consolevisible);
		_console->setVisible(_consolevisible);
		_overlay->setVisible(_consolevisible);
		if (_consolevisible)
		{
			_console->focus();
		}
		return(true);
	}
	_overlay->keyPressed(e);
	if (_consolevisible) return(true);

	switch (e.key)
	{

		default:
			break;
	}
	
	return(true);
}

bool Application::keyReleased(const OIS::KeyEvent &e)
{
	_overlay->keyReleased(e);
	if (_consolevisible) return(true);
	return(true);
}

bool Application::processUnbufferedKeyInput(const FrameEvent& evt)
{
	if (_keyboard->isKeyDown(OIS::KC_ESCAPE))
		return(false);
	if (_consolevisible) return(true);

	Vector3 tv;
	if (_keyboard->isKeyDown(OIS::KC_W))
		tv.z -= 1;
	if (_keyboard->isKeyDown(OIS::KC_S))
		tv.z += 1;
	if (_keyboard->isKeyDown(OIS::KC_A))
		tv.x -= 1;
	if (_keyboard->isKeyDown(OIS::KC_D))
		tv.x += 1;

	if(_camera != NULL) 
	{
		_camera->moveRelative(tv);
	}

	return(true);
}

bool Application::processUnbufferedMouseInput(const FrameEvent& evt)
{
	if (_consolevisible) return(true);

	const OIS::MouseState &ms = _mouse->getMouseState();
	Radian rotx = Degree(ms.X.rel * -0.13);
	Radian roty = Degree(ms.Y.rel * -0.13);

	if(_camera != NULL)
	{
		_camera->yaw(rotx);
	 	_camera->pitch(roty);
	}

	return(true);
}


//********************************************************************************************************
// *** Frame Functions
//********************************************************************************************************
bool Application::frameStarted(const FrameEvent &evt)
{
	return(true);
}

bool Application::frameRenderingQueued(const FrameEvent &evt)
{
	if (_window->isClosed()) return(false);

	_keyboard->capture();
	_mouse->capture();
	if (!processUnbufferedKeyInput(evt)) return(false);
	if (!processUnbufferedMouseInput(evt)) return(false);

	// tnl
	_tnlInterface->checkIncomingPackets();
	_tnlInterface->processConnections();

	if(_root->getTimer()->getMilliseconds() - _lastnetupdate >= 33)
	{
		if(_state >= Connected)
		{
			if(_camera != NULL) _conn->updateCamera(_camera->getPosition(),_camera->getOrientation());
		}
		_lastnetupdate = _root->getTimer()->getMilliseconds();
	}
	
	return(true);
}

bool Application::frameEnded(const FrameEvent &evt)
{
	updateStats();
	return(true);
}

//********************************************************************************************************
// *** Console Commands
//********************************************************************************************************
void Application::consoleCommand(QStringList& command)
{
	if (command.size() == 0) return;
	else if (command[0] == "quit") exit(0);
	else if (command[0] == "disconnect") _conn->disconnect("Manual Disconnect");
	else if (command[0] == "connect")
	{
		if (command.size() != 2)
		{
			qDebug() << "Usage: connect hostname:port";
			return;
		}

		if (_conn != NULL) _conn->disconnect("Manual Disconnect");
		_state = Connected;
		_conn = new TestConnection();
		_conn->connect(_tnlInterface, Address(qPrintable(command[1])));
		qDebug() << "Should be connecting to:" << command[1];
	}
	else if (command[0] == "load")
	{
		if (command.size() != 2)
		{
			qDebug() << "Usage: load mapname.bsp";
			return;
		}

		loadMap("maps/"+command[1]);
	}
	else if (command[0] == "unload")
	{
		unloadMap();
	}
	else
	{
		if (_conn != NULL)
			_conn->sendConsoleCommand(command);
	}
}

//********************************************************************************************************
// *** Network
//********************************************************************************************************
void Application::connected()
{
	qDebug() << "Connected!";
	_state = Connected;
}

void Application::disconnected()
{
	qDebug() << "Disconnected!";
	_state = Disconnected;
	_conn = NULL;
}

//********************************************************************************************************
// *** Utility Functions
//********************************************************************************************************
Application *Application::instance()
{
	return(_app);
}

void Application::logString(const char *string)
{
	qDebug() << "TNL:" << string;
}

SceneManager *Application::getScene()
{
	return(_scene);
}

void Application::updateStats()
{
	static String currFps = "Current FPS: ";
	static String avgFps = "Average FPS: ";
	static String bestFps = "Best FPS: ";
	static String worstFps = "Worst FPS: ";
	static String tris = "Triangle Count: ";
	static String batches = "Batch Count: ";

                // update stats when necessary
	try {
		OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
		OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
		OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
		OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

		const RenderTarget::FrameStats& stats = _window->getStatistics();
		guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
		guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
		guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
				+" "+StringConverter::toString(stats.bestFrameTime)+" ms");
		guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
				+" "+StringConverter::toString(stats.worstFrameTime)+" ms");

		OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
		guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

		OverlayElement* guiBatches = OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
		guiBatches->setCaption(batches + StringConverter::toString(stats.batchCount));

		OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
		guiDbg->setCaption("What?");
	}
	catch(...) { /* ignore */ }

}

//********************************************************************************************************
// *** Map Functions
//********************************************************************************************************
void Application::loadMap(QString map)
{
	ResourceGroupManager::getSingleton().linkWorldGeometryToResourceGroup(
	    ResourceGroupManager::getSingleton().getWorldResourceGroupName(),
	    qPrintable(map), _scene);

	ResourceGroupManager::getSingleton().loadResourceGroup(
	    ResourceGroupManager::getSingleton().getWorldResourceGroupName(),
	    false, true);
	
		//Init camera
	_camera->setFixedYawAxis(true, Vector3::UNIT_Z);
	ViewPoint vp = _scene->getSuggestedViewpoint(true);
	_camera->setPosition(vp.position);
	_camera->pitch(Degree(90));
	_camera->rotate(vp.orientation);
	_camera->setNearClipDistance(5);
	_camera->setAspectRatio(Real(_viewport->getActualWidth()) / Real(_viewport->getActualHeight()));
}

void Application::unloadMap()
{
	_scene->clearScene();
	_window->removeAllViewports();
	_scene->destroyCamera(_camera);
	_camera=NULL;
	_viewport=NULL;
	ResourceGroupManager::getSingleton().unloadResourceGroup(
	    ResourceGroupManager::getSingleton().getWorldResourceGroupName());
	_scene = _root->createSceneManager("BspSceneManager");
	_camera = _scene->createCamera("PlayerCam");
	_viewport = _window->addViewport(_camera);
}

