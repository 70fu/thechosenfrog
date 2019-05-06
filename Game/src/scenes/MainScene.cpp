
#include "../Game.h"
#include "IScene.h"
#include "../RuntimeClasses.h"
#include "../util/GLMPXConversion.h"
#include <ObjectInterfacePerModule.h>
#include <characterkinematic/PxControllerManager.h>
#include <PxPhysicsAPI.h>
#include <PxRigidActor.h>
#include <PxPhysics.h>


class MainScene : public TInterface<RuntimeClassIds::MAIN_SCENE,IScene>
{
private:
    struct SignParameters
    {
        glm::vec3 translation;
        glm::vec3 rotation;
        std::string text;
    };

    TransformComponent& makeSign(Game& game, SignParameters params)
    {
        EntityId id = game.createEntity();
        TransformComponent& trans = game.transformComps.addComp(id);
        trans.setTranslation(params.translation);
        trans.setRotation(params.rotation);
        trans.setScaling({1,1,1});

        game.meshComps.addComp(id).mesh=game.getAssetManager().getMesh(MeshIds::SIGNPOST);

        MaterialComponent& material = game.materialComps.addComp(id);
        material.material = game.getAssetManager().getMaterial(MaterialIds::UNLIT);
        material.instanceProp.setColor("colorMultiply",{200,160,70,255});
        material.retrieveUniformLocations();

        //create world space text
        EntityId textId = game.createEntity();
        TransformComponent& textTrans = game.transformComps.addComp(textId);
        textTrans.setTranslation({-0.45,1.1,-0.25});
        static constexpr float TEXT_SCALE = 1/768.0f;
        textTrans.setScaling({TEXT_SCALE,TEXT_SCALE,TEXT_SCALE});
        textTrans.setParent(trans,false);

        TextComponent& text = game.textComps.addComp(textId);
        text.text = params.text;
        text.font = game.getAssetManager().getBitmapFont(BitmapFontIds::DEFAULT);
        text.inScreenspace = false;
        text.wrapWidth = 768;
        text.color = {0,0,0,255};

        return trans;
    }

public:
    void init(Game& game) override
    {
        //set skybox
        game.activeSkybox = game.getAssetManager().getCubeMap(CubeMapIds::DEFAULT);

        //TODO do some prefab system, construction methods, object file description system, ... its starting to get messy

        //init player
        {
            //init camera
            EntityId camId = game.createEntity();

            TransformComponent& camTrans = game.transformComps.addComp(camId);
            camTrans.setTranslation({0,0.5f,0});

            CameraComponent& camera = game.cameraComps.addComp(camId);
            camera.setFar(100);
            camera.setNear(0.001f);
            camera.setFov(static_cast<float>(60*TO_RADIANS));

            CameraControllerComponent& camController = game.cameraControllerComps.addComp(camId);
            camController.controlFlags=CameraControllerComponent::ROTATE_VER;

            //init player object
            EntityId playerId = game.createEntity();
            TransformComponent& pTrans = game.transformComps.addComp(playerId);
            pTrans.setTranslation({0,5,8});
            game.playerComps.addComp(playerId);

            //create character controller
            CharControllerComponent& controller = game.charControllerComps.addComp(playerId);
            controller.minJumpHeight=0.5f;
            controller.maxJumpHeight=10;
            controller.minJumpDistance=2;
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
            material.instanceProp.setTexture("diffuseTex",game.getAssetManager().getTexture(TextureIds::DEFAULT));
            material.retrieveUniformLocations();

            PhysicsComponent& pComp = game.physicsComps.addComp(id);
            physx::PxRigidActor* actor = physx::PxCreatePlane(*game.getPhysics().getPhysics(),physx::PxPlane(0,1,0,1),*game.getPhysics().getNullMaterial());
            game.getPhysics().getScene()->addActor(*actor);
            pComp.setActor(actor);
            pComp.setLayerAndCollisionMask(PhysicsComponent::PLATFORM,PhysicsComponent::ALL);
        }

        //make some platforms
        {
            static constexpr unsigned int PLATFORM_COUNT = 5;
            for (int i = 0; i < PLATFORM_COUNT; ++i)
            {
                float size = 8-i;

                EntityId id = game.createEntity();
                TransformComponent &transform = game.transformComps.addComp(id);
                transform.setTranslation({((i%2)*2-1)*8*(i&1), 7 * (i + 1), -(i + 1) * 15});
                transform.setScaling({size, 0.1f, size});

                game.meshComps.addComp(id).mesh = game.getAssetManager().getMesh(MeshIds::UNIT_CUBE);

                MaterialComponent &material = game.materialComps.addComp(id);
                material.material = game.getAssetManager().getMaterial(MaterialIds::UNLIT);
                material.instanceProp.setVec2("texRepeat",{size/2,size/2});
                material.instanceProp.setTexture("diffuseTex",game.getAssetManager().getTexture(TextureIds::DEFAULT));
                material.retrieveUniformLocations();

                PhysicsComponent &pComp = game.physicsComps.addComp(id);
                physx::PxRigidActor *actor = physx::PxCreateStatic(*game.getPhysics().getPhysics(),physx::PxTransform(GLMPXConversion::glmToPx(transform.getTranslation())),physx::PxBoxGeometry(size/2,0.05,size/2),*game.getPhysics().getNullMaterial());
                game.getPhysics().getScene()->addActor(*actor);
                pComp.setActor(actor);
                pComp.setLayerAndCollisionMask(PhysicsComponent::PLATFORM, PhysicsComponent::ALL);

                //place winning sign on last platform
                if(i==PLATFORM_COUNT-1)
                {
                    TransformComponent& signTrans = makeSign(game,{{0,0,0},{0,0,0},"Wow you did it!\n\nYOU WIN!"});
                    signTrans.setParent(transform,true);
                    signTrans.setTranslation({-0.25,0.01,-0.25});
                    signTrans.setRotation({0,45*TO_RADIANS,0});
                }
            }
        }

        //make a screenspace text
        {
            /*EntityId id = game.createEntity();
            TransformComponent &transform = game.transformComps.addComp(id);
            transform.setTranslation({400,400,0});

            TextComponent& text = game.textComps.addComp(id);
            text.text="Hello Frog";
            text.inScreenspace = true;
            text.font = game.getAssetManager().getBitmapFont(BitmapFontIds::DEFAULT);*/
        }

        makeSign(game,{{-1.5,-1,4},{0,45*TO_RADIANS,0},"TEST COURSE\n\n Make it to the highest platform!"});
        makeSign(game,{{1.5,-1,4},{0,-45*TO_RADIANS,0},"Move with WASD\n\n Look around with Mouse"});
        makeSign(game,{{1,-1,-3},{0,-30*TO_RADIANS,0},"Press SPACE to charge a jump.\n\n Release to jump"});
    }
};
REGISTERCLASS(MainScene)