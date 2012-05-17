/***************************************************************************
 *   Copyright (C) 2008 by Brendan Powers   *
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
#include "mapservice.h"

MapService::MapService(QObject *parent)
 : ServiceProxy(parent)
{
}


MapService::~MapService()
{
}


ReturnValue MapService::auth(QString, QString)
{
	_netfs = (NetFs *)getData("netfs");
	return(true);
}


ReturnValue MapService::map(QString name, QString type, QString src, QStringList options)
{
	return(_netfs->map(name,type,src,options));
}


ReturnValue MapService::unMap(QString name)
{
	return(_netfs->unMap(name));
}
