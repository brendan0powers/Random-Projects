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
#include "qxtircconnection.h"
#include "qxtircconnection_p.h"

#include <QMutexLocker>
#include <QTcpSocket>
#include <QStringList>
#include <QHostInfo>
#include <QTime>
#include <QMetaObject>
#include <QRegExp>
#include <QTextStream>
#include <QMetaType>
#include <QVariant>
#include <qxtircmessage.h>
#include <qxtircbotmessage.h>

QxtIrcConnection::QxtIrcConnection(QObject *parent)
		: QObject(parent)
{
	QXT_INIT_PRIVATE(QxtIrcConnection);
	
	qxt_d().nickname = "qxtirc_client";
	qxt_d().realname = "QxtIrc Client";
	
	qRegisterMetaType<QxtIrcMessage>("QxtIrcMessage");
	qRegisterMetaType<QxtIrcNick>("QxtIrcNick");
	qRegisterMetaType<QxtIrcConnectionPrivate::Filter>("QxtIrcNick::Filter");
	qRegisterMetaType<QxtIrcBotMessage>("QxtIrcBotMessage");

// 	fill in default access prefixes, this will be overwritten if the server says so (even if it agrees :P)
// 	PREFIX=(qaohv)~&@%+
	qxt_d().accessPrefix['~'] = Founder;
	qxt_d().accessPrefix['&'] = Administrator;
	qxt_d().accessPrefix['@'] = Operator;
	qxt_d().accessPrefix['%'] = HalfOp;
	qxt_d().accessPrefix['+'] = Voice;

        qxt_d().socket = new QTcpSocket(this);
	connect(qxt_d().socket, SIGNAL(connected()), this, SLOT(connected()));
	connect(qxt_d().socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
	connect(qxt_d().socket, SIGNAL(readyRead()), this, SLOT(messageReceived()), Qt::QueuedConnection);
	connect(this, SIGNAL(queuedParseMessage(QString)), this, SLOT(parseMessageAndReadMore(QString)), Qt::QueuedConnection);
	connect(&qxt_d().timeouttimer, SIGNAL(timeout()),this, SLOT(timedout()));
	
	setTimeout(130);
}

QxtIrcConnection::~QxtIrcConnection()
{
}

void QxtIrcConnection::setNickName(QString nick)
{
	qxt_d().nickname = nick;
	if (getState() > Connecting)
	{
		//If we're already connected then we send the nick command
		writeMessage("NICK", nick);
	}

	if (qxt_d().realname == "QxtIrc Client")
		qxt_d().realname = nick;
}

void QxtIrcConnection::setRealName(QString real)
{
	qxt_d().realname = real;
}

bool QxtIrcConnection::connectToHost(QString hostname, int port, bool ssl)
{
	qxt_d().socket->connectToHost(hostname, port);
	qxt_d().setState(Connecting);
	return ssl;
}

void QxtIrcConnection::disconnectFromHost()
{
	qxt_d().socket->disconnectFromHost();
}

void QxtIrcConnection::quit(QString message)
{
	if (getState() > Connecting)
	{
		//If we're already connected then we send the nick command
		writeMessage("QUIT", message);
	}
	qxt_d().socket->flush();
	qxt_d().socket->close();
}

QxtIrcConnection::State QxtIrcConnection::getState()
{
// 	QMutexLocker locker(&mutex);
	return qxt_d().state;
}

void QxtIrcConnectionPrivate::setState(QxtIrcConnection::State s)
{
	state = s;
	emit qxt_p().stateChanged(s);
}

void QxtIrcConnection::connected()
{
	qDebug() << "We have connected to the IRC server, registering now";
	writeMessage("NICK", qxt_d().nickname);
	writeMessage("USER", QStringList() << qxt_d().nickname << QHostInfo::localHostName() << "*" << qxt_d().realname);

	qxt_d().setState(Registering);
}

void QxtIrcConnection::registered()
{
	if (qxt_d().password != "")
	{
		sendMessage("NickServ", "id " + qxt_d().password);
	}
}

void QxtIrcConnection::parseMessageAndReadMore(QString line)
{
	parseMessage(line);
	while (qxt_d().socket->canReadLine())
	{
		line = qxt_d().socket->readLine().trimmed();
		parseMessage(line);
	}
}

void QxtIrcConnection::messageReceived()
{
	while (qxt_d().socket->canReadLine())
	{
		QString line = qxt_d().socket->readLine().trimmed();
		parseMessage(line);
	}
}

///NOTE I'm not sure what to do with this, since it will not always return the message you expect
bool QxtIrcConnection::waitForMessage(QxtIrcMessage &msg, int t)
{
	QTime timer(0, 0);
	QTime end(0, 0);
	end = end.addMSecs(t);
	qDebug() << "We've starting waiting for messages" << timer.msecsTo(end);
	while (timer.msecsTo(end) > 0)
	{
		if (qxt_d().socket->canReadLine())
		{
			QString line = qxt_d().socket->readLine().trimmed();
			qDebug() << "Caught some data while waiting for messages." << line;
			emit queuedParseMessage(line);
			msg = QxtIrcMessage(line);
			return true;
		}
		if (qxt_d().socket->waitForReadyRead(timer.msecsTo(end)))
		{
			if (qxt_d().socket->canReadLine())
			{
				QString line = qxt_d().socket->readLine().trimmed();
				qDebug() << "Caught some data while waiting for messages." << line;
				emit queuedParseMessage(line);
				msg = QxtIrcMessage(line);
				return true;
			}
		}
		else
		{
			qDebug() << "NO DATA!";
			return false;
		}
	}
	return false;
}

/*
:rkon.homeip.net 311 fagbot chrisinajar chris rkon/core/chrisinajar * :Chris
:rkon.homeip.net 319 fagbot chrisinajar :~#guests #services ~#rkon ~#core
:rkon.homeip.net 312 fagbot chrisinajar rkon.homeip.net :rkon IRC Server
:rkon.homeip.net 330 fagbot chrisinajar chrisinajar :is logged in as
:rkon.homeip.net 317 fagbot chrisinajar 8 1235253870 :seconds idle, signon time
:rkon.homeip.net 318 fagbot chrisinajar :End of /WHOIS list.
*/

QxtIrcConnection::WhoisData QxtIrcConnection::getWhois(QxtIrcNick nick, int t)
{
	WhoisData ret;
	ret.isLoggedIn = false;
	ret.isOper = false;
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

	for (int i = 0; i < attempts; i++)
	{
		QTime timer(0, 0);
		QTime end(0, 0);
		end = end.addMSecs(timeout);
		writeMessage("WHOIS", QStringList() << nick.nick());
		QxtIrcMessage msg;
		while (waitForMessage(msg, timer.msecsTo(end)))
		{
			bool ok;
			int code = msg.cmd().toInt(&ok, 10);
			if (ok)
			{
				switch (code)
				{
					case RPL_WHOISUSER: //chris rkon/core/chrisinajar * :Chris Vickery
					{
						QStringList args = msg.args();
						args.takeFirst();
						args.takeFirst();
						if (args.count() > 0)
							nick.setIdent(args[0]);
						if (args.count() > 1)
							nick.setHost(args[1]);
						if (args.count() > 3)
							ret.realName = args[3];
						ret.nick = nick; //chrisinajar!chris@rkon/core/chrisinajar
						break;
					}
					case RPL_WHOISCHANNELS: //:~#guests #services ~#rkon ~#core
					{
						QStringList args = msg.args();
						args.takeFirst();
						args.takeFirst();
						if (args.count() > 0)
						{
							foreach(QString channel, args[0].split(' ', QString::SkipEmptyParts))
							{
								if (!channel.contains('#'))
									continue;
								if (channel.startsWith('#'))
									ret.channelList[channel] = Member;
								else
									ret.channelList[channel.mid(1)] = qxt_d().accessPrefix[channel[0]];
							}
						}
						break;
					}
					case RPL_WHOISSERVER: //rkon.homeip.net :rkon IRC Server
					{
						QStringList args = msg.args();
						args.takeFirst();
						args.takeFirst();
						if (args.count() > 0)
							ret.serverHost = args[0];
						if (args.count() > 1)
							ret.serverName = args[1];
						break;
					}
					case RPL_WHOISOPERATOR: //:is a SuperAdmin on rkon
					{
						QStringList args = msg.args();
						args.takeFirst();
						args.takeFirst();
						if (args.count() > 0)
						{
							if (args[0].startsWith("is a "))
							{
								QStringList op = args[0].split(' ', QString::SkipEmptyParts);
								if (op.count() > 2)
								{
									ret.isOper = true;
									ret.operStatus = op[2];
								}
							}
							else
							{
								qWarning() << "FIXME: Unknown RPLqxt_d().WHOISOPERATOR format, " << args[0];
							}
						}
						break;
					}
					case RPL_WHOISIDENTIFIED: //chrisinajar :is logged in as
					{
						QStringList args = msg.args();
						args.takeFirst();
						args.takeFirst();
						if (args.count() > 0)
						{
							ret.userName = args[0];
							ret.isLoggedIn = true;
						}
						break;
					}
					case RPL_WHOISIDLE: //1341 1235259566 :seconds idle, signon time
					{
						QStringList args = msg.args();
						args.takeFirst();
						args.takeFirst();
						if (args.count() > 1)
						{
							QStringList descriptors = args[args.count() - 1].split(", ", QString::SkipEmptyParts);
							if ((args.count() - 1) != descriptors.count())
							{
								qWarning() << "FIXME: Unknown RPLqxt_d().WHOISIDLE format, " << args;
								break;
							}
							for (int i = 0; i < descriptors.count(); i++)
							{
								if (descriptors[i] == "seconds idle")
								{
									QTime idle(0, 0, 0);
									idle.addSecs(args[i].toInt());
									ret.idle = idle;
								}
								else if (descriptors[i] == "signon time")
								{
									ret.signOnTime = QDateTime::fromTime_t(args[i].toUInt()); //not 2106 safe
								}
								else
								{
									qWarning() << "FIXME: Unkown RPLqxt_d().WHOISIDLE descriptor, " << descriptors[i];
								}
							}
						}
						break;
					}
					case RPL_ENDOFWHOIS: //:End of /WHOIS list.
						return ret;
						break;
				}
			}
		}
	}
	return ret;
}

QxtIrcConnection::WhoisData QxtIrcConnection::getWhois(QString nick, int t)
{
	return getWhois(QxtIrcNick(nick), t);
}

QxtIrcConnection::AccountStatus QxtIrcConnection::getAccountStatus(QxtIrcNick nick, int t)
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

	for (int i = 0; i < attempts; i++)
	{
		QTime timer(0, 0);
		QTime end(0, 0);
		end = end.addMSecs(timeout);
		sendMessage("NickServ", "ACC " + nick.nick());
		QxtIrcMessage msg;
		while (waitForMessage(msg, timer.msecsTo(end)))
		{
			QStringList line = msg.args()[0].split(" ", QString::SkipEmptyParts);

			qDebug() << line;
			if (line.count() >= 3)
			{
				if (line[1] == "ACC")
				{
					return static_cast<QxtIrcConnection::AccountStatus>(line[2].toInt());
					break;
				}
				else
				{
					qDebug() << "got a message, but not the message we were looking for...";
				}
			}
		}
	}
	return AccountStatusError;
}

