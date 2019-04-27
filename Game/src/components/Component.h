#ifndef CGUE19_THECHOSENFROG_COMPONENT_H
#define CGUE19_THECHOSENFROG_COMPONENT_H


#include "Entity.h"


class Game;

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

    /**
     * Do cleanup tasks and reset component, called by the component store
     * @param game
     */
    virtual void cleanup(Game& game){};
};


#endif //CGUE19_THECHOSENFROG_COMPONENT_H
