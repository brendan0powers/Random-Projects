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

#ifndef CHANNELWIDGET_H
#define CHANNELWIDGET_H

#include <QWidget>
#include "ui_ChannelWidget.h"
#include "channelbase.h"
#include <QxtIrcConnection>
#include <QxtIrcChannel>
#include "channelmodel.h"
#include "mainwindow.h"

class ChannelWidget : public QWidget, private Ui::Form, public ChannelBase
{
	Q_OBJECT

public:
	ChannelWidget(QString name, QxtIrcConnection *c, QWidget* parent = 0, Qt::WFlags fl = 0);
	~ChannelWidget();

signals:
        void activated(QWidget *);
	void nickMentioned(ChannelWidget *thiswidget);
	void newMessage(ChannelWidget *thiswidget);
	void command(QString cmd, QString name, int context, ChannelBase *instance);
	void newPrivateChannel(QString name, QString msg);
	
public slots:
	virtual void append(QString html);
	virtual void clear();
	virtual void send();
	virtual MainWindow::Scope type() {return MainWindow::Channel;}
        virtual void focus();
        virtual void showNickList(bool show);
	virtual void scroll(bool up);
        void activate();
	virtual QString name();
	

protected slots:
	void on_SendButton_clicked();
	void messageReceived(QxtIrcMessage msg);
	void userJoined(QxtIrcNick user);
	void userParted(QxtIrcNick user, QString reason);
	void topicChanged(QString channel, QString topic);
	void nickChanged(QxtIrcNick oldnick, QxtIrcNick newnick);
	void userKicked(QxtIrcNick user, QString reason, QxtIrcNick from);
	void userKilled(QxtIrcNick user, QString reason, QxtIrcNick from);
	void userQuit(QxtIrcNick user, QString reason);
	void nickActivated(QModelIndex index);
	
private:
	QxtIrcConnection *_client;
	QxtIrcChannel *_channel;
	QString _name;
	ChannelModel *_model;
};

#endif

