#ifndef REPLICATEDOBJECT_H
#define REPLICATEDOBJECT_H

#include "netobjectbase.h"
#include "testconnection.h"

class ReplicatedObject : public NetObjectBase
{
public:
DECLARE_NETVARIABLES(ReplicatedObject, NetObjectBase, Initialize, Value)
    ReplicatedObject();
    // called on the client, we use this to get a pointer to the connection object

    QString value();
    void setValue(QString value);

protected:
    QString _value;
};


#endif //#ifndef REPLICATEDOBJECT_H
