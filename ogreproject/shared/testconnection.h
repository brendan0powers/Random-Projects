#ifndef TESTCONNECTION_H
#define TESTCONNECTION_H

#include "customtypes.h"
#include <QDebug>
#include <Ogre.h>
#include <tnl.h>
#include <tnlGhostConnection.h>
#include <tnlNetInterface.h>
#include <tnlRPC.h>
#include <stdio.h>
#include "connection.h"

class ReplicatedObject;

using namespace TNL;

class TestConnection : public Connection
{
typedef Connection Parent;
public:
    // Let the network system know this is a valid network connection.
    TNL_DECLARE_NETCONNECTION(TestConnection);

    // declare the client to server message for sending arbitrary console commands
    TNL_DECLARE_RPC(sendConsoleCommand, (QStringList cmd));
    void debugString(QString val);
    void onConnectionEstablished();
    void onConnectionTerminated(TerminationReason, const char *errorDisconnectString);
private:
    ReplicatedObject* _obj;
};


#endif //#ifndef TESTCONNECTION_H
