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
//Ok, you can copy that floppy


#include "mainwindow.h"
#include "channelbase.h"
#include "logwidget.h"
#include "channelwidget.h"
#include <QxtIrcConnection>
#include <QDebug>
#include <QKeyEvent>
#include <QAction>
#include <QTimer>
#include <QxtIrcBotMessage>
#include "passworddialog.h"
#include "nickdialog.h"
#include "privatewidget.h"
#ifdef Q_OS_WINCE
#include <objbase.h>
#include <initguid.h>
#include <winsock.h>
#include <connmgr_proxy.h>
#include <connmgr.h>
#include <wininet.h>
#include < E:\Program Files\Microsoft Visual Studio 8\SmartDevices\SDK\PocketPC2003\Include\nled.h >
extern "C"
{
    BOOL NLedGetDeviceInfo(INT nID, PVOID pOutput);
    BOOL NLedSetDevice(INT nID, PVOID pOutput);
}
#endif

class HaxTab : QTabWidget
{
	public:
		QTabBar * tabBar() {return(QTabWidget::tabBar());}  //HAX
};

MainWindow::MainWindow(QWidget* parent, Qt::WFlags fl)
		: QMainWindow(parent, fl), Ui::MainWindow()
{
	setupUi(this);
	ChannelTabs->clear();

#ifdef Q_OS_WINCE
        CONNMGR_CONNECTIONINFO ci = {0};
    PROXY_CONFIG pcProxy = {0};
    DWORD dwStatus = 0;
    DWORD dwIndex = 0;
    HRESULT hr = S_OK;
    HANDLE hConnection = NULL;
    HANDLE hOpen = NULL;
    LPTSTR pszProxy = NULL;
    DWORD dwAccessType;
    GUID guidTest;

    // Register with the connection manager
    ci.cbSize = sizeof(CONNMGR_CONNECTIONINFO);
    ci.dwParams = CONNMGR_PARAM_GUIDDESTNET;
    ci.dwFlags = CONNMGR_FLAG_PROXY_HTTP;
    ci.dwPriority = CONNMGR_PRIORITY_USERINTERACTIVE;

    const GUID IID_ConnPrv_IProxyExtension = {0xaf96b0bd,0xa481,0x482c,{0xa0,0x94,0x44,0x87,0x67,0xa0,0xc0}};

    // Map the URL to a network, if we can
    hr = ConnMgrMapURL(_T("http://www.google.com"), &(ci.guidDestNet), &dwIndex);
    //Check hr value.

    hr = ConnMgrEstablishConnectionSync(&ci, &hConnection, 25000, &dwStatus);

	menuBar()->setDefaultAction(defaultaction);
#endif
	
	//Menu Stuff
	QAction *defaultaction = new QAction("Tab", this);
	QObject::connect(defaultaction, SIGNAL(triggered()), this, SLOT(tab()));
	
	_shownicklistaction = new QAction("Show User List", this);
	_shownicklistaction->setCheckable(true);
	_shownicklistaction->setChecked(false);
	menubar->addAction(_shownicklistaction);

	menubar->addSeparator();
	_channelmenu = menubar->addMenu("Channels");
	
	QAction *tmp = new QAction("Close Channel",this);
	QObject::connect(tmp, SIGNAL(triggered()), this, SLOT(closeActiveChannel()));
	menubar->addAction(tmp);
	
	menubar->addSeparator();
	tmp = new QAction("Connect",this);
	QObject::connect(tmp, SIGNAL(triggered()), this, SLOT(connect()));
	menubar->addAction(tmp);
	tmp = new QAction("Disconnect",this);
	QObject::connect(tmp, SIGNAL(triggered()), this, SLOT(disconnect()));
	menubar->addAction(tmp);
	
	_client = new QxtIrcConnection(this);
	QObject::connect(_client, SIGNAL(stateChanged(QxtIrcConnection::State)), this, SLOT(stateChanged(QxtIrcConnection::State)));
	QObject::connect(_client, SIGNAL(messageReceived(QxtIrcMessage)), this, SLOT(messageReceived(QxtIrcMessage)));
	QObject::connect(_client, SIGNAL(joinedChannel(QString)), this, SLOT(joinedChannel(QString)));
	QObject::connect(_client, SIGNAL(partedChannel(QString, QString)), this, SLOT(partedChannel(QString)));
	QObject::connect(_client, SIGNAL(kicked(QString, QString, QxtIrcNick)), this, SLOT(kicked(QString)));
	QObject::connect(_client, SIGNAL(nickChanged(QxtIrcNick)), this, SLOT(nickChanged(QxtIrcNick)));
	QObject::connect(_client, SIGNAL(nickInUse()), this, SLOT(nickInUse()));
	QObject::connect(_client, SIGNAL(timeout(int)),this, SLOT(timeout(int)));
	QObject::connect(ChannelTabs, SIGNAL(currentChanged(int)), this, SLOT(channelChanged(int)));

	_log = new LogWidget(_client, this);
	_channels << _log;
	ChannelTabs->addTab(_log, "Server");
	QAction *action = new QAction("Server", _log);
	QObject::connect(_log,SIGNAL(command(QString,QString,int,ChannelBase*)),this,SLOT(command(QString,QString,int,ChannelBase*)));
	QObject::connect(action, SIGNAL(triggered()), _log, SLOT(activate()));
	QObject::connect(_log, SIGNAL(activated(QWidget*)), this, SLOT(channelActivated(QWidget*)));
	_channelmenu->addAction(action);
	_channelmenu->addSeparator();
	
	_client->setTimeout(500);
	
	connect();
}

