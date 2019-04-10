#ifndef CGUE19_THECHOSENFROG_COMPONENT_H
#define CGUE19_THECHOSENFROG_COMPONENT_H


#include "Entity.h"

/**
 * Base class for every component
 */
class Component
{
public:
    /**
     * The entity id of the entity this component belongs to
     */
    EntityId entity;//TODO should be private
};


#endif //CGUE19_THECHOSENFROG_COMPONENT_H
