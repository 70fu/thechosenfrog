

#include <ObjectInterfacePerModule.h>
#include "Game.h"
#include "RuntimeClasses.h"
#include "IGameRenderer.h"

class GameRenderer : public TInterface<RuntimeClassIds ::GAME_RENDERER,IGameRenderer>
{
public:
    void render(Game* game) override
    {
        //TODO
    }
};
REGISTERCLASS(GameRenderer)
