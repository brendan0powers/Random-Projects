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


#include "channelwidget.h"
#include <QDebug>
#include <QScrollBar>

ChannelWidget::ChannelWidget(QString name, QxtIrcConnection *c, QWidget* parent, Qt::WFlags fl)
		: QWidget(parent, fl), Ui::Form(), ChannelBase()
{
	setupUi(this);
	_client = c;
	clear();
	_name = name;
	_channel = _client->getChannel(name);

	QObject::connect(_channel, SIGNAL(newMessage(QxtIrcMessage)), this, SLOT(messageReceived(QxtIrcMessage)));
	QObject::connect(_channel, SIGNAL(newNotice(QxtIrcMessage)), this, SLOT(messageReceived(QxtIrcMessage)));
	QObject::connect(_channel, SIGNAL(userJoined(QxtIrcNick)),this,SLOT(userJoined(QxtIrcNick)));
	QObject::connect(_channel, SIGNAL(userParted(QxtIrcNick, QString)), this, SLOT(userParted(QxtIrcNick, QString)));
	QObject::connect(_channel, SIGNAL(topicChanged(QString, QString)), this, SLOT(topicChanged(QString, QString)));
	QObject::connect(_channel, SIGNAL(nickChanged(QxtIrcNick, QxtIrcNick)), this, SLOT(nickChanged(QxtIrcNick, QxtIrcNick)));
	QObject::connect(_channel, SIGNAL(userKicked(QxtIrcNick, QString, QxtIrcNick)), this, SLOT(userKicked(QxtIrcNick, QString, QxtIrcNick)));
	QObject::connect(_channel, SIGNAL(userKilled(QxtIrcNick, QString, QxtIrcNick)), this, SLOT(userKilled(QxtIrcNick, QString, QxtIrcNick)));
	QObject::connect(_channel, SIGNAL(userQuit(QxtIrcNick, QString)), this, SLOT(userQuit(QxtIrcNick, QString)));
	QObject::connect(NickView, SIGNAL(activated(QModelIndex)), this, SLOT(nickActivated(QModelIndex)));
	
	_model = new ChannelModel(_channel,this);
	NickView->setModel(_model);
	
	TextEntry->setNickList(_channel->listUsers());
	
	append("%OLIVE%BTOPIC:%B " + _channel->topic());
}

ChannelWidget::~ChannelWidget()
{
}

void ChannelWidget::append(QString html)
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

void ChannelWidget::clear()
{
	TextView->clear();
}

void ChannelWidget::send()
{
	on_SendButton_clicked();
}

void ChannelWidget::on_SendButton_clicked()
{
	if (TextEntry->text() == "") return;

	if (TextEntry->text().startsWith("/"))
	{
		emit(command(TextEntry->text(),_name,MainWindow::Channel,this));
		TextEntry->addToHistory();
		TextEntry->clear();
	}
	else
	{
		QString html = TextEntry->text();
		
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
		
		_client->sendMessage(_name, html);
		append("%RED" + _client->nick() + "%R: " + html);
		TextEntry->addToHistory();
		TextEntry->clear();
	}
}

void ChannelWidget::messageReceived(QxtIrcMessage msg)
{
	if (msg.cmd() == "PRIVMSG")
	{
		if (msg.args().count() == 0) return;
		if(msg.args()[0].startsWith(QString() + (char)1 + "ACTION"))
		{
			QString txt = msg.args()[0].mid(7,msg.args()[0].size()-8);
			append("%BLUE *%R " + msg.from().nick() + " %BLUE" + txt);
		}
		else
		{
			append("%BLUE" + msg.from().nick() + "%R: " + msg.args()[0]);
		}
		
	}
	else 	if (msg.cmd() == "NOTICE")
	{
		if (msg.args().count() == 0) return;
		append("%OLIVE[NOTICE]" + msg.from().nick() + "%R: " + msg.args()[0]);
	}
	else return;
	
	emit(newMessage(this));
	if(msg.args().join(" ").contains(_client->nick(),Qt::CaseInsensitive))
		emit(nickMentioned(this));
}

void ChannelWidget::focus()
{
	if(NickView->isVisible())
	{
		NickView->setFocus();
	}
	else
	{
		TextEntry->setFocus();
	}
}

void ChannelWidget::showNickList(bool show)
{
	if (show)
	{
		NickView->show();
		NickView->setFocus();
	}
	else
	{
		NickView->hide();
		TextEntry->setFocus();
	}
}

void ChannelWidget::activate()
{
	emit(activated(this));
}

void ChannelWidget::userJoined(QxtIrcNick user)
{
	append("%BROWN%B" + user.nick() + "%B joined the channel.");
	_model->update();
	TextEntry->setNickList(_channel->listUsers());
}

void ChannelWidget::userParted(QxtIrcNick user, QString reason)
{
	append("%BROWN%B" + user.nick() + "%B left the channel: " + reason);
	_model->update();
	TextEntry->setNickList(_channel->listUsers());
}

void ChannelWidget::topicChanged(QString channel, QString topic)
{
	append("%OLIVE%BTOPIC:%B " + topic);
}

void ChannelWidget::nickChanged(QxtIrcNick oldnick, QxtIrcNick newnick)
{
	append("%BROWN%B" + oldnick.nick() + "%B is now known as %B" + newnick.nick() + "%B");
	_model->update();
	TextEntry->setNickList(_channel->listUsers());
}

void ChannelWidget::userKicked(QxtIrcNick user, QString reason, QxtIrcNick from)
{
	append("%BROWN%B" + user.nick() + "%B has been kicked by %B" + from.nick() + "%B : " + reason);
	_model->update();
	TextEntry->setNickList(_channel->listUsers());
}

void ChannelWidget::userKilled(QxtIrcNick user, QString reason, QxtIrcNick from)
{
	append("%RED%B" + user.nick() + "%B has been killed by %B" + from.nick() + "%B : " + reason);
	_model->update();
	TextEntry->setNickList(_channel->listUsers());
}

void ChannelWidget::userQuit(QxtIrcNick user, QString reason)
{
	append("%BROWN%B" + user.nick() + "%B has quit: " + reason);
	_model->update();
	TextEntry->setNickList(_channel->listUsers());
}

void ChannelWidget::scroll(bool up)
{
	if(up)
		TextView->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepSub);
	else
		TextView->verticalScrollBar()->triggerAction(QAbstractSlider::SliderPageStepAdd);
}

QString ChannelWidget::name()
{
	return(_name);
}

void ChannelWidget::nickActivated(QModelIndex index)
{
	QString name = _model->getNickFromIndex(index);
	emit(newPrivateChannel(name,""));
}