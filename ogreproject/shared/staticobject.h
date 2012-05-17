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
#ifndef STATICOBJECT_H
#define STATICOBJECT_H

#include <entity.h>
#include <Ogre.h>

/**
	@author Brendan Powers <brendan@resara.com>
*/
class StaticObject : public Entity
{
DECLARE_NETVARIABLES(StaticObject, Entity, Position, Orientation, Scale, Mesh);
public:
    StaticObject();
    StaticObject(QString mesh);
    ~StaticObject();

    virtual bool onGhostAdd(TNL::GhostConnection *theConnection);
    void setPosition(Ogre::Vector3 position);
    void setOrientation(Ogre::Quaternion orientation);
    void setScale(Ogre::Vector3 orientation);
    
private:
	void positionChanged();
	void orientationChanged();
	void scaleChanged();
	
	Ogre::Vector3 _position;
	Ogre::Quaternion _orientation;
	Ogre::Vector3 _scale;
	Ogre::SceneNode *_node;
	QString _mesh;
	static entnum;
};

#endif
