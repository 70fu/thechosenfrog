
#include "../Game.h"
#include "IScene.h"
#include "../RuntimeClasses.h"
#include <ObjectInterfacePerModule.h>


class MainScene : public TInterface<RuntimeClassIds::MAIN_SCENE,IScene>
{
public:
    void init(Game& game) override
    {
        //init camera
        {
            EntityId id = game.createEntity();

            game.transformComps.addComp(id);

            CameraComponent& camera = game.cameraComps.addComp(id);
            camera.setFar(100);
            camera.setNear(0.1f);
            camera.setFov(static_cast<float>(60*TO_RADIANS));

            CameraControllerComponent& controller = game.cameraControllerComps.addComp(id);
        }

        //init default object
        {
            EntityId id = game.createEntity();
            game.transformComps.addComp(id).setTranslation({0,0,-5});
            game.meshComps.addComp(id).mesh=game.getAssetManager().getMesh(MeshIds::DEFAULT);
            game.materialComps.addComp(id).material = game.getAssetManager().getMaterial(MaterialIds::DEFAULT);
        }
    }
};
REGISTERCLASS(MainScene)