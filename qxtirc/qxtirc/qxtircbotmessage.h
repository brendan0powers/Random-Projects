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
#ifndef QXTIRCBOTMESSAGE_H
#define QXTIRCBOTMESSAGE_H

#include <QString>
#include <QStringList>
#include <qxtircnick.h>
#include <qxtircmessage.h>
#include <qxtircconnection.h>
#include <qxtpimpl.h>

class QxtIrcBotMessagePrivate;

/**
	@author Brendan Powers <brendan@resara.com>
*/
class QxtIrcBotMessage
{
public:
	QxtIrcBotMessage();
	QxtIrcBotMessage(QString msg, QString filter, QxtIrcMessage message, QxtIrcConnection::MessageScope scope);
	QxtIrcBotMessage(const QxtIrcBotMessage &msg);
	~QxtIrcBotMessage();
	
	QxtIrcNick to() const;
	QxtIrcNick from() const;
	QStringList args() const;
	QString filter() const;
	QString msgText() const;
	QxtIrcMessage msg() const;
	QxtIrcConnection::MessageScope scope() const;
	QString reply() const;
	QString replyPrivate() const;
	QString replyChannel() const;
	bool match(QString format);
	bool match(QStringList parts, QStringList args);
	bool matchAll(QString format);
	
private:
	QXT_DECLARE_PRIVATE(QxtIrcBotMessage);
	
// 	void parse(QString msg);
// 	int nextChar(int start, int end, const char *buf);
// 	QxtIrcNick _from;
// 	QxtIrcNick _to;
// 	QStringList _args;
// 	QString _filter;
// 	QString _msgtext;
// 	QxtIrcMessage _msg;
// 	QxtIrcConnection::MessageScope _scope;
};

#endif