QxtIrcConnection::AccountStatus QxtIrcConnection::getAccountStatus(QString nick, int t)
{
	return getAccountStatus(QxtIrcNick(nick), t);
}

void QxtIrcConnection::parseMessage(QString line)
{
	QxtIrcMessage msg(line);
	qDebug() << "LineStart:" << line;
	switch (qxt_d().state)
	{
		case Registering:
		{
			bool ok;
			int num = msg.cmd().toInt(&ok, 10);
			if (ok)
			{
				switch(num)
				{
					case 433:
					case 436:
						emit(nickInUse());
						break;
					default:
						registered();
						qxt_d().setState(Connected);
						break;
				}
				

			}
			break;
		}
		case Connected:
		{
			bool ok;
			int code = msg.cmd().toInt(&ok, 10);
			if (ok)
			{
				switch (code)
				{
					case REG_SERVERCONFIG:
					{
						foreach(QString cmd, msg.args())
						{
							if (cmd.contains('='))
							{
								QString key = cmd.section('=', 0, 0);
								QString value = cmd.section('=', 1, 1);
								qxt_d().serverConfig[key] = value;
								if (key == "PREFIX")
								{
									value.remove('(');
									QStringList values = value.split(')', QString::SkipEmptyParts);
									if (values.count() != 2)
									{
										qCritical() << "Failed to parse PREFIX value, too many )'s";
										continue;
									}
									if (values[0].length() != values[1].length())
									{
										qCritical() << "Failed to parse PREFIX value, the two sections have different lengths";
										continue;
									}
									qxt_d().accessPrefix.clear();
									QHash<QChar, ChannelAccess> channelValues;
									channelValues['q'] = Founder;
									channelValues['a'] = Administrator;
									channelValues['o'] = Operator;
									channelValues['h'] = HalfOp;
									channelValues['v'] = Voice;
									for (int i = 0; i < values[0].length(); i++)
									{
										qxt_d().accessPrefix[values[1][i]] = channelValues[values[0][i]];
									}
								}
							}
							else
							{
								qxt_d().serverConfig[cmd] = true;
							}
						}
						break;
					}
					case RPL_NAMREPLY:
					{
						// this is the message containing the current user list.
						// chan = arg[2], nicks are arg[3]
						qDebug() << "RPL_NAMES:" << line << msg.args();
						QStringList users = msg.args()[3].split(" ", QString::SkipEmptyParts);
						for (int i = 0; i < users.count(); i++)
						{
							QString user = users.at(i);
							qDebug() << "BEFORE USER:" << user;
							user.remove(QRegExp("~"));
							user.remove(QRegExp("@"));
							qDebug() << "JOINING CHANNEL:" << msg.args()[2] << user;
							qxt_d().channels[msg.args()[2]]->joinUser(user);
						}
						break;
					}
					case RPL_ENDOFNAMES:
					{
						qDebug() << "=====JOINED CHANNEL:" << msg.args()[1];
						emit(joinedChannel(msg.args()[1]));
						break;
					}
					case RPL_TOPIC:
					{
						//This is the message containing a channel topic
						// chan = arg[1] topic = arg[2]
						qDebug() << "a topic has changed" << msg.args()[1] << msg.args()[2] << line;
						if (qxt_d().channels.contains(msg.args()[1])) qxt_d().channels[msg.args()[1]]->setTopic(msg.args()[2]);
						break;
					}
					default:
						// This responce code has not yet been implemented
						break;
				}
			}
			else if (msg.cmd() == "PING")
			{
				writeMessage("PONG", msg.args()[0]);
				if(qxt_d().timeout > 0)
				{
					qxt_d().timeouttimer.stop();
					qxt_d().timeouttimer.start(qxt_d().timeout);
				}
			}
			else if (msg.cmd() == "JOIN")
			{
				if (msg.args().count() < 1)
				{
					qCritical() << "Got the wrong number of arguments for the JOIN message!";
				}
				else
				{
					if (msg.from().nick() == qxt_d().nickname)
					{
						qxt_d().channels[msg.args()[0]] = new QxtIrcChannel(msg.args()[0], this);
						qDebug() << "Seems I've joined a channel!" << msg.args()[0];
					}
					else
					{
						if (qxt_d().channels.contains(msg.args()[0]))
						{
							if (!qxt_d().channels[msg.args()[0]].isNull())
							{
								qxt_d().channels[msg.args()[0]]->joinUser(msg.from());
								emit userJoined(msg.args()[0], msg.from());
							}
							else
							{
								qCritical() << "Got user list for channel that doesn't exist!";
							}
						}
						else
						{
							qCritical() << "Got user list for channel that I'm not in!";
						}
					}
				}
			}
			else if (msg.cmd() == "PART")
			{
				QString reason;
				if(msg.args().count() > 1) reason = msg.args()[1];
				if (msg.from().nick() == qxt_d().nickname)
				{
					qxt_d().channels.take(msg.args()[0])->deleteLater();
					qDebug() << "Seems I've left a channel!" << msg.args()[0];
					emit(partedChannel(msg.args()[0],reason));
				}
				else
				{
					qxt_d().channels[msg.args()[0]]->partUser(msg.from(),reason);
					emit userParted(msg.args()[0], msg.from(),reason);
				}
			}
			else if (msg.cmd() == "KICK")
			{
				if(msg.args()[1] == qxt_d().nickname)
				{
					qxt_d().channels.take(msg.args()[0])->deleteLater();
					qDebug() << "Seems i've been kicked from:"<< msg.args()[0];
					emit(kicked(msg.args()[0],msg.args()[1],msg.from().nick()));
				}
				else
				{
					qxt_d().channels[msg.args()[0]]->kickUser(msg.from(),msg.args()[1],msg.from());
					emit userKicked(msg.args()[0], msg.from(),msg.args()[1],msg.from());
				}
			}
			else if (msg.cmd() == "TOPIC")
			{
				qDebug() << "Seems a topic has changed"  << msg.args(); //<< msg.args()[1] << msg.args()[2];
				if (qxt_d().channels.contains(msg.args()[0])) qxt_d().channels[msg.args()[0]]->setTopic(msg.args()[1]);
			}
			else if (msg.cmd() == "PRIVMSG")
			{
				qxt_d().testFilter(msg);
				if(msg.args().count() == 0) return;
				QString to = msg.to().nick();
				if(to.startsWith("#") && qxt_d().channels.contains(to))
				{
					qxt_d().channels[to]->sendMessage(msg);
				}
			}
			else if (msg.cmd() == "NOTICE")
			{
				qxt_d().testFilter(msg);
				if(msg.args().count() == 0) return;
				QString to = msg.to().nick();
				if(to.startsWith("#") && qxt_d().channels.contains(to))
				{
					qxt_d().channels[to]->sendNotice(msg);
				}
			}
			else if(msg.cmd() == "NICK")
			{
				qDebug() << "Nick changed from:" << msg.from().nick() << "To:" << msg.args()[0];
				if(msg.from().nick() == qxt_d().nickname)
				{
					qxt_d().nickname = msg.args()[0];
					emit(nickChanged(QxtIrcNick(msg.args()[0])));
				}
				emit(nickChanged(msg.from(),QxtIrcNick(msg.args()[0])));
				foreach(QxtIrcChannel *chan, qxt_d().channels)
				{
					chan->changeNick(msg.from(),QxtIrcNick(msg.args()[0]));
				}
			}
			else if(msg.cmd() == "QUIT")
			{
				QString message;
				if(msg.args().count() > 0)
				{
					message = msg.args()[0];
				}
				
				qDebug() << "User Quit:" << msg.from().nick() << "Message:" << message;
				
				emit(killed(message,msg.from()));
				foreach(QxtIrcChannel *chan, qxt_d().channels)
				{
					chan->quitUser(msg.from(),message);
				}
				
				emit(userQuit(msg.from(),message));
			}
			else if(msg.cmd() == "KILL")
			{
				QString message,user;
				if(msg.args().count() >0)
				{
					user = msg.args()[0];
				}
				if(msg.args().count() >1)
				{
					message = msg.args()[1];
				}
				
				qDebug() << "User Killed:" << user << "Message:" << message << "By:" << msg.from().nick();
				
				if(user == qxt_d().nickname)
				{
					emit(userKilled(user,message,msg.from()));
				}
				else
				{
					emit(killed(message,msg.from()));
					foreach(QxtIrcChannel *chan, qxt_d().channels)
					{
						chan->killUser(QxtIrcNick(user),message,msg.from());
					}
				}
			}
			else
			{
				qDebug() << "What the fuck is a" << msg.cmd();
			}
			break;
		}
		default:
			qDebug() << "Invalid state";

	}
	qDebug() << "Line:" << line;
	emit messageReceived(QxtIrcMessage(line));
}

