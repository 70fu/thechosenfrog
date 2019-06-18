#ifndef CGUE19_THECHOSENFROG_PLAYERHUDCOMPONENT_H
#define CGUE19_THECHOSENFROG_PLAYERHUDCOMPONENT_H


#include "Component.h"

/**
 * Component for tagging a textComponent that shows jumps and time
 */
class PlayerHUDComponent : public Component
{
public:
    EntityId playerId=INVALID_ENTITY_ID;
};


#endif //CGUE19_THECHOSENFROG_PLAYERHUDCOMPONENT_H
