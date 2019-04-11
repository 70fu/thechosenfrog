
#ifndef CGUE19_THECHOSENFROG_IGAMEUPDATER_H
#define CGUE19_THECHOSENFROG_IGAMEUPDATER_H

#include <IObject.h>

class Game;
/**
 * Responsible for drawing all debug gui
 */
class IGameUpdater : public IObject
{
public:
    virtual void update(Game& game)=0;
};

#endif //CGUE19_THECHOSENFROG_IGAMEUPDATER_H