void QxtIrcConnection::disconnected()
{
	qDebug() << "We have disconnected from the IRC server";
	qxt_d().setState(Disconnected);
}

void QxtIrcConnection::sendMessage(QString to, QString message)
{
// 	QMutexLocker locker(&mutex);
	QTextStream stream(&message);
	QString line;
	while ((line = stream.readLine()) != QString::Null())
	{
		writeMessageTo(to, "PRIVMSG", line);
	}
}

void QxtIrcConnection::sendNotice(QString to, QString message)
{
// 	QMutexLocker locker(&mutex);
	QTextStream stream(&message);
	QString line;
	while ((line = stream.readLine()) != QString::Null())
	{
		writeMessageTo(to, "NOTICE", line);
	}
}

void QxtIrcConnection::writeMessage(QxtIrcMessage msg)
{
	qDebug() << "Writing " << msg.toString().append('\n');
	qxt_d().socket->write(qPrintable(msg.toString().append('\n')));
	qxt_d().socket->waitForBytesWritten();
}

void QxtIrcConnection::sendAction(QString to, QString message)
{
	char action = static_cast<char>(1);
	QByteArray msg;
	msg.append("PRIVMSG ");
	msg.append(to);
	msg.append(" :");
	msg.append(action);
	msg.append("ACTION ");
	msg.append(message);
	msg.append(action);
	writeRawMessage(msg);
}

