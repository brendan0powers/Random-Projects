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
#ifndef CHANNELBASE_H
#define CHANNELBASE_H

#include <QString>
#include "mainwindow.h"

/**
	@author Brendan Powers <brendan@resara.com>
*/
class ChannelBase
{
public:
	
	ChannelBase();
	~ChannelBase();

	virtual void send() {}
	virtual void clear() {}
        virtual void append(QString msg) {}
	virtual MainWindow::Scope type() {return MainWindow::None;}
        virtual void focus() {}
        virtual void showNickList(bool show) {}
	virtual void scroll(bool up) {}
	virtual QString name() { return ""; }
};

#endif
