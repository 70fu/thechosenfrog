
#ifndef CGUE19_THECHOSENFROG_ISCENE_H
#define CGUE19_THECHOSENFROG_ISCENE_H

#include <IObject.h>

class Game;
/**
 * Responsible for constructing all objects of a scene
 */
class IScene : public IObject
{
public:
    virtual void init(Game& game)=0;
};

#endif //CGUE19_THECHOSENFROG_ISCENE_H
