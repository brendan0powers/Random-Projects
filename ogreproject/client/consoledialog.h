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

#ifndef CONSOLEDIALOG_H
#define CONSOLEDIALOG_H

#include <QWidget>
#include "ui_ConsoleDialog.h"
#include <Ogre.h>

class OgreConsoleListener
{
public:
	virtual void consoleCommand(QStringList& command) = 0;
};

class ConsoleDialog : public QWidget, private Ui::ConsoleDialog, public Ogre:: LogListener
{
	Q_OBJECT

public:
	ConsoleDialog(QWidget* parent = 0, Qt::WFlags fl = 0);
	~ConsoleDialog();

	void setListener(OgreConsoleListener *l);
	void focus();

protected slots:
	void commandEntered(QString command);
	
protected:
	void append(QString text);
	void messageLogged(const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName);

private:
	OgreConsoleListener *_listener;

};

#endif

