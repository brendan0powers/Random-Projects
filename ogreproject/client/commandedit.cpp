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
#include "commandedit.h"
#include <QKeyEvent>

CommandEdit::CommandEdit(QWidget *parent)
 : QLineEdit(parent)
{
}


CommandEdit::~CommandEdit()
{
}

bool CommandEdit::event(QEvent *event)
{
	if(event->type() != QEvent::KeyPress) return(QLineEdit::event(event));
	QKeyEvent *keyevent = (QKeyEvent *)event;

	if(keyevent->key() == Qt::Key_Enter)
	{
		emit(commandEntered(text()));
		clear();
	}
	
	return(QLineEdit::event(event));
}
