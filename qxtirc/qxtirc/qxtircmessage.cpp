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
#include "qxtircmessage.h"
#include "qxtircmessage_p.h"
#include <QDebug>
#include <string.h>

#define MSG_FROM 0
#define MSG_CMD 1
#define MSG_TO 2
#define MSG_ARGS 3
#define MSG_LASTARG 4

QxtIrcMessage::QxtIrcMessage()
{
	QXT_INIT_PRIVATE(QxtIrcMessage);

	qxt_d().from = "";
	qxt_d().to = "";
	qxt_d().cmd = "";
	qxt_d().args = QStringList();
}

QxtIrcMessage::QxtIrcMessage(QString msg)
{
	QXT_INIT_PRIVATE(QxtIrcMessage);

	qxt_d().parse(msg);
}

QxtIrcMessage::QxtIrcMessage(QString f, QString t, QString c, QStringList a)
{
	QXT_INIT_PRIVATE(QxtIrcMessage);
	qxt_d().from = f;
	qxt_d().to = t;
	qxt_d().cmd = c;
	qxt_d().args = a;

}

QxtIrcMessage::QxtIrcMessage(QString f, QString t, QString c, QString a)
{
	QXT_INIT_PRIVATE(QxtIrcMessage);
	qxt_d().from = f;
	qxt_d().to = t;
	qxt_d().cmd = c;
	qxt_d().args.clear();
	qxt_d().args = QStringList() << a;
}

QxtIrcMessage::QxtIrcMessage(const QxtIrcMessage &msg)
{
	QXT_INIT_PRIVATE(QxtIrcMessage);
	qxt_d().from = msg.qxt_d().from;
	qxt_d().to = msg.qxt_d().to;
	qxt_d().cmd = msg.qxt_d().cmd;
	qxt_d().args.clear();
	qxt_d().args = msg.qxt_d().args;
}

QxtIrcMessage::~QxtIrcMessage()
{
}

QxtIrcNick QxtIrcMessage::from() const
{
	return(qxt_d().from.toString());
}

QxtIrcNick QxtIrcMessage::to() const
{
	return(qxt_d().to.toString());
}

QString QxtIrcMessage::cmd() const
{
	return(qxt_d().cmd);
}

int QxtIrcMessage::toNumber() const
{
	bool ok = false;
	int num = qxt_d().cmd.toInt(&ok);
	if (!ok) return(-1);
	if (num < 100 || num > 999) return(-1);
	return(num);
}

QStringList QxtIrcMessage::args() const
{
	return(qxt_d().args);
}

void QxtIrcMessage::setFrom(QxtIrcNick f)
{
	qxt_d().from = f;
}

void QxtIrcMessage::setTo(QxtIrcNick t)
{
	qxt_d().to = t;
}

void QxtIrcMessage::setCmd(QString c)
{
	qxt_d().cmd = c;
}

void QxtIrcMessage::setArgs(QStringList a)
{
	qxt_d().args = a;
}

QString QxtIrcMessage::toString() const
{
	QString msg;
	if (qxt_d().from.toString() != "")
		msg += ":" + qxt_d().from.toString() + " ";

	msg += qxt_d().cmd;

	if (qxt_d().to.toString() != "")
	{
		msg += " " + qxt_d().to.toString();
	}

	for (int i = 0; i < qxt_d().args.size(); i++)
	{
		if ((i == qxt_d().args.size() - 1) && ((qxt_d().args[i].indexOf(' ') != -1) || qxt_d().cmd == "PRIVMSG"))
		{
			msg += " :" + qxt_d().args[i];
		}
		else
		{
			msg += " " + qxt_d().args[i];
		}
	}

	return(msg);
}

//:from CMD to arg1 arg2 :arg3 has spaces

void QxtIrcMessagePrivate::parse(QString msg)
{
	QString fromstr = "";
	QString tostr = "";
	cmd = "";
	args = QStringList();
	QString lastarg;
	QString argstr;

	if (msg.length() != strlen(qPrintable(msg))) return;

	char *buf = new char[msg.size()+1];
	memcpy(buf, qPrintable(msg), msg.size());
	buf[msg.size()] = '\0';

	int state = MSG_CMD;
	int pos = 0;
	if (msg.startsWith(":"))
	{
		state = MSG_FROM;
		pos = 1;
	}

	for (; pos < msg.size(); pos++)
	{
		switch (state)
		{
			case MSG_FROM:
				if (buf[pos] != ' ')
				{
					fromstr += buf[pos];
				}
				else
				{
					from = fromstr;
					pos = nextChar(pos, msg.length(), buf);
					if (pos == -1)
					{
						delete buf;
						return;
					}
					state = MSG_CMD;
				}
				break;
			case MSG_CMD:
				if (buf[pos] != ' ')
					cmd += buf[pos];
				else
				{
					pos = nextChar(pos, msg.length(), buf);
					if (pos == -1)
					{
						delete buf;
						return;
					}
					//check to see if this is a command that has a to argument
					if (cmd == "PRIVMSG" || cmd == "NOTICE") state = MSG_TO;
					else state = MSG_ARGS;
				}
				break;
			case MSG_TO:
				if (buf[pos] != ' ')
					tostr += buf[pos];
				else
				{
					to = tostr;
					pos = nextChar(pos, msg.length(), buf);
					if (pos == -1)
					{
						delete buf;
						return;
					}
					state = MSG_ARGS;
				}
				break;
			case MSG_ARGS:
				if (buf[pos] == ':')
				{
					state = MSG_LASTARG;
					continue;
				}
				else if (buf[pos] != ' ')
				{
					argstr += buf[pos];
				}
				if ((pos + 1) == msg.length())
				{
					args.append(argstr);
					argstr = "";
					delete buf;
					return;
				}
				if (buf[pos] == ' ')
				{
					args.append(argstr);
					argstr = "";
					pos = nextChar(pos, msg.length(), buf);
					if (pos == -1)
					{
						delete buf;
						return;
					}
				}
				break;
			case MSG_LASTARG:
				lastarg += buf[pos];
				break;
		}
	}

	if (state == MSG_LASTARG) args.append(lastarg);
	delete buf;
}

int QxtIrcMessagePrivate::nextChar(int start, int end, const char *buf)
{
	for (int i = start; i < end; i++)
	{
		if (buf[i] != ' ') return(i -1);
	}
	//if we got here, we reached the end of the string without finding any characters
	return(-1);
}
