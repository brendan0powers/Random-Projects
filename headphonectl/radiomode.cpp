/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) <year>  <name of author>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "radiomode.h"
#include <QStringList>
#include <QDebug>
#include <stdlib.h>

RadioMode::RadioMode()
{
	_proc = new QProcess(NULL);
}


void RadioMode::start()
{
}

void RadioMode::end()
{
}


void RadioMode::left()
{
	qDebug() << "OH";
        _proc->start("sh", QStringList() << "-c" << "arecord -D hw:NVidia -f S16_LE -c 2 | aplay");
}

void RadioMode::middle()
{

}

void RadioMode::right()
{
	_proc->kill();
        system("killall arecord");
        system("killall aplay");
}

