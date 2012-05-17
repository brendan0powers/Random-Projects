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
#include <QString>

#include "testbot.h"
#include <QxtIrcMessage>
#include <QDebug>

/**
This bot is used for managing #bots channel. He ops everyone who enters the room. Please don't fuck with him, kick him out of the channel, or anything else dumb like that.
*/

int main(int argc, char** argv)
{
	QCoreApplication app(argc, argv);
	
	QxtIrcMessage *msg1 = new QxtIrcMessage("asfd asdf asdfasdf asdfafd");
	QxtIrcMessage msg2 = *msg1;
	qDebug() << "1" << msg1->toString();
	qDebug() << "2" << msg2.toString();
	delete msg1;
	qDebug() << "2" << msg2.toString();
	
	QString hostname = "rkon.homeip.net";
	int port = 6667;
	if (argc > 1)
		hostname = argv[1];
	if (argc > 2)
		port = QString(argv[2]).toInt();
	TestBot bot(&app);
	bot.init(hostname, port);
	return app.exec();
}
