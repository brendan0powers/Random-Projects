#include "connection.h"
#include "player.h"

using namespace TNL;

QList<NetObjectBase*> Connection::_netobjects;
int Connection::_id = 0;
TNL::SafePtr<Connection> Connection::_conn;
QHash<quint32, TNL::SafePtr<Player> > Connection::_players;

TNL_IMPLEMENT_NETCONNECTION(Connection, NetClassGroupGame, true);

Connection* Connection::instance()
{
    return _conn.getPointer();
}

TNL_IMPLEMENT_RPC(Connection, setId, (quint32 id), (id),
NetClassGroupGameMask, RPCGuaranteedOrdered, RPCDirServerToClient, 0)
{
    qDebug() << "====== Got ID:" << id;
    _id = id;
}

TNL_IMPLEMENT_RPC(Connection, updateCamera, (Ogre::Vector3 position, Ogre::Quaternion orientation),(position,orientation),NetClassGroupGameMask, RPCUnguaranteed, RPCDirClientToServer,0)
{
	_player->camera()->update(position,orientation);
}

void Connection::onConnectionEstablished()
{
    // call the parent's onConnectionEstablished.
    // by default this will set the initiator to be a connection
    // to "server" and the non-initiator to be a connection to "client"
    Parent::onConnectionEstablished();

    if(isInitiator())
    {
        _conn = this;
        setGhostFrom(false);
        setGhostTo(true);
    }
    else
    {
        // on the server, we create a replicated object that will be the scope object
        // for this client.
	_id++;
	setId(_id);
        _player = new Player;
        _player->setId(_id);
        _players[_id] = _player;
        setScopeObject(_player);
        setGhostFrom(true);
        setGhostTo(false);
       	activateGhosting();
   }
}

quint32 Connection::playerId()
{
    #ifdef SERVER_SIDE
    if(_player.getPointer() == NULL)
        return 0;
    return _player->id();
    #else
    return _id;
    #endif
}

Player* Connection::getPlayerById(quint32 id)
{
    if(!_players.contains(id))
        return NULL;
    return _players[id].getPointer();
}

void Connection::registerPlayer(Player* player, quint32 id)
{
    _players[id] = player;
}

void Connection::unregisterPlayer(quint32 id)
{
    _players.remove(id);
}

void Connection::registerNetObject(NetObjectBase* object)
{
    _netobjects << object;
}

void Connection::unregisterNetObject(NetObjectBase* object)
{
    _netobjects.removeAll(object);
}

QList<NetObjectBase*> Connection::getGhostList()
{
    return _netobjects;
}