void QxtIrcConnection::writeMessageTo(QString to, QString cmd, QStringList args)
{
	writeMessage(QxtIrcMessage("", to, cmd, args));
}

void QxtIrcConnection::writeMessageTo(QString to, QString cmd, QString arg)
{
	writeMessage(QxtIrcMessage("", to, cmd, arg));
}

void QxtIrcConnection::writeMessage(QString cmd, QStringList args)
{
	writeMessage(QxtIrcMessage("", "", cmd, args));
}

void QxtIrcConnection::writeMessage(QString cmd, QString arg)
{
	writeMessage(QxtIrcMessage("", "", cmd, arg));
}

void QxtIrcConnection::writeRawMessage(QString msg)
{
	qxt_d().socket->write(qPrintable(msg.append('\n')));
	qxt_d().socket->waitForBytesWritten();
}

QStringList QxtIrcConnection::listChannels()
{
	return qxt_d().channels.keys();
}

QxtIrcChannel* QxtIrcConnection::getChannel(QString chan)
{
	if (!qxt_d().channels.contains(chan)) return(NULL);
	return qxt_d().channels[chan];
}


void QxtIrcConnection::joinChannel(QString channel)
{
// 	QMutexLocker locker(&mutex);
	qDebug() << "Joining a channel!" << qPrintable("JOIN " + channel);
	writeMessage("JOIN", channel);
}

