#include "customtypes.h"
#include <QDebug>
#include <QFile>
#include <OgreQuaternion.h>
#include <OgreVector3.h>

#define STRING_LENGTH_SIZE 16
#define POINT3F_BITCOUNT 16
namespace Types
{

void read(TNL::BitStream &s, QString *str)
{
    *str=QString();
    qDebug() << "Reading String";
    static char buff[256]; // room for the NULL terminator
    TNL::U32 size = s.readInt(STRING_LENGTH_SIZE);
    str->reserve(size);
    qDebug() << "size is" << size;
    TNL::U32 chunks = size/255;
    qDebug() << "Full chunks are" << chunks;
    for(TNL::U32 i = 0; i < chunks; i++)
    {
        s.readString(buff);
        qDebug() << "Read this:" << buff;
        str->insert((i*255), QString::fromAscii(buff, 255));
    }
    s.readString(buff);
    str->insert((chunks*255), QString::fromAscii(buff, (size%255)));
}

void write(TNL::BitStream &s, QString str)
{
	qDebug() << "Writing String";
    s.writeInt(str.size(), STRING_LENGTH_SIZE);
    TNL::U32 chunks = str.size()/255;
    const char* data = str.toAscii().data();
    for(TNL::U32 i = 0; i < chunks; i++)
    {
        const char* buff = data + (i * 255);
        s.writeString(buff, 255);
    }
    const char* buff = data + (chunks * 255);
    s.writeString(buff);
}

void read(TNL::BitStream &s, QStringList *val)
{
    *val = QStringList();
    TNL::U32 size = s.readInt(8);
    qDebug() << "Reading String List:" << size;
    for(TNL::U32 i = 0; i < size; i++)
    {
        QString tmp;
        read(s, &tmp);
        (*val) << tmp;
    }
}

void write(TNL::BitStream &s, QStringList val)
{
    qDebug() << "writing String List:" << val.size();
    s.writeInt(val.size(), 8);
    for(QStringList::const_iterator i = val.constBegin(); i != val.constEnd(); ++i)
        write(s, *i);
}

void read(TNL::BitStream &s, std::string *str)
{
    qDebug() << "Reading Std:String";
    static char buff[256]; // room for the NULL terminator
    TNL::U32 size = s.readInt(STRING_LENGTH_SIZE);
    str->reserve(size);
    TNL::U32 chunks = size/255;
    qDebug() << "Full chunks are" << chunks;
    for(TNL::U32 i = 0; i < chunks; i++)
    {
        s.readString(buff);
        qDebug() << "Read this:" << buff;
        str->insert((i*255), buff, 255);
    }
    s.readString(buff);
    str->insert((chunks*255), buff, (size%255));
}

void write(TNL::BitStream &s, std::string& str)
{
	qDebug() << "Writing Std:String";
    s.writeInt(str.size(), STRING_LENGTH_SIZE);
    TNL::U32 chunks = str.size()/255;
    const char* data = str.data();
    for(TNL::U32 i = 0; i < chunks; i++)
    {
        const char* buff = data + (i * 255);
        s.writeString(buff, 255);
    }
    const char* buff = data + (chunks * 255);
    s.writeString(buff, (str.size()%255));
}

void read(TNL::BitStream &s, Ogre::Vector3 *vect)
{
    //s.readNormalVector((TNL::Point3F*)vect->ptr(), POINT3F_BITCOUNT);
    
    /*
    vect->x = s.readFloat(sizeof(float)*8);
    vect->y = s.readFloat(sizeof(float)*8);
    vect->z = s.readFloat(sizeof(float)*8);
    */
	
	s.readBits(sizeof(float)*3*8,vect->ptr());
}

void write(TNL::BitStream &s, Ogre::Vector3& vect)
{
    //s.writeNormalVector(*((TNL::Point3F*)vect.ptr()), POINT3F_BITCOUNT);
	
     /*
	s.writeFloat(vect.x,sizeof(float)*8);
	s.writeFloat(vect.y,sizeof(float)*8);
	s.writeFloat(vect.z,sizeof(float)*8);
    */
	
	s.writeBits(sizeof(float)*3*8,vect.ptr());
}

void read(TNL::BitStream &s, Ogre::Quaternion *quat)
{
    s.readNormalVector((TNL::Point3F*)quat->ptr(), POINT3F_BITCOUNT);
    *(quat->ptr()+3) = s.readFloat(POINT3F_BITCOUNT);
}

void write(TNL::BitStream &s, Ogre::Quaternion& quat)
{
    s.writeNormalVector(*((TNL::Point3F*)quat.ptr()), POINT3F_BITCOUNT);
    s.writeFloat(*(quat.ptr()+3), POINT3F_BITCOUNT);
}

};
