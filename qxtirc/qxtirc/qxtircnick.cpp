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
#include "qxtircnick.h"
#include "qxtircnick_p.h"

#include <QDebug>

QxtIrcNick::QxtIrcNick()
{
	QXT_INIT_PRIVATE(QxtIrcNick);
	
	qxt_d().nick = "";
	qxt_d().ident = "";
}

QxtIrcNick::QxtIrcNick(QString nick)
{
	QXT_INIT_PRIVATE(QxtIrcNick);
	
	qxt_d().parse(nick);
	
	qDebug() << "Constructing nick" << nick << toString();
}

QxtIrcNick::QxtIrcNick(const QxtIrcNick &nick)
{
	QXT_INIT_PRIVATE(QxtIrcNick);
	qxt_d().nick = nick.qxt_d().nick;
	qxt_d().ident = nick.qxt_d().ident;
	qxt_d().host = nick.qxt_d().host;
}

QxtIrcNick::~QxtIrcNick()
{
}

QString QxtIrcNick::nick() const
{
	return(qxt_d().nick);
}

QString QxtIrcNick::ident() const
{
	return(qxt_d().ident);
}

QString QxtIrcNick::host() const
{
	return(qxt_d().host);
}

void QxtIrcNick::setNick(QString n)
{
	qxt_d().nick = n;
}

void QxtIrcNick::setIdent(QString i)
{
	qxt_d().ident = i;
}

void QxtIrcNick::setHost(QString h)
{
	qxt_d().host = h;
}

QString QxtIrcNick::toString() const
{
	QString n = qxt_d().nick;

	if (qxt_d().ident != "")
	{
		n += "!" + qxt_d().ident;
	}

	if (qxt_d().host != "")
	{
		n += "@" + qxt_d().host;
	}

	return(n);
}

void QxtIrcNickPrivate::parse(QString line)
{
	int iindex = line.indexOf('!');
	int hindex = line.indexOf('@');

	//Defaults
	nick = "";
	ident = "";
	host = "";

	//get the nick
	if (iindex == -1 && hindex == -1)
	{
		nick = line;
		return;
	}
	if (iindex != -1)
	{
		nick = line.left(iindex);
		line = line.mid(iindex + 1);
	}
	else if (hindex != 1)
	{
		nick = line.left(hindex);
		line = line.mid(hindex + 1);
	}

	//Parse ident, if its there
	if (iindex != -1)
	{
		hindex = line.indexOf('@');
		if (hindex == -1)
		{
			ident = line;
			return;
		}
		else
		{
			ident = line.left(hindex);
			line = line.mid(hindex + 1);
		}
	}

	//if we got here, then the host is all that's left
	host = line;
}

QxtIrcNick &QxtIrcNick::operator=(const QString & nick)
{
	qxt_d().parse(nick);
	return *this;
}


QxtIrcNick &QxtIrcNick::operator=(const QxtIrcNick & nick)
{
	qDebug() << "Copying NICK";
	qxt_d().nick = nick.qxt_d().nick;
	qxt_d().ident = nick.qxt_d().ident;
	qxt_d().host = nick.qxt_d().host;
	return *this;
}


bool QxtIrcNick::operator==(const QString &nick)
{
	return operator==(QxtIrcNick(nick));
}

bool QxtIrcNick::operator==(const QxtIrcNick &nick)
{
	return(qxt_d().nick == nick.qxt_d().nick);
}
