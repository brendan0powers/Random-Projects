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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_MainWindow.h"
#include <QxtIrcConnection>
#include <QList>
#include <QHash>

class ChannelBase;
class LogWidget;
class ChannelWidget;
class PrivateWidget;


class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

public:
	enum Scope
	{
		None,
		Server,
		Channel,
		Private
	};

	MainWindow(QWidget* parent = 0, Qt::WFlags fl = 0);
	~MainWindow();

public slots:
	void appendAll(QString msg);

protected slots:
	void connect();
	void disconnect();
	void stateChanged(QxtIrcConnection::State state);
	void messageReceived(QxtIrcMessage msg);
	void joinedChannel(QString name);
	void partedChannel(QString name);
	void kicked(QString name);
	void nickChanged(QxtIrcNick nick);
	void send();
	void channelChanged(int index);
	void channelActivated(QWidget *w);
	bool event(QEvent *event);
	bool eventFilter(QObject *obj, QEvent *event);
	void nickMentioned(ChannelWidget *channel);
	void newMessage(ChannelWidget *channel);
	void startVibrate();
	void stopVibrate();
	void command(QString cmd, QString name, int context, ChannelBase *instance);
	void nickInUse();
	PrivateWidget *newPrivateChannel(QString name, QString msg);
	void closeActiveChannel();
	void tab();
	void timeout(int secs);
	
protected:
	ChannelBase *activeChannel();

private:
	LogWidget *_log;
	QxtIrcConnection *_client;
	QList<ChannelBase *> _channels;
	QAction *_shownicklistaction;
	QMenu *_channelmenu;
};

#endif

