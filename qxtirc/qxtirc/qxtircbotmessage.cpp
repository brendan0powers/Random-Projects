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
#include "qxtircbotmessage.h"
#include "qxtircbotmessage_p.h"
#include <QDebug>

#define MSG_QUOTE 0
#define MSG_DOUBLEQUOTE 1
#define MSG_ARG 2
#define MSG_STARTARG 3

QxtIrcBotMessage::QxtIrcBotMessage()
{
	QXT_INIT_PRIVATE(QxtIrcBotMessage);
}

QxtIrcBotMessage::QxtIrcBotMessage(QString msg, QString filter, QxtIrcMessage message, QxtIrcConnection::MessageScope scope)
{
	QXT_INIT_PRIVATE(QxtIrcBotMessage);
	
	qxt_d().parse(msg);
	qxt_d().msg = message;
	qxt_d().msgtext = msg;
	qxt_d().filter=filter;
	qxt_d().to = qxt_d().msg.to();
	qxt_d().from = qxt_d().msg.from();
	qxt_d().scope = scope;
}

QxtIrcBotMessage::QxtIrcBotMessage(const QxtIrcBotMessage &msg)
{
	QXT_INIT_PRIVATE(QxtIrcBotMessage);
	

	qxt_d().msg = msg.qxt_d().msg;
	qxt_d().msgtext = msg.qxt_d().msgtext;
	qxt_d().filter=msg.qxt_d().filter;
	qxt_d().to = msg.qxt_d().to;
	qxt_d().from = msg.qxt_d().from;
	qxt_d().scope = msg.qxt_d().scope;
}


QxtIrcBotMessage::~QxtIrcBotMessage()
{
}

QxtIrcNick QxtIrcBotMessage::to() const
{
	return(qxt_d().to);
}

QxtIrcNick QxtIrcBotMessage::from() const
{
	return(qxt_d().from);
}

QStringList QxtIrcBotMessage::args() const
{
	return(qxt_d().args);
}

QString QxtIrcBotMessage::filter() const
{
	return(qxt_d().filter);
}

QString QxtIrcBotMessage::msgText() const
{
	return(qxt_d().msgtext);
}

QxtIrcMessage QxtIrcBotMessage::msg() const
{
	return(qxt_d().msg);
}

QxtIrcConnection::MessageScope QxtIrcBotMessage::scope() const
{
	return(qxt_d().scope);
}

void QxtIrcBotMessagePrivate::parse(QString msg)
{
	QString tmparg="";
	int state=MSG_STARTARG;
	
	msg = msg.trimmed();
	char *buf = new char[msg.length()+1];
	strcpy(buf, qPrintable(msg));
	
	for (int pos=0; pos < msg.length(); pos++)
	{
		switch (state)
		{
			case MSG_STARTARG:
			{
				if (buf[pos] == '"')
					state = MSG_DOUBLEQUOTE;
				else if (buf[pos] == '\'')
					state = MSG_QUOTE;
				else
				{
					tmparg += buf[pos];
					state = MSG_ARG;
				}
			}
			break;
			case MSG_ARG:
			{
				if (buf[pos] != ' ')
				{
					tmparg += buf[pos];
				}
				else
				{
					args << tmparg;
					tmparg="";
					
					pos = nextChar(pos, msg.length(), buf);
					if (pos == -1)
					{
						delete buf;
						return;
					}
					state = MSG_STARTARG;
				}
			}
			break;
			case MSG_QUOTE:
			{
                                if (buf[pos] == '\\' &&  msg.length() > pos+1)
                                {
                                        pos++;
                                        if(buf[pos] == '\'')
                                        {
                                                tmparg += '\'';
                                        }
                                        else if(buf[pos] == '\\')
                                        {
                                                tmparg += '\\';
                                        }
                                        else
                                        {
                                                //if it's not an escapable character, treat it as a backslash...
                                                pos--;
                                                tmparg += '\\';
                                        }
                                }
				else if (buf[pos] != '\'')
				{
					tmparg += buf[pos];
				}
				else
				{
					args << tmparg;
					tmparg="";
					
					pos = nextChar(++pos, msg.length(), buf);
					if (pos == -1)
					{
						delete buf;
						return;
					}
					state = MSG_STARTARG;
				}
			}
			break;
			case MSG_DOUBLEQUOTE:
			{
				if (buf[pos] == '\\' &&  msg.length() > pos+1)
				{
					pos++;
					if(buf[pos] == '"')
					{
						tmparg += '"';
					}
					else if(buf[pos] == '\\')
					{
						tmparg += '\\';
					}
					else
					{
						//if it's not an escapable character, treat it as a backslash...
						pos--;
						tmparg += '\\';
					}
				}
				else if (buf[pos] != '"')
				{
					tmparg += buf[pos];
				}
				else
				{
					args << tmparg;
					tmparg="";
					
					pos = nextChar(++pos, msg.length(), buf);
					if (pos == -1)
					{
						delete buf;
						return;
					}
					state = MSG_STARTARG;
				}
			}
		}
	}
	
	if(tmparg != "") args << tmparg;
}

