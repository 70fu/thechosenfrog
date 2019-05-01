
#include "../Game.h"
#include "IScene.h"
#include "../RuntimeClasses.h"
#include "../util/GLMPXConversion.h"
#include <ObjectInterfacePerModule.h>
#include <characterkinematic/PxControllerManager.h>
#include <PxPhysicsAPI.h>
#include <PxRigidActor.h>


class MainScene : public TInterface<RuntimeClassIds::MAIN_SCENE,IScene>
{
public:
    void init(Game& game) override
    {
        //set skybox
        game.activeSkybox = game.getAssetManager().getCubeMap(CubeMapIds::DEFAULT);

        //init player
        {
            //init camera
            EntityId camId = game.createEntity();

            TransformComponent& camTrans = game.transformComps.addComp(camId);
            camTrans.setTranslation({0,0.5f,0});

            CameraComponent& camera = game.cameraComps.addComp(camId);
            camera.setFar(100);
            camera.setNear(0.1f);
            camera.setFov(static_cast<float>(60*TO_RADIANS));

            CameraControllerComponent& camController = game.cameraControllerComps.addComp(camId);
            camController.controlFlags=CameraControllerComponent::ROTATE_VER;

            //init player object
            EntityId playerId = game.createEntity();
            TransformComponent& pTrans = game.transformComps.addComp(playerId);
            pTrans.setTranslation({0,5,0});
            game.playerComps.addComp(playerId);

            //create character controller
            CharControllerComponent& controller = game.charControllerComps.addComp(playerId);
            controller.minJumpHeight=0.5f;
            controller.maxJumpHeight=10;
            controller.minJumpDistance=1;
            controller.maxJumpDistance=20;
            controller.minJumpDuration=1;
            controller.maxJumpDuration=3;
            controller.airWalkSpeed = 1;
            controller.groundWalkForce = 15;
            controller.maxGroundSpeed = 3;

            physx::PxCapsuleControllerDesc cDesc;
            //player is 1.5 meters tall
            cDesc.radius=0.25f;
            cDesc.height=1;
            cDesc.position = GLMPXConversion::glmToPxExtVec(pTrans.getTranslation());
            cDesc.maxJumpHeight = controller.maxJumpHeight;
            cDesc.contactOffset=0.01f;
            cDesc.reportCallback = game.getEventManager();
            cDesc.material = game.getPhysics().getNullMaterial();
            //cDesc.isValid();
            controller.setController(game.getPhysics().getControllerManager()->createController(cDesc));

            //add camera controller to player
            CameraControllerComponent& playerCam = game.cameraControllerComps.addComp(playerId);
            playerCam.controlFlags = CameraControllerComponent::ROTATE_HOR;

            //set camera as child of player
            camTrans.setParent(pTrans,false);
        }

        //init default object
        {
            EntityId id = game.createEntity();
            game.transformComps.addComp(id).setTranslation({0,0,-5});
            game.meshComps.addComp(id).mesh=game.getAssetManager().getMesh(MeshIds::DEFAULT);
            game.materialComps.addComp(id).material = game.getAssetManager().getMaterial(MaterialIds::DEFAULT);
        }

        //make a textured cube
        {
            EntityId id = game.createEntity();
            TransformComponent& transform = game.transformComps.addComp(id);
            transform.setTranslationAndRotation({2,0,-5},{30*TO_RADIANS,30*TO_RADIANS,30*TO_RADIANS});
            transform.setScaling({1,1.5f,0.5f});

            game.meshComps.addComp(id).mesh=game.getAssetManager().getMesh(MeshIds::UNIT_CUBE);
            game.materialComps.addComp(id).material = game.getAssetManager().getMaterial(MaterialIds::UNLIT);
        }

        //make a plane
        {
            EntityId id = game.createEntity();
            TransformComponent& transform = game.transformComps.addComp(id);
            transform.setTranslation({0,-1,0});
            transform.setScaling({20,0.1f,20});

            game.meshComps.addComp(id).mesh=game.getAssetManager().getMesh(MeshIds::UNIT_CUBE);

            MaterialComponent& material = game.materialComps.addComp(id);
            material.material = game.getAssetManager().getMaterial(MaterialIds::UNLIT);
            material.instanceProp.setVec2("texRepeat",{10,10});
            material.retrieveUniformLocations();

            PhysicsComponent& pComp = game.physicsComps.addComp(id);
            physx::PxRigidActor* actor = physx::PxCreatePlane(*game.getPhysics().getPhysics(),physx::PxPlane(0,1,0,1),*game.getPhysics().getNullMaterial());
            game.getPhysics().getScene()->addActor(*actor);
            pComp.setActor(actor);
            pComp.setLayerAndCollisionMask(PhysicsComponent::PLATFORM,PhysicsComponent::ALL);
        }
    }
};
REGISTERCLASS(MainScene)