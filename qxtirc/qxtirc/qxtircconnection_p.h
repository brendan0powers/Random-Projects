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

#ifndef QXTIRCCONNECTION_P_H
#define QXTIRCCONNECTION_P_H

#include <QTimer>

class QxtIrcConnectionPrivate : public QxtPrivate<QxtIrcConnection>
{
public:
	QxtIrcConnectionPrivate() {}
	QXT_DECLARE_PUBLIC(QxtIrcConnection);
	
	class Filter
	{
		public:
			Filter() {}
			QxtIrcConnection::MessageScope scope;
			QString filter;
			QObject *obj;
			QString slot;
	};
	
	void setState(QxtIrcConnection::State);
	void testFilter(QxtIrcMessage msg);
	QxtIrcConnection::MessageScope getScope(QxtIrcMessage msg);
	QString nickname;
	QString realname;
	QTcpSocket* socket;
	QxtIrcConnection::State state;
	QHash<QString, QPointer<QxtIrcChannel> > channels;
	QString password;
	QList<Filter> filters;
	QHash<QChar, QxtIrcConnection::ChannelAccess> accessPrefix;
	QHash<QString, QVariant> serverConfig;
	int timeout;
	QTimer timeouttimer;
};

#endif
