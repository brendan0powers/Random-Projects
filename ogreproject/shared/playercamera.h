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
#ifndef PLAYERCAMERA_H
#define PLAYERCAMERA_H

#include <entity.h>
#include <Ogre.h>

/**
	@author Brendan Powers <brendan@resara.com>
*/
class PlayerCamera : public Entity
{
DECLARE_NETVARIABLES(PlayerCamera, Entity, Position, Orientation);
public:
    PlayerCamera();
    ~PlayerCamera();

    virtual bool onGhostAdd(TNL::GhostConnection *theConnection);
    void update(Ogre::Vector3 position, Ogre::Quaternion orientation);
    
private:
	void positionChanged();
	void orientationChanged();
	
	Ogre::Vector3 _position;
	Ogre::Quaternion _orientation;
	Ogre::SceneNode *_node;
};

#endif
