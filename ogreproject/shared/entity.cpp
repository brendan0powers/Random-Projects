#include "entity.h"

using namespace TNL;

Entity::Entity()
{
    _owner = 0;
}

START_IMPLEMENT_NETVARIABLES(Entity);
IMPLEMENT_NETVARIABLE(OwnerId, _owner);
END_IMPLEMENT_NETVARIABLES;

void Entity::init(QHash<QString,QString> &data)
{
}

Entity* Entity::spawn(const char* className, QHash<QString,QString> &data)
{
    Object* obj = create(className);
    Entity* entity = dynamic_cast<Entity*>(obj);
    if(entity == NULL)
    {
        delete obj;
        return NULL;
    }
    entity->init(data);
    return entity;
}

quint32 Entity::owner()
{
    return _owner;
}

void Entity::setOwner(quint32 ownerId)
{
    #ifdef SERVER_SIDE
    _owner = ownerId;
    setMaskBits(OwnerId);
    #endif
}


