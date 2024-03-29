
#ifndef CGUE19_THECHOSENFROG_IGAMERENDERER_H
#define CGUE19_THECHOSENFROG_IGAMERENDERER_H

#include <IObject.h>

class Game;
/**
 * Responsible for rendering the scene (all entities, so not including debug gui)
 */
class IGameRenderer : public IObject
{
public:
    virtual void init(Game& game)=0;
    virtual void render(Game& game,int width, int height)=0;
    virtual void cleanup(Game& game)=0;
};
#endif //CGUE19_THECHOSENFROG_IGAMERENDERER_H
