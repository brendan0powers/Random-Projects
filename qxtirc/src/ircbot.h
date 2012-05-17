/***************************************************************************
 *   Copyright (C) 2009 by Chris Vickery   *
 *   chrisinajar@gmail.com   *
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
#ifndef IRCBOT_H
#define IRCBOT_H

#include <QObject>
#include <qxtircconnection.h>

/**
	@author Chris Vickery <chrisinajar@gmail.com>
*/
class IrcBot : public QObject
{
	Q_OBJECT
public:
	IrcBot(QObject *parent = 0);
	void connectToIrc();
	~IrcBot();

public slots:
	void stateChanged(QxtIrcConnection::State);
	void messageReceived(QxtIrcMessage);
	void userJoined(QString chan, QxtIrcNick nick);
	void interval();

protected:
	QxtIrcConnection client;

};

#endif
