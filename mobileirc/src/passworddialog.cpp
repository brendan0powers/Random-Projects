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


#include "passworddialog.h"

PasswordDialog::PasswordDialog(QWidget* parent, Qt::WFlags fl)
		: QDialog(parent, fl), Ui::PasswordDialog()
{
	setupUi(this);
	UserEntry->setFocus();
}

PasswordDialog::~PasswordDialog()
{
}

QString PasswordDialog::user()
{
	return(UserEntry->text());
}

QString PasswordDialog::password()
{
	return(PasswordEntry->text());
}

void PasswordDialog::setUser(QString user)
{
	UserEntry->setText(user);
}

void PasswordDialog::setPassword(QString password)
{
	PasswordEntry->setText(password);
}

void PasswordDialog::setUserEnabled(bool enabled)
{
	UserEntry->setEnabled(enabled);
	
	if(enabled)
		UserEntry->setFocus();
	else
		PasswordEntry->setFocus();
}

void PasswordDialog::focusPassword()
{
	PasswordEntry->setFocus();
}