QString QxtIrcConnection::nick()
{
	return(qxt_d().nickname);
}

void QxtIrcConnection::leaveChannel(QString channel, QString reason)
{
	qDebug() << "Leaving a channel!";
	writeMessage("PART", QStringList() << channel << reason);
}

void QxtIrcConnection::identify(QString password)
{
	qxt_d().password = password;
	if (getState() > Connecting)
	{
		sendMessage("NickServ", "id " + password);
	}
}

void QxtIrcConnection::addFilter(MessageScope scope, QString filter, QObject *obj, QString slot)
{
	QxtIrcConnectionPrivate::Filter f;
	f.scope = scope;
	f.filter = filter;
	f.obj = obj;

	slot = QMetaObject::normalizedSignature(qPrintable(slot));

	if (slot.startsWith("1"))
	{
		slot = slot.mid(1);
		int index = obj->metaObject()->indexOfSlot(qPrintable(slot));
		if (index == -1)
		{
			qWarning() << "WARNING: slot not found:" << slot;
		}

		index = slot.indexOf('(');
		slot = slot.left(index);

	}
	else if (slot.startsWith("2"))
	{
		slot = slot.mid(1);
		int index = obj->metaObject()->indexOfSignal(qPrintable(slot));
		if (index == -1)
		{
			qWarning() << "WARNING: signal not found:" << slot;
		}

		index = slot.indexOf('(');
		slot = slot.left(index);
	}

	f.slot = slot;
	qxt_d().filters.append(f);
}

