#ifndef __PLAYER_H
#define __PLAYER_H

#include "netobjectbase.h"
#include "playercamera.h"

class Player : public NetObjectBase
{
DECLARE_NETVARIABLES(Player, NetObjectBase, PlayerId)
public:
    Player();
    ~Player();
    quint32 id();
    void setId(quint32 id);
    bool isMe();
    static quint32 myId();
    static Player *getPlayerById(quint32 id);
    virtual bool onGhostAdd(TNL::GhostConnection *theConnection);
    virtual void performScopeQuery(TNL::GhostConnection *connection);
    PlayerCamera *camera();

protected:
    quint32 _id;
    PlayerCamera *_camera;
};


#endif //#ifndef __PLAYER_H

