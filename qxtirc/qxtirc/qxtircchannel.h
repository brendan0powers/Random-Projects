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
#ifndef QXTIRCCHANNEL_H
#define QXTIRCCHANNEL_H

#include <QObject>
#include <qxtircnick.h>
#include "qxtircmessage.h"
#include <qxtpimpl.h>

class QxtIrcConnection;
class QxtIrcChannelPrivate;

/**
	@author Chris Vickery <chrisinajar@gmail.com>
*/
class QxtIrcChannel : public QObject
{
	Q_OBJECT
public:
	QxtIrcChannel(QString c, QxtIrcConnection *parent);
	~QxtIrcChannel();
	QList<QxtIrcNick> listUsers();
	QString channelName();
	QString topic();
	
public slots:
	void joinUser(QxtIrcNick user);
	void partUser(QxtIrcNick user, QString reason);
	void setTopic(QString topic);	
	void changeNick(QxtIrcNick oldnick, QxtIrcNick newnick);
	void kickUser(QxtIrcNick user, QString reason, QxtIrcNick from);
	void killUser(QxtIrcNick user, QString reason, QxtIrcNick from);
	void quitUser(QxtIrcNick user, QString reason);
	void sendMessage(QxtIrcMessage msg);
	void sendNotice(QxtIrcMessage msg);
	
signals:
	void userJoined(QxtIrcNick user);
	void userParted(QxtIrcNick user, QString reason);
	void topicChanged(QString channel, QString topic);
	void nickChanged(QxtIrcNick oldnick, QxtIrcNick newnick);
	void userKicked(QxtIrcNick user, QString reason, QxtIrcNick from);
	void userKilled(QxtIrcNick user, QString reason, QxtIrcNick from);
	void userQuit(QxtIrcNick user, QString reason);
	void newMessage(QxtIrcMessage msg);
	void newNotice(QxtIrcMessage msg);
	
protected:
	QXT_DECLARE_PRIVATE(QxtIrcChannel);
	
// 	QString _name;
// 	QList<QxtIrcNick> _users;
// 	QString _topic;
};

#endif
