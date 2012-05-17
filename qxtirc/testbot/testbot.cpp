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
#include "testbot.h"
#include <QDebug>
#include <qxtircnick.h>

TestBot::TestBot(QObject *parent)
		: QObject(parent),
		connection(this)
{
	connection.setNickName("testbot");
	connection.setRealName("Chris IrcBot");
	connect(&connection, SIGNAL(stateChanged(QxtIrcConnection::State)), this, SLOT(stateChanged(QxtIrcConnection::State)));
	connect(&connection, SIGNAL(messageReceived(QxtIrcMessage)), this, SLOT(messageReceived(QxtIrcMessage)));
	connect(&connection, SIGNAL(userJoined(QString, QxtIrcNick)), this, SLOT(userJoined(QString, QxtIrcNick)));
}

TestBot::~TestBot()
{
}

void TestBot::init(QString host, int port)
{
	connection.connectToHost(host, port, false);
}

void TestBot::stateChanged(QxtIrcConnection::State state)
{
	qDebug() << "State has changed!";
	switch (state)
	{
		case QxtIrcConnection::Connected:
			connection.sendMessage("NickServ", "id grills");
			connection.joinChannel("#bots");
			connection.sendMessage("#bots", "Testbot initialized! Lets hope i work!!");
			break;
		default:
			break;
	}
}

void TestBot::messageReceived(QxtIrcMessage msg)
{
	if(msg.from() == QxtIrcNick(":NickServ!NickServ@services.int"))
	{
		qDebug() << "Message from nickserv, parsing...";
		connection.sendMessage("NickServ", "id grills");
	}
}

void TestBot::userJoined(QString chan, QxtIrcNick nick)
{
	if(chan == "#bots")
	{
		connection.sendMessage(chan, nick.nick()+": Oh hey, welcome to "+chan+"... I have given you operator status so that you can test your bots correctly. Please don't break anything or kick this bot, or you will be banned from this channel.");
		connection.writeMessage("MODE", QStringList() << "#bots" << "+o" << nick.nick());
	}
}



