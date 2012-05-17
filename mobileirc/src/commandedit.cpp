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
#include "commandedit.h"
#include <QDebug>
#include <QKeyEvent>

CommandEdit::CommandEdit(QWidget *parent)
		: QLineEdit(parent)
{
	_historylimit=100;
	_currentitem=-1;

        _irccommands << "/join";
        _irccommands << "/ns";
        _irccommands << "/cs";
        _irccommands << "/admin";
        _irccommands << "/away";
        _irccommands << "/connect";
        _irccommands << "/die";
        _irccommands << "/error";
        _irccommands << "/info";
        _irccommands << "/invite";
        _irccommands << "/ison";
        _irccommands << "/join";
        _irccommands << "/kick";
        _irccommands << "/kill";
        _irccommands << "/links";
        _irccommands << "/list";
        _irccommands << "/lusers";
        _irccommands << "/mode";
        _irccommands << "/motd";
        _irccommands << "/names";
        _irccommands << "/nick";
        _irccommands << "/notice";
        _irccommands << "/oper";
        _irccommands << "/part";
        _irccommands << "/pass";
        _irccommands << "/ping";
        _irccommands << "/pong";
        _irccommands << "/privmsg";
        _irccommands << "/quit";
        _irccommands << "/rehash";
        _irccommands << "/restart";
        _irccommands << "/service";
        _irccommands << "/servlist";
        _irccommands << "/server";
        _irccommands << "/squery";
        _irccommands << "/squit";
        _irccommands << "/stats";
        _irccommands << "/summon";
        _irccommands << "/time";
        _irccommands << "/topic";
        _irccommands << "/trace";
        _irccommands << "/user";
        _irccommands << "/userhost";
        _irccommands << "/users";
        _irccommands << "/version";
        _irccommands << "/wallops";
        _irccommands << "/who";
        _irccommands << "/whois";
        _irccommands << "/whowas";
}


CommandEdit::~CommandEdit()
{
}


bool CommandEdit::event(QEvent *event)
{	
	if(event->type() != QEvent::KeyPress) return(QLineEdit::event(event));
	QKeyEvent *keyevent = (QKeyEvent *)event;
	
	if(keyevent->key() == Qt::Key_Tab)
	{
		int index = text().lastIndexOf(" ",cursorPosition()-1);
		QString part = text().mid(index+1,(cursorPosition())-(index+1));
		qDebug() << "PART:" << part << index << cursorPosition() << index+1 << (cursorPosition())-(index);
		
                if(index < 0 && text().startsWith("/"))
                {
                    qDebug() << "COMMDN_PART" << part;

                    foreach(QString cmd, _irccommands)
                    {
                        qDebug() << "CMD: " << cmd;
                        if(cmd.toLower().startsWith(part.toLower()))
                        {
                            QString tmp = text().remove(index+1,(cursorPosition())-(index+1));
                            tmp = tmp.insert(index+1,cmd);
                            setText(tmp);
                            setCursorPosition(index+cmd.size()+1);
                            break;
                        }
                    }
                }
                else
                {
                    foreach(QxtIrcNick nick, _nicks)
                    {
                            if(nick.nick().toLower().startsWith(part.toLower()))
                            {
                                    QString tmp = text().remove(index+1,(cursorPosition())-(index+1));
                                    tmp = tmp.insert(index+1,nick.nick());
                                    if(index <= 0)
                                    {
                                            tmp += ": ";
                                            setText(tmp);
                                    }
                                    else
                                    {
                                            setText(tmp);
                                            setCursorPosition(index+nick.nick().size()+1);
                                    }
                            }
                    }
                }

		if(_currentitem == -1) _currenttext = text();
		
		return(true);
	}
	else if(keyevent->key() == Qt::Key_Down)
	{
		if(_currentitem == -1) return(true);
		_currentitem--;
		
                if(_currentitem == -1)
		{
			setText(_currenttext);
			return(true);
		}
		else
		{
			setText(_history[(_history.count()-1) - _currentitem]);
		}
		
		return(true);
	}
	else if(keyevent->key() == Qt::Key_Up)
	{
		if(_currentitem >= (_history.count() -1)) return(true);
		_currentitem++;
		
		setText(_history[(_history.count()-1) - _currentitem]);
		
		return(true);
	}
	else if(((keyevent->modifiers() & Qt::ControlModifier) == Qt::ControlModifier) && keyevent->key() == Qt::Key_U)
	{
		int pos = cursorPosition();
		pos+=2;
		setText(text().insert(cursorPosition(),"%U"));
		setCursorPosition(pos);
		return(true);
	}
	else if(((keyevent->modifiers() & Qt::ControlModifier) == Qt::ControlModifier) && keyevent->key() == Qt::Key_B)
	{
		int pos = cursorPosition();
		pos+=2;
		setText(text().insert(cursorPosition(),"%B"));
		setCursorPosition(pos);
		return(true);
	}
	else if(((keyevent->modifiers() & Qt::ControlModifier) == Qt::ControlModifier) && keyevent->key() == Qt::Key_I)
	{
		int pos = cursorPosition();
		pos+=2;
		setText(text().insert(cursorPosition(),"%I"));
		setCursorPosition(pos);
		return(true);
	}
	else if(((keyevent->modifiers() & Qt::ControlModifier) == Qt::ControlModifier) && keyevent->key() == Qt::Key_R)
	{
		int pos = cursorPosition();
		pos+=2;
		setText(text().insert(cursorPosition(),"%R"));
		setCursorPosition(pos);
		return(true);
	}
	else if(((keyevent->modifiers() & Qt::ControlModifier) == Qt::ControlModifier) && keyevent->key() == Qt::Key_K)
	{
		int pos = cursorPosition();
		pos+=2;
		setText(text().insert(cursorPosition(),"%C"));
		setCursorPosition(pos);
		return(true);
	}
	
	if(_currentitem == -1) _currenttext = text();
	
	return(QLineEdit::event(event));
}

void CommandEdit::setNickList(QList<QxtIrcNick> nicks)
{
	_nicks = nicks;
}

void CommandEdit::setHistoryLimit(int num)
{
	_historylimit=num;
}

void CommandEdit::addToHistory()
{
	if(_history.size() == _historylimit) _history.dequeue();
	_history.enqueue(text());
	_currenttext="";
	_currentitem=-1;
}
