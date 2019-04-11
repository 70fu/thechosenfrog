

#include <ObjectInterfacePerModule.h>
#include "Game.h"
#include "RuntimeClasses.h"

class GameUpdater : public TInterface<RuntimeClassIds::GAME_UPDATER,IGameUpdater>
{
public:
    void update(Game& game) override
    {
        //TODO
    }
};
REGISTERCLASS(GameUpdater)

