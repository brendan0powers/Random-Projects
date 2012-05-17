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
#ifndef QXTIRCMESSAGE_H
#define QXTIRCMESSAGE_H

#include <QString>
#include <QStringList>
#include "qxtircnick.h"
#include <qxtpimpl.h>

class QxtIrcMessagePrivate;

/**
	@author Brendan Powers <brendan@resara.com>
*/
class QxtIrcMessage
{
public:
	QxtIrcMessage();
	QxtIrcMessage(QString msg);
	QxtIrcMessage(QString f, QString t, QString c, QStringList a);
	QxtIrcMessage(QString f, QString t, QString c, QString a);
	QxtIrcMessage(const QxtIrcMessage &msg);
	~QxtIrcMessage();

	QxtIrcNick from() const;
	QxtIrcNick to() const;
	QString cmd() const;
	int toNumber() const;
	QStringList args() const;
	void setFrom(QxtIrcNick f);
	void setTo(QxtIrcNick t);
	void setCmd(QString c);
	void setArgs(QStringList a);
	QString toString() const;

private:
	QXT_DECLARE_PRIVATE(QxtIrcMessage);
	
// 	QxtIrcNick _from;
// 	QxtIrcNick _to;
// 	QString _cmd;
// 	QStringList _args;
// 	void parse(QString msg);
// 	int nextChar(int start, int end, const char *buf);
};

#endif