MainWindow::~MainWindow()
{
}

void MainWindow::connect()
{
	_log->clear();
	_log->append("%GREENWelcome to MobileIRC 0.1%R");
	_client->setNickName("MobileIRC");
	_client->setRealName("Mobile IRC 0.1");
	_client->connectToHost("rkon.homeip.net", 6667, false);
}

void MainWindow::disconnect()
{
	_client->disconnectFromHost();
}

void MainWindow::nickInUse()
{
	NickDialog dlg(this);
	dlg.setNick(_client->nick());
	if(dlg.exec() == QDialog::Accepted)
	{
		_client->setNickName(dlg.nick());
	}
}

void MainWindow::stateChanged(QxtIrcConnection::State state)
{
	switch (state)
	{
		case QxtIrcConnection::Connecting:
			_log->append("%BROWNConnecting...%R");
			break;
		case QxtIrcConnection::Connected:
			_log->append("%GREENConnected%R");
			
			foreach(QString var, _client->listServerConfig())
			{
				qDebug() << "CONFIG VAR:" << _client->getServerConfig(var,QVariant());
			}
			
			break;
		case QxtIrcConnection::Registering:
			_log->append("%BROWNRegistering...%R");
			break;
		case QxtIrcConnection::Disconnected:
			qDebug() << "We disconnected?";
			appendAll("%REDDisconnected%R");
			foreach(QString chan, _client->listChannels())
			{
				if(chan.startsWith("#"))
					partedChannel(chan);
			}
			break;
	}
}

void MainWindow::messageReceived(QxtIrcMessage msg)
{
	if (msg.cmd() == "005") return;
	if (msg.cmd() == "004") return;
	if (msg.cmd() == "396") return;
	if (msg.cmd() == "332") return;
	if (msg.cmd() == "333") return;
	if (msg.cmd() == "353") return;
	if (msg.cmd() == "366") return;
	if (msg.cmd() == "421")
	{
		activeChannel()->append("%RED%B Unkown Command %R");
		return;
	}
	if (msg.cmd() == "381")
	{
		activeChannel()->append("%GREEN%B You are now an IRC operator on this server.%R");
		return;
	}
	if (msg.cmd() == "491")
	{
		activeChannel()->append("%RED%B Invalid oper credentials.%R");
		return;
	}
	bool ok;
	msg.cmd().toInt(&ok);
	if (ok)
	{
		QStringList args = msg.args();
		if (args.count() > 0 && args[0] == _client->nick()) args.removeAt(0);
		activeChannel()->append("%BROWN[" + msg.cmd() + "] " + args.join(" ") + "%R");
		return;
	}
	if (msg.cmd() == "PING") 
	{
		qDebug() << "Got PING!";
	}
	if (msg.cmd() == "MODE") return;
	if (msg.cmd() == "PRIVMSG")
	{
		if(msg.to().nick().startsWith("#")) return;
		PrivateWidget *chan = NULL;
		foreach(ChannelBase *c, _channels)
		{
			if(c->type() == Private && c->name() == msg.from().nick())
				chan=(PrivateWidget *)c;
		}
		
		if(chan == NULL)
		{
			chan = newPrivateChannel(msg.from().nick(),"");
		}
		
		if(msg.args()[0].startsWith(QString() + (char)1 + "ACTION"))
		{
			QString txt = msg.args()[0].mid(7,msg.args()[0].size()-8);
			chan->append("%BLUE *%R " + msg.from().nick() + " %BLUE" + txt);
		}
		else
		{
			chan->append("%BLUE" + msg.from().nick() + "%R: " + msg.args()[0]);
		}
	}
	if (msg.cmd() == "NICK") return;
	if (msg.cmd() == "NOTICE")
	{
		if (msg.args().count() == 0) return;
		if (msg.to().nick().startsWith("#")) return;
		if (msg.to().nick() == _client->nick())
		{
			activeChannel()->append("%BROWN[NOTICE] " + msg.from().nick() + "%R: " + msg.args()[0]);
		}
		else
		{
			_log->append("%BROWN[NOTICE] " + msg.from().nick() + "%R: " + msg.args()[0]);
		}
		return;
	}
	if (msg.cmd() == "JOIN")
	{
		_log->append("%BROWN[JOIN] Joined Channel " + msg.args()[0] + "%R");
		return;
	}
	_log->append(msg.toString());
}


