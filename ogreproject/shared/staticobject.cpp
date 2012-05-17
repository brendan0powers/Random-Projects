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
#include "staticobject.h"
#include "player.h"
#include <QDebug>
#ifdef CLIENT_SIDE
	#include "application.h"
#endif

int StaticObject::entnum=0;

StaticObject::StaticObject(QString mesh)
 : Entity()
{
	_mesh = mesh;
	_node = NULL;
}

StaticObject::StaticObject()
{
	_node = NULL;
}

START_IMPLEMENT_NETVARIABLES(StaticObject);
IMPLEMENT_NETVARIABLE_CALLBACK(Position,positionChanged, _position);
IMPLEMENT_NETVARIABLE_CALLBACK(Orientation,orientationChanged, _orientation);
IMPLEMENT_NETVARIABLE_CALLBACK(Scale,scaleChanged, _scale);
IMPLEMENT_NETVARIABLE(Mesh,_mesh);
END_IMPLEMENT_NETVARIABLES;

StaticObject::~StaticObject()
{
}


bool StaticObject::onGhostAdd(TNL::GhostConnection *theConnection)
{
	if(!Parent::onGhostAdd(theConnection))
		return false; //A parent had an error
	
#ifdef CLIENT_SIDE
		qDebug() << "Creating static object";
		Ogre::Entity *entity;
		char entname[100];
		snprintf(entname, 100, "crateent-static%i", entnum);
		entity = Application::instance()->getScene()->createEntity(entname, "cube.mesh");
		entity->setQueryFlags(1 << 2);
		char entname2[100];
		snprintf(entname2, 100, "crateent-static%i", entnum);
		_node = Application::instance()->getScene()->getRootSceneNode()->createChildSceneNode(entname2);
		_node->attachObject(entity);
		entnum++;
#endif
	return(true);
}

void StaticObject::positionChanged()
{
#ifdef CLIENT_SIDE
	if(_node != NULL) _node->setPosition(_position);
#endif
}

void StaticObject::orientationChanged()
{
#ifdef CLIENT_SIDE
	if(_node != NULL) _node->setOrientation(_orientation);
#endif	
}

void StaticObject::scaleChanged()
{
#ifdef CLIENT_SIDE
	if(_node != NULL) _node->setScale(_scale);
#endif		
}

void StaticObject::setPosition(Ogre::Vector3 position)
{
	_position=position;
	setMaskBits(Position);
}
		
void StaticObject::setOrientation(Ogre::Quaternion orientation)
{
	_orientation=orientation;
	setMaskBits(Orientation);
}

void StaticObject::setScale(Ogre::Vector3 scale)
{
	_scale = scale;
	setMaskBits(Scale);
}