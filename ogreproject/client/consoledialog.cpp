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


#include "consoledialog.h"

using namespace Ogre;

ConsoleDialog::ConsoleDialog(QWidget* parent, Qt::WFlags fl)
: QWidget( parent, fl ), Ui::ConsoleDialog()
{
	setupUi(this);
	LogManager::getSingleton().getDefaultLog()->addListener(this);
	_listener=NULL;
	
	QObject::connect(CommandEntry,SIGNAL(commandEntered(QString)),this,SLOT(commandEntered(QString)));
}

ConsoleDialog::~ConsoleDialog()
{
}

void ConsoleDialog::setListener(OgreConsoleListener *l)
{
	_listener = l;
}

void ConsoleDialog::append(QString html)
{
	QString text;
	bool bold = false;
	bool italic = false;
	bool underline = false;
	bool color = false;

	html = html.replace("&","&amp;");
	html = html.replace("<","&lt;");
	html = html.replace(">","&amp;");
	html = html.replace("%WHITE",QString() + (char)3 + "00");
	html = html.replace("%BLACK",QString() + (char)3 + "01");
	html = html.replace("%NAVY",QString() + (char)3 + "02");
	html = html.replace("%GREEN",QString() + (char)3 + "03");
	html = html.replace("%RED",QString() + (char)3 + "04");
	html = html.replace("%BROWN",QString() + (char)3 + "05");
	html = html.replace("%PURPLE",QString() + (char)3 + "06");
	html = html.replace("%OLIVE",QString() + (char)3 + "07");
	html = html.replace("%YELLOW",QString() + (char)3 + "08");
	html = html.replace("%LIME",QString() + (char)3 + "09");
	html = html.replace("%TEAL",QString() + (char)3 + "10");
	html = html.replace("%AQUA",QString() + (char)3 + "11");
	html = html.replace("%BLUE",QString() + (char)3 + "12");
	html = html.replace("%PINK",QString() + (char)3 + "13");
	html = html.replace("%GREY",QString() + (char)3 + "15");
	html = html.replace("%SILVER",QString() + (char)3 + "16");
	html = html.replace("%B", QString() + (char)2);
	html = html.replace("%C", QString() + (char)3);
	html = html.replace("%I", QString() + (char)9);
	html = html.replace("%U", QString() + (char)31);
	html = html.replace("%R", QString() + (char)15);
	
	QByteArray buff = html.toAscii();
	for (int i = 0; i < buff.length(); i++)
	{
		switch (buff[i])
		{
			case 2: //bold
				if (!bold)
				{
					text += "<b>";
					bold = true;
				}
				else
				{
					text += "</b>";
					bold = false;
				}
				break;
				case 15: //Reset
					if (bold) text += "</b>";
					if (italic) text += "</i>";
					if (underline) text += "</u>";
					if (color) text += "</font>";
					bold = italic = underline = color = false;
					break;
			case 9:
				case 22: //Italic
					if (!italic)
					{
						text += "<i>";
						italic = true;
					}
					else
					{
						text += "</i>";
						italic = false;
					}
					break;
					case 31: //Underline
						if (!underline)
						{
							text += "<u>";
							underline = true;
						}
						else
						{
							text += "</u>";
							underline = false;
						}
						break;
						case 3: //Color
							if (!color)
							{
								if ((i + 2) < buff.length())
								{
									QString scolor = QString() + buff[i+1];
									scolor += buff[i+2];

									QString name;
									if (scolor == "00") name = "white";
									else if (scolor == "01") name = "black";
									else if (scolor == "02") name = "navy";
									else if (scolor == "03") name = "green";
									else if (scolor == "04") name = "red";
									else if (scolor == "05") name = "brown";
									else if (scolor == "06") name = "purple";
									else if (scolor == "07") name = "olive";
									else if (scolor == "08") name = "yellow";
									else if (scolor == "09") name = "lime";
									else if (scolor == "10") name = "teal";
									else if (scolor == "11") name = "aqua";
									else if (scolor == "12") name = "blue";
									else if (scolor == "13") name = "pinK";
									else if (scolor == "14") name = "grey";
									else if (scolor == "15") name = "silver";
									else if (scolor == "16") name = "white";

									i += 2;
									text += "<font color=" + name + ">";
									color = true;
									break;
								}
							}
							else
							{
								text += "</font>";
								color = false;
								break;
							}
			default:
				text += buff[i];
				break;
		}
	}

	TextView->append(text);
}

void ConsoleDialog::messageLogged(const String& message, LogMessageLevel lml, bool maskDebug, const String &logName)
{
	append(message.c_str());
}

void ConsoleDialog::commandEntered(QString command)
{
	QStringList list = command.split(" ",QString::SkipEmptyParts);
	if(_listener != NULL) _listener->consoleCommand(list);
}

void ConsoleDialog::focus()
{
	setFocus();
	CommandEntry->setFocus();
}