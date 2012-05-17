#ifndef CUSTOM_TYPES_OGREPROJECT_H
#define CUSTOM_TYPES_OGREPROJECT_H

#include <string>
#include <vector>
#include <tnl.h>
#include <tnlEventConnection.h>
#include <QList>
#include <QString>
#include <QStringList>

namespace Ogre
{
    class Vector3;
    class Quaternion;
}

namespace Types
{
void read(TNL::BitStream &s, std::string *str);
void write(TNL::BitStream &s, std::string& str);
void read(TNL::BitStream &s, QString *str);
void write(TNL::BitStream &s, QString str);
void read(TNL::BitStream &s, QStringList *str);
void write(TNL::BitStream &s, QStringList str);
void read(TNL::BitStream &s, Ogre::Vector3 *vect);
void write(TNL::BitStream &s, Ogre::Vector3& vect);
void read(TNL::BitStream &s, Ogre::Quaternion *quat);
void write(TNL::BitStream &s, Ogre::Quaternion& quat);

/// Reads a std::vector of objects from a BitStream.
template <typename T> inline void read(TNL::BitStream &s, std::vector<T> *val)
{
    TNL::U32 size = s.readInt(8);
    val->resize(size);
    for(TNL::S32 i = 0; i < size; i++)
        read(s, &((*val)[i]));
}
/// Writes a std::vector of objects into a BitStream.
template <typename T> void write(TNL::BitStream &s, std::vector<T> &val)
{
    s.writeInt(val.size(), 8);
    for(TNL::S32 i = 0; i < val.size(); i++)
        write(s, val[i]);
}

template <typename T> inline void read(TNL::BitStream &s, QList<T> *val)
{
    TNL::U32 size = s.readInt(8);
    val->resize(size);
    for(TNL::S32 i = 0; i < size; i++)
        read(s, &((*val)[i]));
}

template <typename T> void write(TNL::BitStream &s, QList<T> &val)
{
    s.writeInt(val.size(), 8);
    for(typename QList<T>::const_iterator i = val.constBegin(); i != val.constEnd(); ++i)
        write(s, *i);
}

template <typename T, typename V> inline void read(TNL::BitStream &s, QMap<T, V> *val)
{
    TNL::U32 size = s.readInt(8);
    for(TNL::S32 i = 0; i < size; i++)
    {
        T key;
        V value;
        read(s, &key);
        read(s, &value);
        val->insertMulti(key, value);
    }
}

template <typename T, typename V> void write(TNL::BitStream &s, QMap<T, V> &val)
{
    s.writeInt(val.size(), 8);
    for(typename QMap<T, V>::const_iterator i = val.constBegin(); i != val.constEnd(); ++i)
    {
        write(s, i.key());
        write(s, i.value());
    }
}

template <typename T, typename V> inline void read(TNL::BitStream &s, QHash<T, V> *val)
{
    TNL::U32 size = s.readInt(8);
    for(TNL::S32 i = 0; i < size; i++)
    {
        T key;
        V value;
        read(s, &key);
        read(s, &value);
        val->insertMulti(key, value);
    }
}

template <typename T, typename V> void write(TNL::BitStream &s, QHash<T, V> &val)
{
    s.writeInt(val.size(), 8);
    for(typename QHash<T, V>::const_iterator i = val.constBegin(); i != val.constEnd(); ++i)
    {
        write(s, i.key());
        write(s, i.value());
    }
}

};
#endif //#ifndef CUSTOM_TYPES_OGREPROJECT_H
