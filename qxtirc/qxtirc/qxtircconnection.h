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
#ifndef QXTIRCCONNECTION_H
#define QXTIRCCONNECTION_H

#include <QObject>
#include <QMutex>
#include <QHash>
#include <QTime>
#include <QDateTime>
#include <QPointer>
#include <qxtircmessage.h>
#include <qxtircchannel.h>
#include <qxtpimpl.h>

class QTcpSocket;
class QxtIrcConnectionPrivate;

/**
	@author Chris Vickery <chrisinajar@gmail.com>
 */
class QxtIrcConnection : public QObject
{
	Q_OBJECT
public:
	enum State
	{
		Disconnected,
		Connecting,
		Registering,
		Connected
	};

	enum ResponseCodes
	{
		REG_SERVERCONFIG = 5,
		ERR_NOSUCHNICK = 401,
		ERR_NOSUCHSERVER = 402,
		ERR_NOSUCHCHANNEL = 403,
		ERR_CANNOTSENDTOCHAN = 404,
		ERR_TOOMANYCHANNELS = 405,
		ERR_WASNOSUCHNICK = 406,
		ERR_TOOMANYTARGETS = 407,
		ERR_NOORIGIN = 409,
		ERR_NORECIPIENT = 411,
		ERR_NOTEXTTOSEND = 412,
		ERR_NOTOPLEVEL = 413,
		ERR_WILDTOPLEVEL = 414,
		ERR_UNKNOWNCOMMAND = 421,
		ERR_NOMOTD = 422,
		ERR_NOADMININFO = 423,
		ERR_FILEERROR = 424,
		ERR_NONICKNAMEGIVEN = 431,
		ERR_ERRONEUSNICKNAME = 432,
		ERR_NICKNAMEINUSE = 433,
		ERR_NICKCOLLISION = 436,
		ERR_USERNOTINCHANNEL = 441,
		ERR_NOTONCHANNEL = 442,
		ERR_USERONCHANNEL = 443,
		ERR_NOLOGIN = 444,
		ERR_SUMMONDISABLED = 445,
		ERR_USERSDISABLED = 446,
		ERR_NOTREGISTERED = 451,
		ERR_NEEDMOREPARAMS = 461,
		ERR_ALREADYREGISTRED = 462,
		ERR_NOPERMFORHOST = 463,
		ERR_PASSWDMISMATCH = 464,
		ERR_YOUREBANNEDCREEP = 465,
		ERR_KEYSET = 467,
		ERR_CHANNELISFULL = 471,
		ERR_UNKNOWNMODE = 472,
		ERR_INVITEONLYCHAN = 473,
		ERR_BANNEDFROMCHAN = 474,
		ERR_BADCHANNELKEY = 475,
		ERR_NOPRIVILEGES = 481,
		ERR_CHANOPRIVSNEEDED = 482,
		ERR_CANTKILLSERVER = 483,
		ERR_NOOPERHOST = 491,
		ERR_UMODEUNKNOWNFLAG = 501,
		ERR_USERSDONTMATCH = 502,
		RPL_NONE = 300,
		RPL_USERHOST = 302,
		RPL_ISON = 303,
		RPL_AWAY = 301,
		RPL_UNAWAY = 305,
		RPL_NOWAWAY = 306,
		RPL_WHOISUSER = 311,
		RPL_WHOISSERVER = 312,
		RPL_WHOISOPERATOR = 313,
		RPL_WHOISIDLE = 317,
		RPL_WHOISIDENTIFIED = 330,
		RPL_ENDOFWHOIS = 318,
		RPL_WHOISCHANNELS = 319,
		RPL_WHOWASUSER = 314,
		RPL_ENDOFWHOWAS = 369,
		RPL_LISTSTART = 321,
		RPL_LIST = 322,
		RPL_LISTEND = 323,
		RPL_CHANNELMODEIS = 324,
		RPL_NOTOPIC = 331,
		RPL_TOPIC = 332,
		RPL_INVITING = 341,
		RPL_SUMMONING = 342,
		RPL_VERSION = 351,
		RPL_WHOREPLY = 352,
		RPL_ENDOFWHO = 315,
		RPL_NAMREPLY = 353,
		RPL_ENDOFNAMES = 366,
		RPL_LINKS = 364,
		RPL_ENDOFLINKS = 365,
		RPL_BANLIST = 367,
		RPL_ENDOFBANLIST = 368,
		RPL_INFO = 371,
		RPL_ENDOFINFO = 374,
		RPL_MOTDSTART = 375,
		RPL_MOTD = 372,
		RPL_ENDOFMOTD = 376,
		RPL_YOUREOPER = 381,
		RPL_REHASHING = 382,
		RPL_TIME = 391,
		RPL_USERSSTART = 392,
		RPL_USERS = 393,
		RPL_ENDOFUSERS = 394,
		RPL_NOUSERS = 395,
		RPL_TRACELINK = 200,
		RPL_TRACECONNECTING = 201,
		RPL_TRACEHANDSHAKE = 202,
		RPL_TRACEUNKNOWN = 203,
		RPL_TRACEOPERATOR = 204,
		RPL_TRACEUSER = 205,
		RPL_TRACESERVER = 206,
		RPL_TRACENEWTYPE = 208,
		RPL_TRACELOG = 261,
		RPL_STATSLINKINFO = 211,
		RPL_STATSCOMMANDS = 212,
		RPL_STATSCLINE = 213,
		RPL_STATSNLINE = 214,
		RPL_STATSILINE = 215,
		RPL_STATSKLINE = 216,
		RPL_STATSYLINE = 218,
		RPL_ENDOFSTATS = 219,
		RPL_STATSLLINE = 241,
		RPL_STATSUPTIME = 242,
		RPL_STATSOLINE = 243,
		RPL_STATSHLINE = 244,
		RPL_UMODEIS = 221,
		RPL_LUSERCLIENT = 251,
		RPL_LUSEROP = 252,
		RPL_LUSERUNKNOWN = 253,
		RPL_LUSERCHANNELS = 254,
		RPL_LUSERME = 255,
		RPL_ADMINME = 256,
		RPL_ADMINLOC1 = 257,
		RPL_ADMINLOC2 = 258,
		RPL_ADMINEMAIL = 259
	};

