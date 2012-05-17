#include "player.h"
#include "connection.h"
#include <QDebug>
//#include "tnlGhostConnection.h"

Player::Player()
{
}

Player::~Player()
{
    Connection::unregisterPlayer(_id);
}

START_IMPLEMENT_NETVARIABLES(Player)
IMPLEMENT_NETVARIABLE(PlayerId, _id)
END_IMPLEMENT_NETVARIABLES

quint32 Player::id()
{
    return _id;
}

void Player::setId(quint32 id)
{
    #ifdef SERVER_SIDE
    _id = id;
    setMaskBits(PlayerId);
    qDebug() << "******* Setting ID:" << id;
    _camera = new PlayerCamera();
    _camera->setOwner(id);
    #else
    qCritical() << "Clients cannot set player ID's";
    #endif
}

bool Player::isMe()
{
    if(Connection::instance() == NULL)
        return false;
    return (Connection::instance()->playerId() == _id);
}

quint32 Player::myId()
{
    return Connection::instance()->playerId();
}

Player* Player::getPlayerById(quint32 id)
{
    return Connection::getPlayerById(id);
}

bool Player::onGhostAdd(TNL::GhostConnection *theConnection)
{
    if(!Parent::onGhostAdd(theConnection))
        return false; //A parent had an error

    Connection* conn = dynamic_cast<Connection*>(theConnection);
    if(conn == NULL)
    {
        qCritical() << "The ghost connection object could not be casted to a Connection object.";
        return false;
    }
    
    qDebug() << "Got Player:" << _id << Connection::instance()->playerId();
    
    conn->registerPlayer(this, _id);
    return true;
}

void Player::performScopeQuery(TNL::GhostConnection *connection)
{
    Connection* conn = dynamic_cast<Connection*>(connection);
    if(conn == NULL)
    {
        qCritical() << "The ghost connection object could not be casted to a Connection object.";
        return;
    }
    QList<NetObjectBase*> list = conn->getGhostList();
    foreach(NetObjectBase* ghost, list)
    {
        conn->objectInScope(ghost);
    }
}

PlayerCamera *Player::camera()
{
	return(_camera);
}
