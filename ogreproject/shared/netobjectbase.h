#ifndef NETOBJECTBASE_H_
#define NETOBJECTBASE_H_

#include "customtypes.h"
#include "tnl.h"
#include "tnlNetObject.h"
#include <QDebug>

//No network variables
#define DECLARE_NETVARIABLES(class, parent, ...) \
typedef parent Parent; \
TNL_DECLARE_CLASS(class); \
enum NetValues { __VA_ARGS__ , TotalNetVariables } ; \
public: \
virtual int countNetVariables() \
{ \
    if(strcmp(#__VA_ARGS__, "NONE") == 0) \
        return Parent::countNetVariables(); \
    return TotalNetVariables + Parent::countNetVariables(); \
} \
void setMaskBits(NetValues mask) \
{ \
    TNL::NetObject::setMaskBits((1 << (mask + Parent::countNetVariables())));\
}\
TNL::U32 packUpdate(TNL::GhostConnection *connection, TNL::U32 updateMask, TNL::BitStream *stream); \
void unpackUpdate(TNL::GhostConnection *connection, TNL::BitStream *stream);

#ifdef CLIENT_SIDE
#define START_IMPLEMENT_NETVARIABLES(class) \
TNL_IMPLEMENT_NETOBJECT(class); \
TNL::U32 class::packUpdate(TNL::GhostConnection *connection, TNL::U32 updateMask, TNL::BitStream *stream) \
{ \
    qFatal("unpack called on client side"); \
} \
void class::unpackUpdate(TNL::GhostConnection *connection, TNL::BitStream *stream) \
{

#define IMPLEMENT_NETVARIABLE(name, ...) \
    if(stream->readFlag()) \
    { \
        readNetVariable(stream, ## __VA_ARGS__);\
    }

#define IMPLEMENT_NETVARIABLE_CALLBACK(name, callback, ...) \
    if(stream->readFlag()) \
    { \
        readNetVariable(stream, ## __VA_ARGS__);\
        callback (); \
    }

#define END_IMPLEMENT_NETVARIABLES \
}
#endif

#ifdef SERVER_SIDE
#define START_IMPLEMENT_NETVARIABLES(class) \
TNL_IMPLEMENT_NETOBJECT(class); \
void class::unpackUpdate(TNL::GhostConnection *connection, TNL::BitStream *stream) \
{ \
    return 0; \
} \
TNL::U32 class::packUpdate(TNL::GhostConnection *connection, TNL::U32 updateMask, TNL::BitStream *stream) \
{ \
int totalVars = Parent::countNetVariables();

#define IMPLEMENT_NETVARIABLE(name,  ...) \
    if(stream->writeFlag(updateMask & (1 << (name + totalVars)))) \
    { \
        writeNetVariable(stream, ## __VA_ARGS__);\
    }

#define IMPLEMENT_NETVARIABLE_CALLBACK(name, callback, ...) \
    if(stream->writeFlag(updateMask & (1 << (name + totalVars)))) \
    { \
        writeNetVariable(stream, ## __VA_ARGS__);\
        callback (); \
    }

#define END_IMPLEMENT_NETVARIABLES \
    return 0; \
}
#endif


class NetObjectBase : public TNL::NetObject
{
  //  /*

typedef TNL::NetObject Parent;
TNL_DECLARE_CLASS(NetObjectBase);
enum NetValues { TotalNetVariables } ;
public:
NetObjectBase();
~NetObjectBase();
virtual int countNetVariables();
TNL::U32 packUpdate(TNL::GhostConnection *connection, TNL::U32 updateMask, TNL::BitStream *stream);
void unpackUpdate(TNL::GhostConnection *connection, TNL::BitStream *stream);
virtual bool onGhostAdd(TNL::GhostConnection *theConnection);
    /*/
public:
DECLARE_NETVARIABLES(NetObjectBase, NetObjectBase, NONE)
//*/

protected:

void writeNetVariable(TNL::BitStream *stream) { }

template <typename a>
void writeNetVariable(TNL::BitStream *stream, a var1)
{
    Types::write(*stream, var1);
}

template <typename a, typename b>
void writeNetVariable(TNL::BitStream *stream, a var1, b var2)
{
    Types::write(*stream, var1);
    Types::write(*stream, var2);
}

template <typename a, typename b, typename c>
void writeNetVariable(TNL::BitStream *stream, a var1, b var2, c var3)
{
    Types::write(*stream, var1);
    Types::write(*stream, var2);
    Types::write(*stream, var3);
}

template <typename a, typename b, typename c, typename d>
void writeNetVariable(TNL::BitStream *stream, a var1, b var2, c var3, d var4)
{
    Types::write(*stream, var1);
    Types::write(*stream, var2);
    Types::write(*stream, var3);
    Types::write(*stream, var4);
}

void readNetVariable(TNL::BitStream *stream) { };

template <typename a>
void readNetVariable(TNL::BitStream *stream, a &var1)
{
    Types::read(*stream, &var1);
    //qDebug() << "Got Network Variable:" << var1;
}

template <typename a, typename b>
void readNetVariable(TNL::BitStream *stream, a &var1, b &var2)
{
    Types::read(*stream, &var1);
    Types::read(*stream, &var2);
    //qDebug() << "Got Network Variables:" << var1 << var2;
}

template <typename a, typename b, typename c>
void readNetVariable(TNL::BitStream *stream, a &var1, b &var2, c &var3)
{
    Types::read(*stream, &var1);
    Types::read(*stream, &var2);
    Types::read(*stream, &var3);
    qDebug() << "Got Network Variables:" << var1 << var2 << var3;
}

template <typename a, typename b, typename c, typename d>
void readNetVariable(TNL::BitStream *stream, a &var1, b &var2, c &var3, d &var4)
{
    Types::read(*stream, &var1);
    Types::read(*stream, &var2);
    Types::read(*stream, &var3);
    Types::read(*stream, &var4);
    qDebug() << "Got Network Variables:" << var1 << var2 << var3 << var4;
}

};

#endif //#ifndef NETOBJECTBASE_H_
