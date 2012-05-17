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


#include <QCoreApplication>
#include <QDebug>
#include "ircbot.h"

int dothatthing(int t)
{

	int attempts, timeout;
	attempts = t / 5000;
	if (attempts < 1)
	{
		attempts = 1;
		timeout = t;
	}
	else
	{
		timeout = 5000 + ((t % 5000) / attempts);
	}
	qDebug() << timeout;
	return attempts * timeout;
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	IrcBot irc(&app);

	/*
	for(int i = 100; i < 30000; i += 123)
	{
		if(((i - dothatthing(i)) > 4) || ((i - dothatthing(i)) < -4))
		{
			qDebug() << "FUCK" << (i - dothatthing(i));
		}
	}
	*/

	return app.exec();
}

