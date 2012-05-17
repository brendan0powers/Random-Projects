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
#include "playercamera.h"
#include "player.h"
#include <QDebug>
#ifdef CLIENT_SIDE
	#include "application.h"
#endif

int entnum=0;

PlayerCamera::PlayerCamera()
 : Entity()
{
	_node = NULL;
}

START_IMPLEMENT_NETVARIABLES(PlayerCamera);
IMPLEMENT_NETVARIABLE_CALLBACK(Position,positionChanged, _position);
IMPLEMENT_NETVARIABLE_CALLBACK(Orientation,orientationChanged, _orientation);
END_IMPLEMENT_NETVARIABLES;

PlayerCamera::~PlayerCamera()
{
}


bool PlayerCamera::onGhostAdd(TNL::GhostConnection *theConnection)
{
	if(!Parent::onGhostAdd(theConnection))
		return false; //A parent had an error
	
#ifdef CLIENT_SIDE
	if(owner() == Player::myId())
	{
		qDebug() << "========= Got My Camera";
		return(true);
	}
	else
	{
		qDebug() << "========= Got user camera:" << owner();
		Ogre::Entity                   *entity;
		char entname[100];
		snprintf(entname, 100, "crateent%i", entnum);
		entity = Application::instance()->getScene()->createEntity(entname, "cube.mesh");
		entity->setQueryFlags(1 << 2);
		char entname2[100];
		snprintf(entname2, 100, "crateent%i", entnum);
		_node = Application::instance()->getScene()->getRootSceneNode()->createChildSceneNode(entname2);
		_node->attachObject(entity);
		entnum++;
	}
#endif
	return(true);
}

void PlayerCamera::positionChanged()
{
#ifdef CLIENT_SIDE
	if(_node != NULL) _node->setPosition(_position);
#endif
}

void PlayerCamera::orientationChanged()
{
#ifdef CLIENT_SIDE
	if(_node != NULL) _node->setOrientation(_orientation);
#endif	
}

void PlayerCamera::update(Ogre::Vector3 position, Ogre::Quaternion orientation)
{
	_position=position;
	_orientation=orientation;
	setMaskBits(Position);
	setMaskBits(Orientation);
}