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
#include "ircbot.h"
#include <qxtircconnection.h>
#include <QDebug>
#include <QTimer>

IrcBot::IrcBot(QObject *parent)
		: QObject(parent)
{
	client.setNickName("chris_qtbot");
	client.setRealName("Robo McRobostein");
	client.connectToHost("rkon.homeip.net", 6667, false);
	connect(&client, SIGNAL(stateChanged(QxtIrcConnection::State)), this, SLOT(stateChanged(QxtIrcConnection::State)));
	connect(&client, SIGNAL(messageReceived(QxtIrcMessage)), this, SLOT(messageReceived(QxtIrcMessage)));
	connect(&client, SIGNAL(userJoined(QString, QxtIrcNick)), this, SLOT(userJoined(QString, QxtIrcNick)));
	QTimer* t = new QTimer(this);
	t->setInterval(20000);
	connect(t, SIGNAL(timeout()), this, SLOT(interval()));
	t->start();
}

IrcBot::~IrcBot()
{
}

void IrcBot::interval()
{
	QList<QxtIrcNick> users = client.getChannel("#bots")->listUsers();
	qDebug() << "Channel has" << users.count() << "users online";
	foreach(QxtIrcNick n, users)
	{
		qDebug() << "User:" << n.nick();
	}
}

void IrcBot::stateChanged(QxtIrcConnection::State state)
{
	qDebug() << "State has changed!";
	switch (state)
	{
		case QxtIrcConnection::Connected:

			client.joinChannel("#bots");
			client.sendMessage("#bots", "I have come from the world of C++! This is a real IRC client bot, no more autojoin!");
			break;
		default:
			break;
	}
}

void IrcBot::messageReceived(QxtIrcMessage)
{
}

void IrcBot::userJoined(QString chan, QxtIrcNick nick)
{
	switch (client.getAccountStatus(nick))
	{
		case QxtIrcConnection::Identified:
			client.sendMessage(chan, nick.nick() + ": We bots welcome those who are registered!!");
			break;
		case QxtIrcConnection::Recognized:
			client.sendMessage(chan, nick.nick() + ": hmm... You appear to be registered and recognized, but I would really like it if you identified.");
			break;
		case QxtIrcConnection::Registered:
			client.sendMessage(chan, nick.nick() + ": Your nickname is registered, but you're not logged in... We look down on that...");
			break;
		case QxtIrcConnection::NotRegistered:
			client.sendMessage(chan, nick.nick() + ": PLEASE register your nickname, or seriously gtfo.");
			break;
		default:
			client.sendMessage(chan, nick.nick() + ": undefined account status");
			break;
	}
}


