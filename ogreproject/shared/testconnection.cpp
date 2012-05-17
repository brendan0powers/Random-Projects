#include "testconnection.h"
#include "Ogre.h"
#include "replicatedobject.h"
#include <QDebug>
#include "util.h"
#include <vector>
#ifdef CLIENT_SIDE
	#include "application.h"
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "windows.h"
#endif

TNL_IMPLEMENT_NETCONNECTION(TestConnection, NetClassGroupGame, true);

TNL_IMPLEMENT_RPC(TestConnection, sendConsoleCommand, (QStringList cmd), (cmd),
NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirClientToServer, 0)
{
    #ifdef SERVER_SIDE
    qDebug() << "Got a command:" << cmd;
    ConsoleListener::runCommand(cmd);
    #endif
}

void TestConnection::debugString(QString val)
{
    qDebug() << "YOU JUST RAN" << val;
}

void TestConnection::onConnectionEstablished()
{
    // call the parent's onConnectionEstablished.
    // by default this will set the initiator to be a connection
    // to "server" and the non-initiator to be a connection to "client"
    Parent::onConnectionEstablished();

    if(isInitiator())
    {
        qDebug() << "connected to server." << getNetAddressString();
#ifdef CLIENT_SIDE
	app()->connected();
#endif
    }
}

void TestConnection::onConnectionTerminated(TerminationReason, const char *errorDisconnectString)
{
	if(isInitiator())
	{
		qDebug() << "Connection to server closed:" << errorDisconnectString;
#ifdef CLIENT_SIDE
		app()->disconnected();
#endif
	}
	else
	{
		qDebug() << "client connection closed:" << errorDisconnectString;
	}
}

