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
#ifndef QXTIRCNICK_H
#define QXTIRCNICK_H

#include <QString>
#include <qxtpimpl.h>

class QxtIrcNickPrivate;

/**
	@author Brendan Powers <brendan@resara.com>
*/
class QxtIrcNick
{
public:
	QxtIrcNick();
	QxtIrcNick(QString nick);
	QxtIrcNick(const QxtIrcNick &nick);

	~QxtIrcNick();
	QString nick() const;
	QString ident() const;
	QString host() const;
	void setNick(QString n);
	void setIdent(QString i);
	void setHost(QString h);
	QString toString() const;

	QxtIrcNick &operator=(const QString &nick);
	QxtIrcNick &operator=(const QxtIrcNick &nick);
	bool operator==(const QString &nick);
	bool operator==(const QxtIrcNick &nick);

private:
	QXT_DECLARE_PRIVATE(QxtIrcNick);
	
// 	QString _nick;
// 	QString _ident;
// 	QString _host;
// 	void parse(QString nick);
};

#endif