int QxtIrcBotMessagePrivate::nextChar(int start, int end, const char *buf)
{
	for (int i = start; i < end; i++)
	{
		if (buf[i] != ' ') return(i -1);
	}
	//if we got here, we reached the end of the string without finding any characters
	return(-1);
}

QString QxtIrcBotMessage::reply() const
{
	if(qxt_d().scope == QxtIrcConnection::PrivateScope)
	{
		return(qxt_d().from.nick());	
	}
	else
	{
		return(qxt_d().to.nick());
	}
}

QString QxtIrcBotMessage::replyPrivate() const
{
	return(qxt_d().from.nick());
}

QString QxtIrcBotMessage::replyChannel() const
{
	return(qxt_d().to.nick());
}

bool QxtIrcBotMessage::match(QString format)
{
	QStringList parts = format.split(" ",QString::SkipEmptyParts);
	QStringList args = qxt_d().args;
	if(args.count() != 0) args.removeAt(0);
	return match(parts, args);
}

bool QxtIrcBotMessage::matchAll(QString format)
{
	QStringList parts = format.split(" ",QString::SkipEmptyParts);
	return match(parts, qxt_d().args);
}

bool QxtIrcBotMessage::match(QStringList parts, QStringList args)
{	
	bool found=false;
	foreach(QString part, parts)
	{
		qDebug() << "PART:" << part;
		if(part == "(...)")
		{
			parts.removeAll(part);
			qDebug() << "Found part!";
			found=true;
		}
	}
	
	foreach(QString arg, args)
	{
		qDebug() << "ARG:" << arg;
	}
	
	if(found)
	{
		if(parts.count() > args.count()) return(false); 
	}
	else
	{
		qDebug() << "Checking:" << parts.count() << args.count();
		if(parts.count() != args.count()) return(false); 
	}
	
	if(parts.count() == 0 && args.count() == 0) return(true);
	
	for(int i=0; i < parts.count(); i++)
	{
		if(parts[i] == "(number)")
		{
			bool ok;
			args[i].toInt(&ok);
			if(!ok) 
			{
				return(false);
			}
			continue;
		}
		else if(parts[i] == "(channel)")
		{
			if(!args[i].startsWith("#")) 
			{
				return(false);
			}
			continue;
		}
		else if(parts[i] == "(string)")
		{
			//Is there really anything to check here? -- yes, there is.
			if(args[i].isEmpty())
			{
//				qDebug() << "The string is null";
				return false;
			}
			continue;
		}
		else if(parts[i] == "(*)")
		{
			//ignore this argument
		}
		else if(parts[i] == "(...)")
		{
			//stop here
			return(true);
		}
		else
		{
			if(parts[i].toLower() != args[i].toLower())
			{
//				qDebug() << parts[i].toLower() << " != " << args[i].toLower();
				return(false);
			}
		}
	}
	
	//If we get here, everything checks out
	return(true);
}

