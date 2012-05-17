/***************************************************************************
 *   Copyright (C) 2009 by Brendan Powers   *
 *   brendan@resara.com   *
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
//DON'T COPY THAT FLOPPY
#ifndef COMMANDEDIT_H
#define COMMANDEDIT_H

#include <QLineEdit>
#include <QList>
#include <QxtIrcNick>
#include <QQueue>

/**
	@author Brendan Powers <brendan@resara.com>
*/
class CommandEdit : public QLineEdit
{
	Q_OBJECT
public:
	CommandEdit(QWidget *parent = 0);
	~CommandEdit();

	virtual bool event(QEvent *event);
	void setNickList(QList<QxtIrcNick> nicks);
	void setHistoryLimit(int num);
	void addToHistory();
	
private:
	QList<QxtIrcNick> _nicks;
	QQueue<QString> _history;
	int _historylimit;
	QString _currenttext;
	int _currentitem;
        QStringList _irccommands;
};

#endif
