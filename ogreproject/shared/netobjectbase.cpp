#include "netobjectbase.h"
#include "connection.h"

NetObjectBase::NetObjectBase()
{
    mNetFlags.set(Ghostable);
    #ifdef SERVER_SIDE
    Connection::registerNetObject(this);
    #endif
}

NetObjectBase::~NetObjectBase()
{
    Connection::unregisterNetObject(this);
}

TNL_IMPLEMENT_NETOBJECT(NetObjectBase);
int NetObjectBase::countNetVariables()
{
    return TotalNetVariables;
}

TNL::U32 NetObjectBase::packUpdate(TNL::GhostConnection *connection, TNL::U32 updateMask, TNL::BitStream *stream)
{
}

void NetObjectBase::unpackUpdate(TNL::GhostConnection *connection, TNL::BitStream *stream)
{
}

bool NetObjectBase::onGhostAdd(TNL::GhostConnection *theConnection)
{
    if(!Parent::onGhostAdd(theConnection))
        return false; //A parent had an error
    Connection::registerNetObject(this);
    return true;
}
