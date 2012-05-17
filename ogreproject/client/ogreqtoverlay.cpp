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
#include "ogreqtoverlay.h"
#include <QDebug>
#include <QPainter>
#include <QGraphicsProxyWidget>
#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QDateTime>
#include <QGraphicsProxyWidget>

using namespace Ogre;

#define POINTER_FILE "../Media/materials/textures/left_ptr.png"
#define POINTER_SCALE_X 0.8
#define POINTER_SCALE_Y 0.8
#define POINTER_Z 100000

OgreQtOverlay::OgreQtOverlay(Ogre::Root *root, Ogre::RenderWindow *window, Ogre::uint w, Ogre::uint h, QObject *parent)
 : QGraphicsScene(parent), _width(w), _height(h)
{
	_root = root;
	_window = window;
	_time.start(); //start a timer for key events
	
	//Connect redraw signal to this class
	QObject::connect(this,SIGNAL(changed(QList<QRectF>)),this,SLOT(sceneChanged(QList<QRectF>)));
	
	//qDebug() << "WIDTH:" << _width << "HEIGHT:" << _height;
	
	//create a texture the size of the screen to draw into
	_texture = TextureManager::getSingleton().createManual("OgreQtTexture",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
			,TEX_TYPE_2D,_width,_height,0,PF_A8R8G8B8,TU_DYNAMIC_WRITE_ONLY).getPointer();
	
	//save texture buffer for later
	_buffer = _texture->getBuffer().getPointer();
	
	//Create a material
	MaterialPtr material = MaterialManager::getSingleton().create("DynamicTextureMaterial",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	material->getTechnique(0)->getPass(0)->createTextureUnitState("OgreQtTexture");
	material->getTechnique(0)->getPass(0)->setSceneBlending(SBT_TRANSPARENT_ALPHA);
	
	//Load overlay
	_overlay = OverlayManager::getSingleton().getByName("MyOverlays/OgreQt");
	OverlayElement *element = OverlayManager::getSingleton().getOverlayElement("MyOverlayElements/QtPanel");
	element->setMaterialName("DynamicTextureMaterial");
	
	//set the screen rect for the graphicsscene
	float ratio = (float)_width / (float)_height;
	setSceneRect(QRectF(0.0,0.0,_width,_height));
	
	//Create Cursor Pixmap
	_cursor = addPixmap(QPixmap(POINTER_FILE));
	_cursor->scale(POINTER_SCALE_X,POINTER_SCALE_Y);
	_cursor->setZValue(POINTER_Z);
	
	//set some default values
	_modifiers = Qt::NoModifier;
	_lastkeytime=0;
	_repeating=false;
	_lastclicktime = 0;
	_visible=false;
	_overlay->hide();
}


OgreQtOverlay::~OgreQtOverlay()
{
	//I probably should put something here:)
}

bool OgreQtOverlay::frameRenderingQueued(const FrameEvent &evt)
{	
	if(!_visible) return(true);
	
	//do key repeats
	if(_lastkeytime != 0)
	{
		if(_repeating)
		{
			if((_time.elapsed() - _lastkeytime) >= 50)
			{
				_lastkeytime = _time.elapsed();
				keyPressEvent( new QKeyEvent(QEvent::KeyPress,_lastkey,_modifiers,_lastchar,true,1));
				keyReleaseEvent( new QKeyEvent(QEvent::KeyRelease,_lastkey,_modifiers,_lastchar,true,1));
			}
		}
		else if((_time.elapsed() - _lastkeytime) >= 400)
		{
			_repeating=true;
		}
		
	}
	else
	{
		_repeating=false;
	}
	
	//Event Loop!
	QApplication::processEvents();
	
	return(true);
}

void OgreQtOverlay::keyPressed(const OIS::KeyEvent &e)
{
	QChar c = e.text;
	QString t = c;
	int key;
	
	//convert OIS key events to qt key events
	switch(e.key)
	{
		case OIS::KC_ESCAPE:
			key=Qt::Key_Escape;
			break;
		case OIS::KC_1:
			key=Qt::Key_1;
			break;
		case OIS::KC_2:
			key=Qt::Key_2;
			break;
		case OIS::KC_3:
			key=Qt::Key_3;
			break;
		case OIS::KC_4:
			key=Qt::Key_4;
			break;
		case OIS::KC_5:
			key=Qt::Key_5;
			break;
		case OIS::KC_6:
			key=Qt::Key_6;
			break;
		case OIS::KC_7:
			key=Qt::Key_7;
			break;
		case OIS::KC_8:
			key=Qt::Key_8;
			break;
		case OIS::KC_9:
			key=Qt::Key_9;
			break;
		case OIS::KC_0:
			key=Qt::Key_0;
			break;
		case OIS::KC_MINUS:
			key=Qt::Key_Minus;
			break;
		case OIS::KC_EQUALS:
			key=Qt::Key_Equal;
			break;
		case OIS::KC_BACK:
			key=Qt::Key_Backspace;
			break;
		case OIS::KC_TAB:
			key=Qt::Key_Tab;
			break;
		case OIS::KC_Q:
			key=Qt::Key_Q;
			break;
		case OIS::KC_W:
			key=Qt::Key_W;
			break;
		case OIS::KC_E:
			key=Qt::Key_E;
			break;
		case OIS::KC_R:
			key=Qt::Key_R;
			break;
		case OIS::KC_T:
			key=Qt::Key_T;
			break;
		case OIS::KC_Y:
			key=Qt::Key_Y;
			break;
		case OIS::KC_U:
			key=Qt::Key_U;
			break;
		case OIS::KC_I:
			key=Qt::Key_I;
			break;
		case OIS::KC_O:
			key=Qt::Key_O;
			break;
		case OIS::KC_P:
			key=Qt::Key_P;
			break;
		case OIS::KC_LBRACKET:
			key=Qt::Key_BracketLeft;
			break;
		case OIS::KC_RBRACKET:
			key=Qt::Key_BracketRight;
			break;
		case OIS::KC_RETURN:
			key=Qt::Key_Enter;
			break;
		case OIS::KC_LCONTROL:
			key=Qt::Key_Control;
			_modifiers|=Qt::ControlModifier;
			break;
		case OIS::KC_A:
			key=Qt::Key_A;
			break;
		case OIS::KC_S:
			key=Qt::Key_S;
			break;
		case OIS::KC_D:
			key=Qt::Key_D;
			break;
		case OIS::KC_F:
			key=Qt::Key_F;
			break;
		case OIS::KC_G:
			key=Qt::Key_G;
			break;
		case OIS::KC_H:
			key=Qt::Key_H;
			break;
		case OIS::KC_J:
			key=Qt::Key_J;
			break;
		case OIS::KC_K:
			key=Qt::Key_K;
			break;
		case OIS::KC_L:
			key=Qt::Key_L;
			break;
		case OIS::KC_SEMICOLON:
			key=Qt::Key_Semicolon;
			break;
		case OIS::KC_APOSTROPHE:
			key=Qt::Key_Apostrophe;
			break;
		case OIS::KC_GRAVE:
			key=Qt::Key_AsciiTilde;
			break;
		case OIS::KC_LSHIFT:
			key=Qt::Key_Shift;
			_modifiers|= Qt::ShiftModifier;
			break;
		case OIS::KC_BACKSLASH:
			key=Qt::Key_Backslash;
			break;
		case OIS::KC_Z:
			key=Qt::Key_Z;
			break;
		case OIS::KC_X:
			key=Qt::Key_X;
			break;
		case OIS::KC_C:
			key=Qt::Key_C;
			break;
		case OIS::KC_V:
			key=Qt::Key_V;
			break;
		case OIS::KC_B:
			key=Qt::Key_B;
			break;
		case OIS::KC_N:
			key=Qt::Key_N;
			break;
		case OIS::KC_M:
			key=Qt::Key_M;
			break;
		case OIS::KC_COMMA:
			key=Qt::Key_Comma;
			break;
		case OIS::KC_PERIOD:
			key=Qt::Key_Period;
			break;
		case OIS::KC_SLASH:
			key=Qt::Key_Slash;
			break;
		case OIS::KC_RSHIFT:
			key=Qt::Key_Shift;
			_modifiers|=Qt::ShiftModifier;
			break;
		case OIS::KC_MULTIPLY:
			key=Qt::Key_multiply;
			break;
		case OIS::KC_LMENU:
			key=Qt::Key_Menu;
			break;
		case OIS::KC_SPACE:
			key=Qt::Key_Space;
			break;
		case OIS::KC_CAPITAL:
			key=Qt::Key_CapsLock;
			break;
		case OIS::KC_F1:
			key=Qt::Key_F1;
			break;
		case OIS::KC_F2:
			key=Qt::Key_F2;
			break;
		case OIS::KC_F3:
			key=Qt::Key_F3;
			break;
		case OIS::KC_F4:
			key=Qt::Key_F4;
			break;
		case OIS::KC_F5:
			key=Qt::Key_F5;
			break;
		case OIS::KC_F6:
			key=Qt::Key_F6;
			break;
		case OIS::KC_F7:
			key=Qt::Key_F7;
			break;
		case OIS::KC_F8:
			key=Qt::Key_F8;
			break;
		case OIS::KC_F9:
			key=Qt::Key_F9;
			break;
		case OIS::KC_F10:
			key=Qt::Key_F10;
			break;
		case OIS::KC_NUMLOCK:
			key=Qt::Key_NumLock;
			break;
		case OIS::KC_SCROLL:
			key=Qt::Key_ScrollLock;
			break;
		case OIS::KC_NUMPAD7:
			key=Qt::Key_7;
			break;
		case OIS::KC_NUMPAD8:
			key=Qt::Key_8;
			break;
		case OIS::KC_NUMPAD9:
			key=Qt::Key_9;
			break;
		case OIS::KC_SUBTRACT:
			key=Qt::Key_Minus;
			break;
		case OIS::KC_NUMPAD4:
			key=Qt::Key_4;
			break;
		case OIS::KC_NUMPAD5:
			key=Qt::Key_5;
			break;
		case OIS::KC_NUMPAD6:
			key=Qt::Key_6;
			break;
		case OIS::KC_ADD:
			key=Qt::Key_Plus;
			break;
		case OIS::KC_NUMPAD1:
			key=Qt::Key_1;
			break;
		case OIS::KC_NUMPAD2:
			key=Qt::Key_2;
			break;
		case OIS::KC_NUMPAD3:
			key=Qt::Key_3;
			break;
		case OIS::KC_NUMPAD0:
			key=Qt::Key_0;
			break;
		case OIS::KC_DECIMAL:
			key=Qt::Key_Period;
			break;
		case OIS::KC_F11:
			key=Qt::Key_F11;
			break;
		case OIS::KC_F12:
			key=Qt::Key_F12;
			break;
		case OIS::KC_F13:
			key=Qt::Key_F13;
			break;
		case OIS::KC_F14:
			key=Qt::Key_F14;
			break;
		case OIS::KC_F15:
			key=Qt::Key_F15;
			break;
		case OIS::KC_KANA:
			key=Qt::Key_Kana_Shift;
			break;
		case OIS::KC_YEN:
			key=Qt::Key_yen;
			break;
		case OIS::KC_NUMPADEQUALS:
			key=Qt::Key_Equal;
			break;
		case OIS::KC_AT:
			key=Qt::Key_At;
			break;
		case OIS::KC_COLON:
			key=Qt::Key_Colon;
			break;
		case OIS::KC_UNDERLINE:
			key=Qt::Key_Underscore;
			break;
		case OIS::KC_KANJI:
			key=Qt::Key_Kanji;
			break;
		case OIS::KC_STOP:
			key=Qt::Key_Stop;
			break;
		case OIS::KC_NUMPADENTER:
			key=Qt::Key_Enter;
			break;
		case OIS::KC_RCONTROL:
			key=Qt::Key_Control;
			_modifiers|=Qt::ControlModifier;
			break;
		case OIS::KC_NUMPADCOMMA:
			key=Qt::Key_Comma;
			break;
		case OIS::KC_DIVIDE:
			key=Qt::Key_division;
			break;
		case OIS::KC_SYSRQ:
			key=Qt::Key_SysReq;
			break;
		case OIS::KC_RMENU:
			key=Qt::Key_Menu;
			break;
		case OIS::KC_PAUSE:
			key=Qt::Key_Pause;
			break;
		case OIS::KC_HOME:
			key=Qt::Key_Home;
			break;
		case OIS::KC_UP:
			key=Qt::Key_Up;
			break;
		case OIS::KC_PGUP:
			key=Qt::Key_PageUp;
			break;
		case OIS::KC_LEFT:
			key=Qt::Key_Left;
			break;
		case OIS::KC_RIGHT:
			key=Qt::Key_Right;
			break;
		case OIS::KC_END:
			key=Qt::Key_End;
			break;
		case OIS::KC_DOWN:
			key=Qt::Key_Down;
			break;
		case OIS::KC_PGDOWN:
			key=Qt::Key_PageDown;
			break;
		case OIS::KC_INSERT:
			key=Qt::Key_Insert;
			break;
		case OIS::KC_DELETE:
			key=Qt::Key_Delete;
			break;
		case OIS::KC_LWIN:
			key=Qt::Key_Meta;
			_modifiers|=Qt::MetaModifier;
			break;
		case OIS::KC_RWIN:
			key=Qt::Key_Meta;
			_modifiers|=Qt::MetaModifier;
			break;

		
		default:
			key=0;
	}
	
	//fill values for key repeats
	if(key != 0)
	{
		_lastkey=key;
		_lastchar = t;
		_lastkeytime = _time.elapsed();
		_repeating=false;
	}
	
	//generate key press event
	keyPressEvent( new QKeyEvent(QEvent::KeyPress,key,_modifiers,t,true,1));
}

void OgreQtOverlay::keyReleased(const OIS::KeyEvent &e)
{
	QChar c = e.text;
	QString t = c;
	int key;
	
	//convert OIS key events to QT key events
	switch(e.key)
	{
		case OIS::KC_ESCAPE:
			key=Qt::Key_Escape;
			break;
		case OIS::KC_1:
			key=Qt::Key_1;
			break;
		case OIS::KC_2:
			key=Qt::Key_2;
			break;
		case OIS::KC_3:
			key=Qt::Key_3;
			break;
		case OIS::KC_4:
			key=Qt::Key_4;
			break;
		case OIS::KC_5:
			key=Qt::Key_5;
			break;
		case OIS::KC_6:
			key=Qt::Key_6;
			break;
		case OIS::KC_7:
			key=Qt::Key_7;
			break;
		case OIS::KC_8:
			key=Qt::Key_8;
			break;
		case OIS::KC_9:
			key=Qt::Key_9;
			break;
		case OIS::KC_0:
			key=Qt::Key_0;
			break;
		case OIS::KC_MINUS:
			key=Qt::Key_Minus;
			break;
		case OIS::KC_EQUALS:
			key=Qt::Key_Equal;
			break;
		case OIS::KC_BACK:
			key=Qt::Key_Backspace;
			break;
		case OIS::KC_TAB:
			key=Qt::Key_Tab;
			break;
		case OIS::KC_Q:
			key=Qt::Key_Q;
			break;
		case OIS::KC_W:
			key=Qt::Key_W;
			break;
		case OIS::KC_E:
			key=Qt::Key_E;
			break;
		case OIS::KC_R:
			key=Qt::Key_R;
			break;
		case OIS::KC_T:
			key=Qt::Key_T;
			break;
		case OIS::KC_Y:
			key=Qt::Key_Y;
			break;
		case OIS::KC_U:
			key=Qt::Key_U;
			break;
		case OIS::KC_I:
			key=Qt::Key_I;
			break;
		case OIS::KC_O:
			key=Qt::Key_O;
			break;
		case OIS::KC_P:
			key=Qt::Key_P;
			break;
		case OIS::KC_LBRACKET:
			key=Qt::Key_BracketLeft;
			break;
		case OIS::KC_RBRACKET:
			key=Qt::Key_BracketRight;
			break;
		case OIS::KC_RETURN:
			key=Qt::Key_Enter;
			break;
		case OIS::KC_LCONTROL:
			key=Qt::Key_Control;
			_modifiers^=Qt::ControlModifier;
			break;
		case OIS::KC_A:
			key=Qt::Key_A;
			break;
		case OIS::KC_S:
			key=Qt::Key_S;
			break;
		case OIS::KC_D:
			key=Qt::Key_D;
			break;
		case OIS::KC_F:
			key=Qt::Key_F;
			break;
		case OIS::KC_G:
			key=Qt::Key_G;
			break;
		case OIS::KC_H:
			key=Qt::Key_H;
			break;
		case OIS::KC_J:
			key=Qt::Key_J;
			break;
		case OIS::KC_K:
			key=Qt::Key_K;
			break;
		case OIS::KC_L:
			key=Qt::Key_L;
			break;
		case OIS::KC_SEMICOLON:
			key=Qt::Key_Semicolon;
			break;
		case OIS::KC_APOSTROPHE:
			key=Qt::Key_Apostrophe;
			break;
		case OIS::KC_GRAVE:
			key=Qt::Key_AsciiTilde;
			break;
		case OIS::KC_LSHIFT:
			key=Qt::Key_Shift;
			_modifiers^=Qt::ShiftModifier;
			break;
		case OIS::KC_BACKSLASH:
			key=Qt::Key_Backslash;
			break;
		case OIS::KC_Z:
			key=Qt::Key_Z;
			break;
		case OIS::KC_X:
			key=Qt::Key_X;
			break;
		case OIS::KC_C:
			key=Qt::Key_C;
			break;
		case OIS::KC_V:
			key=Qt::Key_V;
			break;
		case OIS::KC_B:
			key=Qt::Key_B;
			break;
		case OIS::KC_N:
			key=Qt::Key_N;
			break;
		case OIS::KC_M:
			key=Qt::Key_M;
			break;
		case OIS::KC_COMMA:
			key=Qt::Key_Comma;
			break;
		case OIS::KC_PERIOD:
			key=Qt::Key_Period;
			break;
		case OIS::KC_SLASH:
			key=Qt::Key_Slash;
			break;
		case OIS::KC_RSHIFT:
			key=Qt::Key_Shift;
			_modifiers^=Qt::ShiftModifier;
			break;
		case OIS::KC_MULTIPLY:
			key=Qt::Key_multiply;
			break;
		case OIS::KC_LMENU:
			key=Qt::Key_Menu;
			break;
		case OIS::KC_SPACE:
			key=Qt::Key_Space;
			break;
		case OIS::KC_CAPITAL:
			key=Qt::Key_CapsLock;
			break;
		case OIS::KC_F1:
			key=Qt::Key_F1;
			break;
		case OIS::KC_F2:
			key=Qt::Key_F2;
			break;
		case OIS::KC_F3:
			key=Qt::Key_F3;
			break;
		case OIS::KC_F4:
			key=Qt::Key_F4;
			break;
		case OIS::KC_F5:
			key=Qt::Key_F5;
			break;
		case OIS::KC_F6:
			key=Qt::Key_F6;
			break;
		case OIS::KC_F7:
			key=Qt::Key_F7;
			break;
		case OIS::KC_F8:
			key=Qt::Key_F8;
			break;
		case OIS::KC_F9:
			key=Qt::Key_F9;
			break;
		case OIS::KC_F10:
			key=Qt::Key_F10;
			break;
		case OIS::KC_NUMLOCK:
			key=Qt::Key_NumLock;
			break;
		case OIS::KC_SCROLL:
			key=Qt::Key_ScrollLock;
			break;
		case OIS::KC_NUMPAD7:
			key=Qt::Key_7;
			break;
		case OIS::KC_NUMPAD8:
			key=Qt::Key_8;
			break;
		case OIS::KC_NUMPAD9:
			key=Qt::Key_9;
			break;
		case OIS::KC_SUBTRACT:
			key=Qt::Key_Minus;
			break;
		case OIS::KC_NUMPAD4:
			key=Qt::Key_4;
			break;
		case OIS::KC_NUMPAD5:
			key=Qt::Key_5;
			break;
		case OIS::KC_NUMPAD6:
			key=Qt::Key_6;
			break;
		case OIS::KC_ADD:
			key=Qt::Key_Plus;
			break;
		case OIS::KC_NUMPAD1:
			key=Qt::Key_1;
			break;
		case OIS::KC_NUMPAD2:
			key=Qt::Key_2;
			break;
		case OIS::KC_NUMPAD3:
			key=Qt::Key_3;
			break;
		case OIS::KC_NUMPAD0:
			key=Qt::Key_0;
			break;
		case OIS::KC_DECIMAL:
			key=Qt::Key_Period;
			break;
		case OIS::KC_F11:
			key=Qt::Key_F11;
			break;
		case OIS::KC_F12:
			key=Qt::Key_F12;
			break;
		case OIS::KC_F13:
			key=Qt::Key_F13;
			break;
		case OIS::KC_F14:
			key=Qt::Key_F14;
			break;
		case OIS::KC_F15:
			key=Qt::Key_F15;
			break;
		case OIS::KC_KANA:
			key=Qt::Key_Kana_Shift;
			break;
		case OIS::KC_YEN:
			key=Qt::Key_yen;
			break;
		case OIS::KC_NUMPADEQUALS:
			key=Qt::Key_Equal;
			break;
		case OIS::KC_AT:
			key=Qt::Key_At;
			break;
		case OIS::KC_COLON:
			key=Qt::Key_Colon;
			break;
		case OIS::KC_UNDERLINE:
			key=Qt::Key_Underscore;
			break;
		case OIS::KC_KANJI:
			key=Qt::Key_Kanji;
			break;
		case OIS::KC_STOP:
			key=Qt::Key_Stop;
			break;
		case OIS::KC_NUMPADENTER:
			key=Qt::Key_Enter;
			break;
		case OIS::KC_RCONTROL:
			key=Qt::Key_Control;
			_modifiers^=Qt::ControlModifier;
			break;
		case OIS::KC_NUMPADCOMMA:
			key=Qt::Key_Comma;
			break;
		case OIS::KC_DIVIDE:
			key=Qt::Key_division;
			break;
		case OIS::KC_SYSRQ:
			key=Qt::Key_SysReq;
			break;
		case OIS::KC_RMENU:
			key=Qt::Key_Menu;
			break;
		case OIS::KC_PAUSE:
			key=Qt::Key_Pause;
			break;
		case OIS::KC_HOME:
			key=Qt::Key_Home;
			break;
		case OIS::KC_UP:
			key=Qt::Key_Up;
			break;
		case OIS::KC_PGUP:
			key=Qt::Key_PageUp;
			break;
		case OIS::KC_LEFT:
			key=Qt::Key_Left;
			break;
		case OIS::KC_RIGHT:
			key=Qt::Key_Right;
			break;
		case OIS::KC_END:
			key=Qt::Key_End;
			break;
		case OIS::KC_DOWN:
			key=Qt::Key_Down;
			break;
		case OIS::KC_PGDOWN:
			key=Qt::Key_PageDown;
			break;
		case OIS::KC_INSERT:
			key=Qt::Key_Insert;
			break;
		case OIS::KC_DELETE:
			key=Qt::Key_Delete;
			break;
		case OIS::KC_LWIN:
			key=Qt::Key_Meta;
			_modifiers^=Qt::MetaModifier;
			break;
		case OIS::KC_RWIN:
			key=Qt::Key_Meta;
			_modifiers^=Qt::MetaModifier;
			break;
		default:
			key=0;
	}
	
	//clear key repeat values
	if(key == _lastkey)
	{
		_lastkeytime = 0;
		_repeating=false;
	}
	
	//generate key release event
	keyReleaseEvent(new QKeyEvent(QEvent::KeyRelease,key,_modifiers,t,true,1));
	
}

void OgreQtOverlay::mouseMoved(const OIS::MouseEvent &e)
{
	float x = e.state.X.abs; // / (float)e.state.width;
	float y = e.state.Y.abs; // / (float)e.state.height;
	
	_cursor->setPos(x,y);
	
	//Generate mouse move event
	QGraphicsSceneMouseEvent ev(QEvent::GraphicsSceneMouseMove);
	ev.setScenePos(QPointF(x,y));
	ev.setScreenPos(QPoint(e.state.X.abs,e.state.Y.abs));
	ev.setLastScenePos(_lastscenepos);
	ev.setLastScreenPos(_lastscreenpos);
	ev.setButtons(_buttons);
	ev.setModifiers(_modifiers);
	ev.setAccepted(false);
	QApplication::sendEvent(this,&ev);	
	
	_lastscenepos = QPointF(x,y);
	_lastscreenpos = QPoint(e.state.X.abs,e.state.Y.abs);
}
 
void OgreQtOverlay::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	float x = e.state.X.abs; // (float)e.state.width;
	float y = e.state.Y.abs; // (float)e.state.height;

	
	Qt::MouseButton button;
	
	//convert OIS buttons to QT buttons
	switch(id)
	{
		case 0:
			button = Qt::LeftButton;
			break;
		case 1:
			button = Qt::RightButton;
			break;
		case 2:
			button = Qt::MidButton;
			break;
		case 3:
			button = Qt::MidButton;
			break;
		case 4:
			button = Qt::XButton1;
			break;
		default:
			button = Qt::XButton2;
			break;
	}
	
	_buttons|=button;
	
	//generate press event
	QGraphicsSceneMouseEvent ev(QEvent::GraphicsSceneMousePress);
	ev.setButtonDownScenePos(button,QPointF(x,y));
	ev.setButtonDownScreenPos(button,QPoint(e.state.X.abs,e.state.Y.abs));
	ev.setScenePos(QPointF(x,y));
	ev.setScreenPos(QPoint(e.state.X.abs,e.state.Y.abs));
	ev.setLastScenePos(_lastscenepos);
	ev.setLastScreenPos(_lastscreenpos);
	ev.setButtons(_buttons);
	ev.setButton(button);
	ev.setModifiers(_modifiers);
	ev.setAccepted(false);
	QApplication::sendEvent(this,&ev);
	
	//do double click events
	if((_time.elapsed() - _lastclicktime) <= qApp->doubleClickInterval())
	{
		QGraphicsSceneMouseEvent ev(QEvent::GraphicsSceneMouseDoubleClick);
		ev.setButtonDownScenePos(button,QPointF(x,y));
		ev.setButtonDownScreenPos(button,QPoint(e.state.X.abs,e.state.Y.abs));
		ev.setScenePos(QPointF(x,y));
		ev.setScreenPos(QPoint(e.state.X.abs,e.state.Y.abs));
		ev.setLastScenePos(_lastscenepos);
		ev.setLastScreenPos(_lastscreenpos);
		ev.setButtons(_buttons);
		ev.setButton(button);
		ev.setModifiers(_modifiers);
		ev.setAccepted(false);
		QApplication::sendEvent(this,&ev);
	}
	
	_lastclicktime = _time.elapsed();
}

