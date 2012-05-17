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
#ifndef OGREQTOVERLAY_H
#define OGREQTOVERLAY_H

#include <QGraphicsScene>
#include <QImage>
#include <QTime>
#include <Ogre.h>
#include <OIS/OIS.h>

/**
	@author Brendan Powers <brendan@resara.com>
*/
class OgreQtOverlay : public QGraphicsScene, public Ogre::FrameListener
{
Q_OBJECT
public:
	OgreQtOverlay(Ogre::Root *root, Ogre::RenderWindow *window, uint w, uint h, QObject *parent = 0);
    ~OgreQtOverlay();
    
    bool frameRenderingQueued(const Ogre::FrameEvent &evt);
    void keyPressed(const OIS::KeyEvent &e);
    void keyReleased(const OIS::KeyEvent &e);
    void mouseMoved(const OIS::MouseEvent &e);
    void mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    void mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    void setVisible(bool visible);
    
protected slots:
	void sceneChanged(QList<QRectF> list);
	
	
private:
	Ogre::Root *_root;
	Ogre::RenderWindow *_window;
	Ogre::Texture *_texture;
	Ogre::Overlay *_overlay;
	Ogre::HardwarePixelBuffer *_buffer;
	uint _width;
	uint _height;
	Qt::KeyboardModifiers _modifiers;
	Qt::MouseButtons _buttons;
	QPointF _lastscenepos;
	QPoint _lastscreenpos;
	QGraphicsItem *_cursor;
	int _lastkey;
	QString _lastchar;
	time_t _lastkeytime;
	bool _repeating;
	time_t _lastclicktime;
	QTime _time;
	bool _visible;
};

#endif