void QxtIrcConnectionPrivate::testFilter(QxtIrcMessage msg)
{
	QxtIrcConnection::MessageScope scope = getScope(msg);

	qDebug() << "Message has scope:" << scope;

	foreach(Filter filter, filters)
	{
		if (scope & filter.scope)
		{
			if((filter.scope & QxtIrcConnection::IgnoreFakeMessages) && (msg.from().ident() == "fake")) continue;
			if (msg.args().count() > 0)
			{
				QString line = "";
				if (scope == QxtIrcConnection::BotChannelScope)
				{
					int index = msg.args()[0].indexOf(' ');
					if (index > 0)
					{
						line = msg.args()[0].mid(index + 1);
					}
				}
				else
				{
					line = msg.args()[0];
				}

				bool found = false;
				if (filter.scope & QxtIrcConnection::RegExpFilter)
				{
					QRegExp r(filter.filter);
					if (r.exactMatch(line))
						found = true;
				}
				else
				{
					if (line.startsWith(filter.filter + " ") || (line == filter.filter))
						found = true;

				}

				if (!found) continue;

				qDebug() << "Filter matches!" << filter.slot;

				QxtIrcBotMessage bmsg(line, filter.filter, msg, scope);

				if (!QMetaObject::invokeMethod(filter.obj, qPrintable(filter.slot), Qt::QueuedConnection, Q_ARG(QxtIrcBotMessage, bmsg)))
				{
					qWarning() << "WARNING: Failed to call slot for filter: " << filter.filter << filter.slot;
				}
			}
		}
	}
}