void OgreQtOverlay::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
	float x = e.state.X.abs; // (float)e.state.width;
	float y = e.state.Y.abs; // (float)e.state.height;
	
	Qt::MouseButton button;
	
	//convert OIS buttons to QT buttons
	switch(id)
	{
		case 0:
			button = Qt::LeftButton;
			break;
		case 1:
			button = Qt::RightButton;
			break;
		case 2:
			button = Qt::MidButton;
			break;
		case 3:
			button = Qt::MidButton;
			break;
		case 4:
			button = Qt::XButton1;
			break;
		default:
			button = Qt::XButton2;
			break;
	}
	
	_buttons^=button;
	
	//generate key release event
	QGraphicsSceneMouseEvent ev(QEvent::GraphicsSceneMouseRelease);
	ev.setButtonDownScenePos(button,QPointF(x,y));
	ev.setButtonDownScreenPos(button,QPoint(e.state.X.abs,e.state.Y.abs));
	ev.setScenePos(QPointF(x,y));
	ev.setScreenPos(QPoint(e.state.X.abs,e.state.Y.abs));
	ev.setLastScenePos(_lastscenepos);
	ev.setLastScreenPos(_lastscreenpos);
	ev.setButtons(_buttons);
	ev.setButton(button);
	ev.setModifiers(_modifiers);
	ev.setAccepted(false);
	QApplication::sendEvent(this,&ev);
}

