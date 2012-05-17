#ifndef __CONNECTION_H
#define __CONNECTION_H

#include "player.h"
#include <QtGlobal>
#include <QHash>
#include <QList>
#include <tnl.h>
#include <tnlGhostConnection.h>
#include <tnlRPC.h>

class Connection : public TNL::GhostConnection
{
typedef TNL::GhostConnection Parent;
public:
    // Let the network system know this is a valid network connection.
    TNL_DECLARE_NETCONNECTION(Connection);

    void onConnectionEstablished();
    static Connection* instance();

    quint32 playerId();
    static Player* getPlayerById(quint32 id);

    static void registerPlayer(Player* player, quint32 id);
    static void unregisterPlayer(quint32 id);
    QList<NetObjectBase*> getGhostList();


    static void Connection::registerNetObject(NetObjectBase* object);
    static void Connection::unregisterNetObject(NetObjectBase* object);

    TNL_DECLARE_RPC(setId, (quint32 id));
    TNL_DECLARE_RPC(updateCamera, (Ogre::Vector3 position, Ogre::Quaternion orientation));

protected:
    static int _id;
    static TNL::SafePtr<Connection> _conn;
    TNL::SafePtr<Player> _player;
    static QHash< quint32, TNL::SafePtr < Player > > _players;
    static QList<NetObjectBase*> _netobjects;
};


#endif //#ifndef __CONNECTION_H
