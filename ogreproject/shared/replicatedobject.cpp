#include "replicatedobject.h"
#include "customtypes.h"
#include "tnlMethodDispatch.h"
#include "tnlGhostConnection.h"

ReplicatedObject::ReplicatedObject()
{
    _value="NONE";
}

QString ReplicatedObject::value()
{
    qDebug() << "Value Requested";
    return _value;
}

void ReplicatedObject::setValue(QString value)
{
    qDebug() << "Value Set";
    _value = value;
    setMaskBits(Value);
}

START_IMPLEMENT_NETVARIABLES(ReplicatedObject)
IMPLEMENT_NETVARIABLE(Initialize)
IMPLEMENT_NETVARIABLE(Value, _value)
END_IMPLEMENT_NETVARIABLES