//Render the QGraphicsScene into a texture
void OgreQtOverlay::sceneChanged(QList<QRectF> list)
{	
	//loop through all screen areas that changed
	foreach(QRectF rect, list)
	{
		//generate a rectangle that is a while number, and fits withing the screen
		if(rect.x() < 0) rect.setX(0);
		if(rect.y() < 0) rect.setY(0);
		if(rect.right() > _width) rect.setRight(_width);
		if(rect.bottom() > _height) rect.setRight(_height);
		rect = QRectF(round(rect.x()),round(rect.y()),round(rect.width()),round(rect.height()));
		
		//Create an image, create a qpainter, fill the image with transparent color, and then render a part of the scene to it
		QImage image(rect.width(),rect.height(),QImage::Format_ARGB32);
		QPainter p(&image);
		image.fill(0);
		render(&p,image.rect(),rect);
		
		//use blitFromMemory to copy the image into the right part of the texture
		Box b(rect.x(),rect.y(),rect.x() + rect.width(),rect.y() + rect.height());
		PixelBox pb(Box(0,0,rect.width(),rect.height()),PF_A8R8G8B8,image.bits());
		_buffer->blitFromMemory(pb,b);
	}
}

//Set weather the overlay is visible
void OgreQtOverlay::setVisible(bool visible)
{
	_visible = visible;
	if(_visible)
	{
		_overlay->show();
	}
	else
	{
		_overlay->hide();
	}
}
