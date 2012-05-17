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
#ifndef APPLICATION_H
#define APPLICATION_H

#include "ogreqtoverlay.h"
#include "consoledialog.h"
#include "testform.h"
#include <Ogre.h>
#include <OgreConfigFile.h>
#include <OIS/OIS.h>
#include <tnl.h>
#include "testconnection.h"
#include "tnlLog.h"


#define MAP_RESOURCE_GROUP "Map"



/**
	@author Brendan Powers <brendan@resara.com>
*/
class Application : public Ogre::FrameListener, public Ogre::WindowEventListener, public OIS::MouseListener, public OIS::KeyListener, public OgreConsoleListener, public LogConsumer
{
public:
	enum ClientGameState
	{
		Disconnected,
  		Connecting,
    		Connected,
      		Loading,
		InGame
	};
	
	Application();
	~Application();

	void go();

	//window listener functions
	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);

	void createConsole();
	
	//Input Functions
	void setupInput();
	bool mouseMoved(const OIS::MouseEvent &e);
	bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
	bool keyPressed(const OIS::KeyEvent &e);
	bool keyReleased(const OIS::KeyEvent &e);
	bool processUnbufferedKeyInput(const Ogre::FrameEvent& evt);
	bool processUnbufferedMouseInput(const Ogre::FrameEvent& evt);

	//Frame Functions
	bool frameStarted(const Ogre::FrameEvent &evt);
	bool frameRenderingQueued(const Ogre::FrameEvent &evt);
	bool frameEnded(const Ogre::FrameEvent &evt);

	//Console Functions
	void consoleCommand(QStringList& command);

	//Network Functions
	void connected();
	void disconnected();
	
	//Utility Functions
	static Application *instance();
	void logString(const char *string);
	Ogre::SceneManager *getScene();
	void updateStats();

	//Map Functions
	void loadMap(QString map);
	void unloadMap();
private:
	void setupResources();
	void loadResources();

	TNL::NetInterface *_tnlInterface;
	TestConnection *_conn;

	Ogre::Root *_root;
	Ogre::Camera *_camera;
	Ogre::SceneManager *_scene;
	Ogre::RenderWindow *_window;
	Ogre::String _resourcepath;
	Ogre::Viewport *_viewport;
	Ogre::String _levelname;

	OIS::InputManager* _inputmanager;
	OIS::Mouse*    _mouse;
	OIS::Keyboard* _keyboard;

	bool _consolevisible;
	ConsoleDialog *_console;
	OgreQtOverlay *_overlay;
	TestForm *_browser;
	
	Ogre::Overlay *_debugoverlay;
	
	//network
	time_t _lastnetupdate;

	static Application *_app;
	
	ClientGameState _state;
};

inline Application *app()
{
	return(Application::instance());
}

#endif
