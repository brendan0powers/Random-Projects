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
#include "qxtircchannel.h"
#include "qxtircchannel_p.h"
#include <qxtircconnection.h>
#include <QDebug>

QxtIrcChannel::QxtIrcChannel(QString c, QxtIrcConnection *parent)
		: QObject(parent)
{
	QXT_INIT_PRIVATE(QxtIrcChannel);
	qxt_d().name = c;
}

QxtIrcChannel::~QxtIrcChannel()
{
}

QList<QxtIrcNick> QxtIrcChannel::listUsers()
{
	return qxt_d().users;
}

QString QxtIrcChannel::channelName()
{
	return qxt_d().name;
}

QString QxtIrcChannel::topic()
{
	return(qxt_d().topic);
}

void QxtIrcChannel::joinUser(QxtIrcNick user)
{
	qDebug() << "Adding" << user.nick();
	if(!qxt_d().users.contains(user))
		qxt_d().users << user;
	emit(userJoined(user));
}

void QxtIrcChannel::partUser(QxtIrcNick user, QString reason)
{
	qxt_d().users.removeAll(user);
	emit(userParted(user,reason));
}

void QxtIrcChannel::setTopic(QString topic)
{
	qxt_d().topic=topic;
	emit(topicChanged(qxt_d().name,topic));
}

void QxtIrcChannel::changeNick(QxtIrcNick oldnick, QxtIrcNick newnick)
{
	if(!qxt_d().users.contains(oldnick)) return;
	qxt_d().users.removeAll(oldnick);
	qxt_d().users << newnick;
	emit(nickChanged(oldnick,newnick));
}

void QxtIrcChannel::kickUser(QxtIrcNick user, QString reason, QxtIrcNick from)
{
	if(!qxt_d().users.contains(user)) return;
	qxt_d().users.removeAll(user);
	emit(userKicked(user,reason,from));
}

void QxtIrcChannel::killUser(QxtIrcNick user, QString reason, QxtIrcNick from)
{
	if(!qxt_d().users.contains(user)) return;
	qxt_d().users.removeAll(user);
	emit(userKilled(user,reason,from));
}

void QxtIrcChannel::quitUser(QxtIrcNick user, QString reason)
{
	if(!qxt_d().users.contains(user)) return;
	qxt_d().users.removeAll(user);
	emit(userQuit(user,reason));
}

void QxtIrcChannel::sendMessage(QxtIrcMessage msg)
{
	emit(newMessage(msg));
}

void QxtIrcChannel::sendNotice(QxtIrcMessage msg)
{
	emit(newNotice(msg));
}
