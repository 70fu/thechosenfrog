
#ifndef CGUE19_THECHOSENFROG_IGAMEUPDATER_H
#define CGUE19_THECHOSENFROG_IGAMEUPDATER_H

#include <IObject.h>

class Game;
/**
 * Responsible for updating all the entities in the game
 */
class IGameUpdater : public IObject
{
public:
    virtual void update(Game& game)=0;
};

#endif //CGUE19_THECHOSENFROG_IGAMEUPDATER_H
