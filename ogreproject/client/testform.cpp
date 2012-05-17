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


#include "testform.h"
#include <QDebug>

TestForm::TestForm(QWidget* parent, Qt::WFlags fl)
: QDialog( parent, fl ), Ui::Dialog()
{
	setupUi(this);
	Web->setUrl(QUrl("http://www.google.com"));
}

TestForm::~TestForm()
{
}

void TestForm::on_AddressEntry_returnPressed()
{
	qDebug() << "ADDRESS:" << QUrl(AddressEntry->text());
	Web->load(QUrl(AddressEntry->text()));
}

void TestForm::on_Web_urlChanged(QUrl url)
{
	AddressEntry->setText(url.toString());
}
