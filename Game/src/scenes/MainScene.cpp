
#include "../Game.h"
#include "IScene.h"
#include "../RuntimeClasses.h"
#include <ObjectInterfacePerModule.h>


class MainScene : public TInterface<RuntimeClassIds::MAIN_SCENE,IScene>
{
public:
    void init(Game& game) override
    {
        //TODO
    }
};
REGISTERCLASS(MainScene)