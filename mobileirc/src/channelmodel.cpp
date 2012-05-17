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
//Ok, you can copy that floppy
#include "channelmodel.h"

ChannelModel::ChannelModel(QxtIrcChannel *chan, QObject *parent)
		: QAbstractListModel(parent)
{
	_channel = chan;
}


ChannelModel::~ChannelModel()
{
}


QVariant ChannelModel::data(const QModelIndex &index, int role) const
{
	if(!index.isValid())
		return(QVariant());
	
	if(role == Qt::DisplayRole)
	{
		if(index.column() == 0)
		{
			QList<QxtIrcNick> nicks = _channel->listUsers();
			if(index.row() >= nicks.count())
				return(QVariant());
			
			return(nicks[index.row()].nick());
		}
	}
	
	return(QVariant());
}

int ChannelModel::rowCount(const QModelIndex &parent) const
{
	return(_channel->listUsers().count());
}

int ChannelModel::columnCount ( const QModelIndex & parent) const
{
	return(1);
}

QVariant ChannelModel::headerData ( int section, Qt::Orientation orientation, int role) const
{
	if(orientation != Qt::Horizontal) return(QVariant());
	if(role != Qt::DisplayRole) return(QVariant());
	if(section == 0) return("Users");
}

QString ChannelModel::getNickFromIndex(QModelIndex index)
{
	QList<QxtIrcNick> nicks = _channel->listUsers();
	if(index.row() >= nicks.count())
		return("");
			
	return(nicks[index.row()].nick());
}

void ChannelModel::update()
{
	reset();
}
