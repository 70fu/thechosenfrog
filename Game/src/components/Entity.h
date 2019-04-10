
#ifndef CGUE19_THECHOSENFROG_ENTITY_H
#define CGUE19_THECHOSENFROG_ENTITY_H

#include "Components.h"

using EntityId=short;
using ComponentId=EntityId;//works because there should not be more components of a type than entities

static constexpr EntityId INVALID_ENTITY_ID = -1;
static constexpr ComponentId INVALID_COMPONENT_ID = -1;

class Game;
class Entity
{
    friend class Game;
private:
    /**
     * indices to the component arrays in the component stores (e.g if components[Components::Types::Transform] contains 4, then the transform comp of the entity is at the 4th position of the transform component store)
     * if entity does not have component of type T then components[T] is undefined
     */
    ComponentId components[Components::COMPONENT_COUNT]={INVALID_COMPONENT_ID};
    /**
     * if entity contains component of type T, then the corresponding bit is set in this mask (see Components::Mask)
     * set to 0 if it gets deleted
     */
    Components::ComponentMask componentMask=0;
    /**
     * Whether or not this entity is active
     */
    bool deleted = true;
};

#endif //CGUE19_THECHOSENFROG_ENTITY_H