void MainWindow::joinedChannel(QString name)
{
	ChannelWidget *channel = new ChannelWidget(name, _client, this);
	_channels << channel;
	ChannelTabs->addTab(channel, name);
	ChannelTabs->setCurrentWidget(channel);

	channel->showNickList(_shownicklistaction->isChecked());
	QObject::connect(_shownicklistaction, SIGNAL(toggled(bool)), channel, SLOT(showNickList(bool)));

	QAction *action = new QAction(name, channel);
	QObject::connect(action, SIGNAL(triggered()), channel, SLOT(activate()));
	QObject::connect(channel, SIGNAL(activated(QWidget*)), this, SLOT(channelActivated(QWidget*)));
	QObject::connect(channel, SIGNAL(nickMentioned(ChannelWidget *)), this, SLOT(nickMentioned(ChannelWidget *)));
	QObject::connect(channel, SIGNAL(newMessage(ChannelWidget *)), this, SLOT(newMessage(ChannelWidget *)));
	QObject::connect(channel, SIGNAL(command(QString,QString,int,ChannelBase*)),this,SLOT(command(QString,QString,int,ChannelBase*)));
	QObject::connect(channel, SIGNAL(newPrivateChannel(QString,QString)),this, SLOT(newPrivateChannel(QString,QString)));
	_channelmenu->addAction(action);
}

void MainWindow::partedChannel(QString name)
{
	for (int i = 0; i < ChannelTabs->count(); i++)
	{
		if (ChannelTabs->tabText(i) == name)
		{
			QWidget *w = ChannelTabs->widget(i);
			ChannelTabs->removeTab(i);
			_channels.removeAt(i);
			w->deleteLater();
		}
	}
}

void MainWindow::kicked(QString name)
{
	partedChannel(name);
}

void MainWindow::appendAll(QString msg)
{
	foreach(ChannelBase *chan, _channels)
	{
		chan->append(msg);
	}
}

void MainWindow::nickChanged(QxtIrcNick nick)
{
	appendAll("%BROWNYou are now known as " + nick.nick() + ".");
}

ChannelBase *MainWindow::activeChannel()
{
	//QString name = ChannelTabs->tabText(ChannelTabs->currentIndex());
	//if (_channels.contains(name)) return(_channels[name]);
	return(_channels[ChannelTabs->currentIndex()]);
}

void MainWindow::send()
{
	activeChannel()->send();
}

void MainWindow::channelChanged(int index)
{
	activeChannel()->focus();
	((HaxTab *)ChannelTabs)->tabBar()->setTabTextColor(ChannelTabs->currentIndex(),QColor(0,0,0));
}

