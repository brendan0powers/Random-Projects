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

#ifndef PRIVATEWIDGET_H
#define PRIVATEWIDGET_H

#include <QWidget>
#include "ui_LogWidget.h"
#include "channelbase.h"
#include "mainwindow.h"

class QxtIrcConnection;

class PrivateWidget : public QWidget, private Ui::LogUi, public ChannelBase
{
	Q_OBJECT

public:
	PrivateWidget(QxtIrcConnection *c, QString name, QWidget* parent = 0, Qt::WFlags fl = 0);
	~PrivateWidget();

signals:
	void activated(QWidget *);
	void command(QString cmd, QString name, int context, ChannelBase *instance);

public slots:
	virtual void append(QString html);
	virtual void clear();
	virtual void send();
	virtual MainWindow::Scope type()
	{
		return MainWindow::Private;
	}
	virtual void focus();
	void activate();
	virtual void scroll(bool up);
	virtual QString name();
	void sendMessage(QString to, QString msg);
	
protected slots:
	void on_SendButton_clicked();

private:
	QxtIrcConnection *_client;
	QString _name;
};

#endif

