#ifndef __ENTITY_H
#define __ENTITY_H

#include "netobjectbase.h"
#include <QStringList>
#include <QHash>
#include <tnl.h>
#include <tnlRPC.h>

class Entity : public NetObjectBase
{
DECLARE_NETVARIABLES(Entity, NetObjectBase, OwnerId);
public:
    Entity();
    virtual void init(QHash<QString,QString> &data);
    static Entity* spawn(const char* className,QHash<QString,QString> &data);
    quint32 owner();
    void setOwner(quint32 ownerId);

protected:
    quint32 _owner;
};


#endif //#ifndef __ENTITY_H