void MainWindow::channelActivated(QWidget *w)
{
	int index = ChannelTabs->indexOf(w);
	ChannelTabs->setCurrentIndex(index);
}
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{	
	if (event->type() != QEvent::KeyPress) return(false);
	QKeyEvent *keyevent = (QKeyEvent *)event;

	int index = ChannelTabs->currentIndex();
	
	if (((keyevent->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier) && (keyevent->key() == Qt::Key_Left))
	{
		qDebug() << "Left";
		if (index == 0) index = ChannelTabs->count();
		index--;
		ChannelTabs->setCurrentIndex(index);
		return(true);
	}
	else if (((keyevent->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier) && (keyevent->key() == Qt::Key_Right))
	{
		qDebug() << "Right";
		if (index == (ChannelTabs->count() - 1)) index = -1;
		index++;
		ChannelTabs->setCurrentIndex(index);
		return(true);
	}
	else if (((keyevent->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier) && (keyevent->key() == Qt::Key_Up))
	{
		qDebug() << "Up";
		activeChannel()->scroll(true);
		return(true);
	}
	else if (((keyevent->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier) && (keyevent->key() == Qt::Key_Down))
	{
		qDebug() << "Down";
		activeChannel()->scroll(false);
		return(true);
	}
	else if (((keyevent->modifiers() & Qt::ShiftModifier) == Qt::ShiftModifier) && ((keyevent->key() == Qt::Key_Return) || (keyevent->key() == Qt::Key_Enter)))
	{
		qDebug() << "Psudo-Tab";
		QKeyEvent *tmp = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab,0);
		QApplication::postEvent(QApplication::focusWidget(),tmp);
		return(true);
	}
	
	return(false);
}
bool MainWindow::event(QEvent *event)
{
	if (event->type() != QEvent::KeyPress) return(QMainWindow::event(event));
	QKeyEvent *keyevent = (QKeyEvent *)event;

	qDebug() << "KEY PRESSED!";

	return(QMainWindow::event(event));

}

void MainWindow::nickMentioned(ChannelWidget *channel)
{
        startVibrate();
	int index  = ChannelTabs->indexOf(channel);
	if(index < 0) return;
	if(index == ChannelTabs->currentIndex()) return;
	
	((HaxTab *)ChannelTabs)->tabBar()->setTabTextColor(index,QColor(150,0,0));
}

void MainWindow::newMessage(ChannelWidget *channel)
{
	int index  = ChannelTabs->indexOf(channel);
	if(index < 0) return;
	if(index == ChannelTabs->currentIndex()) return;
	
	((HaxTab *)ChannelTabs)->tabBar()->setTabTextColor(index,QColor(0,150,0));
}

void MainWindow::startVibrate()
{
#ifdef Q_OS_WINCE
        NLED_SETTINGS_INFO nsi;

        nsi.LedNum = 1;
        nsi.OffOnBlink = 2;
        NLedSetDevice(NLED_SETTINGS_INFO_ID, &nsi);
        QTimer::singleShot(1000,this,SLOT(stopVibrate()));
#endif
}

void MainWindow::stopVibrate()
{
#ifdef Q_OS_WINCE
        NLED_SETTINGS_INFO nsi;

        nsi.LedNum = 1;
        nsi.OffOnBlink = 0;
        NLedSetDevice(NLED_SETTINGS_INFO_ID, &nsi);
#endif
}

void MainWindow::command(QString cmd, QString name, int context, ChannelBase *instance)
{
	QxtIrcBotMessage msg(cmd,"",QxtIrcMessage(),0);
	
	if(msg.matchAll("/part (channel)") || msg.matchAll("/leave (channel)"))
	{
		_client->writeMessage("PART",msg.args()[1]);
		return;
	}
	else if(msg.matchAll("/part (channel) (...)") || msg.matchAll("/leave (channel) (...)"))
	{
		QString channel = msg.args()[1];
		QStringList args = msg.args();
		args.removeAt(0);
		args.removeAt(0);
		_client->writeMessage("PART",QStringList() << channel << args.join(" "));
		return;
	}
	else if((msg.matchAll("/part (...)") || msg.matchAll("/leave (...)")) && context == Channel)
	{
		QStringList args = msg.args();
		args.removeAt(0);
		_client->writeMessage("PART",QStringList() << name << args.join(" "));
		return;
	}
	else if(msg.matchAll("/close") && (context == Private || context == Channel))
	{
		closeActiveChannel();
		return;
	}
	else if(msg.matchAll("/quit (...)") || msg.matchAll("/exit (...)"))
	{
		QStringList args = msg.args();
		args.removeAt(0);
		_client->writeMessage("QUIT",QStringList() << args.join(" "));
		return;
	}
	else if(msg.matchAll("/pass") || msg.matchAll("/password"))
	{
		PasswordDialog dlg;
		dlg.setUser(_client->nick());
		dlg.setUserEnabled(false);
		
		if(dlg.exec() == QDialog::Accepted)
		{
			_client->writeMessage("PASS",QStringList() << dlg.password());
		}
		return;
	}
	else if(msg.matchAll("/password (string)"))
	{
		_client->writeMessage("PASS",QStringList() << msg.args()[1]);
		return;
	}
	else if(msg.matchAll("/op") || msg.matchAll("/oper"))
	{
		PasswordDialog dlg;
		dlg.setUser(_client->nick());
		dlg.focusPassword();
		
		if(dlg.exec() == QDialog::Accepted)
		{
			_client->writeMessage("OPER",QStringList() << dlg.user() << dlg.password());
		}
		return;
	}
	else if(msg.matchAll("/op (string)") || msg.matchAll("/oper (string)"))
	{
		PasswordDialog dlg;
		dlg.setUser(msg.args()[1]);
		dlg.focusPassword();
		
		if(dlg.exec() == QDialog::Accepted)
		{
			_client->writeMessage("OPER",QStringList() << dlg.user() << dlg.password());
		}
		return;
	}
	else if(msg.matchAll("/op (string) (string)"))
	{
		_client->writeMessage("OPER",QStringList() << msg.args()[1] << msg.args()[2]);
		return;
	}
	else if(msg.matchAll("/msg (string) (...)"))
	{
		newPrivateChannel(msg.args()[1],"");
		return;
	}
	else if(msg.matchAll("/msg (string) (...)"))
	{
		QString dest = msg.args()[1];
		QStringList args = msg.args();
		args.removeAt(0);
		args.removeAt(0);
		_client->writeMessage("PRIVMSG",QStringList() << dest << args.join(" "));
		return;
	}
	else if(msg.matchAll("/me (...)") && (context == Channel || context == Private))
	{
		QStringList args = msg.args();
		args.removeAt(0);
		_client->sendAction(name,args.join(" "));
		appendAll(" * " + _client->nick() + " %BLUE" + args.join(" ")); 
		return;
	}
	else if(msg.matchAll("/id") || msg.matchAll("/ns id"))
	{
		PasswordDialog dlg;
		dlg.setUser(_client->nick());
		dlg.focusPassword();
		
		if(dlg.exec() == QDialog::Accepted)
		{
			_client->sendMessage("NickServ","id " + dlg.user() + " " + dlg.password());
		}
		return;
	}
	else if(msg.matchAll("/serverconfig"))
	{
		
		foreach(QString var, _client->listServerConfig())
		{
			qDebug() << "CONFIG VAR:" << var << "=" << _client->getServerConfig(var,QVariant());
		}
		return;
	}
	
	//If we get here, just process the command and send it to the server
	int index = cmd.indexOf(" ");
	QString command = cmd.left(index);
	QString args = cmd.mid(index + 1);

	if (command.startsWith("/")) command = command.mid(1);
	command = command.toUpper();

	qDebug() << "Writing:" << command + " " + args;

	_client->writeRawMessage(command + " " + args);
}

PrivateWidget *MainWindow::newPrivateChannel(QString name, QString msg)
{
	PrivateWidget *chan = new PrivateWidget(_client,name,this);
	_channels << chan;
	ChannelTabs->addTab(chan, name);
	ChannelTabs->setCurrentWidget(chan);

	QAction *action = new QAction(name, chan);
	_channelmenu->addAction(action);
	QObject::connect(action, SIGNAL(triggered()), chan, SLOT(activate()));
	QObject::connect(chan, SIGNAL(activated(QWidget*)), this, SLOT(channelActivated(QWidget*)));
	QObject::connect(chan, SIGNAL(nickMentioned(ChannelWidget *)), this, SLOT(nickMentioned(ChannelWidget *)));
	QObject::connect(chan, SIGNAL(newMessage(ChannelWidget *)), this, SLOT(newMessage(ChannelWidget *)));
	QObject::connect(chan, SIGNAL(command(QString,QString,int,ChannelBase*)),this,SLOT(command(QString,QString,int,ChannelBase*)));
	
	if(msg != "")
		chan->sendMessage(name,msg);
	
	return(chan);
}

void MainWindow::closeActiveChannel()
{
	ChannelBase *chan = activeChannel();
	QWidget *widget = (QWidget *)activeChannel();
	int type = chan->type();
	QString name = chan->name();
	
	if(type == Server) return;
	
	if(type == Channel)
	{
		_client->writeMessage("PART",name);
	}
	else
	{
		_channels.removeAll(chan);
		ChannelTabs->currentWidget()->deleteLater();
	}
	
}

void MainWindow::tab()
{
	QKeyEvent *tmp = new QKeyEvent(QEvent::KeyPress, Qt::Key_Tab,0);
	QApplication::postEvent(QApplication::focusWidget(),tmp);
}

void MainWindow::timeout(int secs)
{
	appendAll(QString("%REDConnection timed out: %1 secconds.").arg(secs));
}