	enum AccountStatus
	{
		NotRegistered = 0,
		Registered = 1,
		Recognized = 2,
		Identified = 3,
		AccountStatusError = 4
	};

	enum MessageScopes
	{
		AllScopes =	 	0x000F,
		NoScope = 		0x0000,
		PrivateScope =	 	0x0001,
		ChannelScope =	 	0x0002,
		BotChannelScope = 	0x0004,
		RegExpFilter = 		0x0010,
		IgnoreFakeMessages = 	0x0020
	};
	Q_DECLARE_FLAGS(MessageScope, MessageScopes);

	enum ChannelAccess
	{
		Founder,
		Administrator,
		Operator,
		HalfOp,
		Voice,
		Member
	};

	struct WhoisData
	{
		QxtIrcNick nick;
		QString realName;
		QHash<QString, ChannelAccess> channelList;
		QString serverHost;
		QString serverName;
		QString userName;
		bool isLoggedIn;
		QTime idle;
		QDateTime signOnTime;
		QString operStatus;
		bool isOper;
	};

	QxtIrcConnection(QObject *parent = 0);
	~QxtIrcConnection();
	bool connectToHost(QString hostname, int port, bool ssl);
	void disconnectFromHost();
	void setNickName(QString nick);
	void setRealName(QString real);
	State getState();
	void sendAction(QString to, QString message);
	void sendMessage(QString to, QString message);
	void sendNotice(QString to, QString message);
	bool waitForMessage(QxtIrcMessage &msg, int t = 30000);
	void writeMessage(QxtIrcMessage msg);
	void writeMessageTo(QString to, QString cmd, QStringList args = QStringList());
	void writeMessageTo(QString to, QString cmd, QString arg);
	void writeMessage(QString cmd, QStringList args = QStringList());
	void writeMessage(QString cmd, QString arg);
	void writeRawMessage(QString msg);
	void joinChannel(QString channel);
	void leaveChannel(QString channel, QString reason);
	void quit(QString message);
	void identify(QString password);
	QString nick();
	QStringList listChannels();
	QxtIrcChannel* getChannel(QString chan);
	AccountStatus getAccountStatus(QxtIrcNick, int t = 30000);
	AccountStatus getAccountStatus(QString, int t = 30000);
	WhoisData getWhois(QxtIrcNick, int t = 30000);
	WhoisData getWhois(QString, int t = 30000);
	void addFilter(MessageScope scope, QString filter, QObject *obj, QString slot);
	void setTimeout(int secs);
	QStringList listServerConfig();
	QVariant getServerConfig(QString name, QVariant def);

public slots:
	void connected();
	void registered();
	void messageReceived();
	void disconnected();

protected slots:
	void parseMessage(QString);
	void parseMessageAndReadMore(QString);
	void timedout();

signals:
	void stateChanged(QxtIrcConnection::State state);
	void messageReceived(QxtIrcMessage nsg);
	void userJoined(QString channel, QxtIrcNick nick);
	void userParted(QString channel, QxtIrcNick nick, QString reason);
	void userKicked(QString channel, QxtIrcNick nick, QString reason, QxtIrcNick from);
	void userKilled(QxtIrcNick nick, QString reason, QxtIrcNick from);
	void userQuit(QxtIrcNick nick, QString reason);
	void joinedChannel(QString name);
	void partedChannel(QString channel,QString reason);
	void kicked(QString channel, QString reason, QxtIrcNick from);
	void killed(QString reason, QxtIrcNick from);
	void nickChanged(QxtIrcNick oldnick, QxtIrcNick newnick);
	void nickChanged(QxtIrcNick newnick);
	void queuedParseMessage(QString);
	void nickInUse();
	void timeout(int secs);
	
			
private:
	QXT_DECLARE_PRIVATE(QxtIrcConnection);
	
// 	void setState(State);
// 	void testFilter(QxtIrcMessage msg);
// 	MessageScope getScope(QxtIrcMessage msg);
// 	QString _nickname;
// 	QString _realname;
//         QTcpSocket* _socket;
// 	State _state;
// 	QHash<QString, QPointer<QxtIrcChannel> > _channels;
// 	QString _password;
// 	QList<Filter> _filters;
// 	QHash<QChar, ChannelAccess> _accessPrefix;
// 	QHash<QString, QVariant> _serverConfig;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QxtIrcConnection::MessageScope)

#endif