QxtIrcConnection::MessageScope QxtIrcConnectionPrivate::getScope(QxtIrcMessage msg)
{
	if (msg.to().toString() == nickname) return(QxtIrcConnection::PrivateScope);
	else if (msg.to().toString().startsWith("#"))
	{
		if (msg.args().count() > 0)
		{
			if (msg.args()[0].startsWith(nickname + ": ") || msg.args()[0].startsWith(nickname + ", "))
				return(QxtIrcConnection::BotChannelScope);
			else return(QxtIrcConnection::ChannelScope);
		}
	}
	return(QxtIrcConnection::NoScope);
}

void QxtIrcConnection::setTimeout(int secs)
{
	qxt_d().timeout=secs;
	if(qxt_d().timeout >0)
	{
		qxt_d().timeouttimer.stop();
		qxt_d().timeouttimer.start(secs*1000);
	}
}

void QxtIrcConnection::timedout()
{
	if(qxt_d().state != Connected) return;
	emit(timeout(qxt_d().timeout));
	disconnectFromHost();
}

QStringList QxtIrcConnection::listServerConfig()
{
	return(qxt_d().serverConfig.keys());
}

QVariant QxtIrcConnection::getServerConfig(QString name, QVariant def)
{
	if(qxt_d().serverConfig.contains(name))
	{
		return(qxt_d().serverConfig[name]);
	}
	else
	{
		return(def);
	}
}
