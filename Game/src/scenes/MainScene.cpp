
#include "../Game.h"
#include "IScene.h"
#include "../RuntimeClasses.h"
#include "../util/GLMPXConversion.h"
#include "../CloudPlatforms.h"
#include "../SignGenerator.h"
#include <ObjectInterfacePerModule.h>
#include <characterkinematic/PxControllerManager.h>
#include <PxPhysicsAPI.h>
#include <PxRigidActor.h>
#include <PxPhysics.h>


class MainScene : public TInterface<RuntimeClassIds::MAIN_SCENE,IScene>
{
private:
    struct PlatformParameters
    {
        glm::vec3 translation={0,0,0};
        float yRotation = 0;
        glm::vec2 size={1,1};//x and z size
    };

    TransformComponent& makePlatform(Game& game, PlatformParameters params)
    {
        EntityId id = game.createEntity();
        TransformComponent &transform = game.transformComps.addComp(id);
        transform.setTranslation(params.translation);
        transform.setScaling({params.size.x, 0.1f, params.size.y});
        transform.setRotation({0,params.yRotation,0});

        game.meshComps.addComp(id).mesh = game.getAssetManager().getMesh(MeshIds::UNIT_CUBE);

        MaterialComponent &material = game.materialComps.addComp(id);
        material.material = game.getAssetManager().getMaterial(MaterialIds::UNLIT);
        material.instanceProp.setVec2("texRepeat",{params.size.x,params.size.y});
        material.instanceProp.setTexture("diffuseTex",game.getAssetManager().getTexture(TextureIds::DEFAULT));
        material.retrieveUniformLocations();

        PhysicsComponent &pComp = game.physicsComps.addComp(id);
        physx::PxRigidActor *actor = physx::PxCreateStatic(*game.getPhysics().getPhysics(),physx::PxTransform(GLMPXConversion::glmToPx(transform.getTranslation())),physx::PxBoxGeometry(params.size.x/2,0.05,params.size.y/2),*game.getPhysics().getNullMaterial());
        game.getPhysics().getScene()->addActor(*actor);
        pComp.setActor(actor);
        pComp.setLayerAndCollisionMask(PhysicsComponent::PLATFORM, PhysicsComponent::ALL);

        return transform;
    }
public:

    void Init(bool isFirstInit) override
    {
        IObject::Init(isFirstInit);
    }

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
            camera.setFov(static_cast<float>(100*TO_RADIANS));
            camera.setFramebuffer(game.getAssetManager().getFramebuffer(FramebufferIds::DEFAULT));

            CameraControllerComponent& camController = game.cameraControllerComps.addComp(camId);
            camController.controlFlags=CameraControllerComponent::ROTATE_VER;

            //init player object
            EntityId playerId = game.createEntity();
            TransformComponent& pTrans = game.transformComps.addComp(playerId);
            pTrans.setTranslation({0,5,5});
            game.playerComps.addComp(playerId);

            //create character controller
            CharControllerComponent& controller = game.charControllerComps.addComp(playerId);
            controller.config=PLAYER_CONFIG;

            physx::PxCapsuleControllerDesc cDesc;
            //player is 1.5 meters tall
            cDesc.radius=0.25f;
            cDesc.height=1;
            cDesc.position = GLMPXConversion::glmToPxExtVec(pTrans.getTranslation());
            cDesc.maxJumpHeight = PLAYER_CONFIG.maxJumpHeight;
            cDesc.contactOffset=0.01f;
            cDesc.reportCallback = &game;
            cDesc.material = game.getPhysics().getNullMaterial();
            //cDesc.isValid();
            controller.setController(game.getPhysics().getControllerManager()->createController(cDesc));

            //add camera controller to player
            CameraControllerComponent& playerCam = game.cameraControllerComps.addComp(playerId);
            playerCam.controlFlags = CameraControllerComponent::ROTATE_HOR;

            //set camera as child of player
            camTrans.setParent(pTrans,false);

            //Create player hud
            EntityId hudId = game.createEntity();
            TransformComponent &textTrans = game.transformComps.addComp(hudId);
            textTrans.setScaling({0.6f,0.6f,1});
            //textTrans.setTranslation({game.settings.display.windowWidth/2-200,game.settings.display.windowHeight/2,0});
            textTrans.setTranslation({20,60,0});

            TextComponent& text = game.textComps.addComp(hudId);
            text.inScreenspace = true;
            text.font = game.getAssetManager().getBitmapFont(BitmapFontIds::DEFAULT);
            text.color = {0,0,0,255};

            PlayerHUDComponent& hud = game.playerHUDComps.addComp(hudId);
            hud.playerId=playerId;
        }

        //make some platforms
        /*{
            static constexpr unsigned int PLATFORM_COUNT = 5;
            for (int i = 0; i < PLATFORM_COUNT; ++i)
            {
                float size = 5-i;

                CloudPlatformParameter params;
                params.translation={((i%2)*2-1)*8*(i&1), 7 * (i + 1), -(i + 1) * 15};
                params.size={size,size};
                params.scale={1,0.8f,1};
                params.type=CloudType::SQUARE;

                TransformComponent &transform = cloudPlatforms.makeCloudPlatform(game, params);

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

        {
            CloudPlatformParameter params;
            params.translation={15,20,-20};
            params.scale={1.5f,1,1.5f};
            params.color={100,150,30,255};
            params.type = CloudType::CIRCLE;

            cloudPlatforms.makeCloudPlatform(game, params);
        }*/

        /*{
            for(int x = 0;x<14;++x)
            {
                for(int y = 0;y<14;++y)
                {
                    EntityId id = game.createEntity();
                    TransformComponent &transform = game.transformComps.addComp(id);
                    transform.setTranslation({10+x*8, -5+x*1, 10-y*8});
                    transform.setScaling({1.25f, 0.8f, 1.25f});

                    CloudComponent &cloud = game.cloudComps.addComp(id);
                    CloudRenderData &renderData = *cloud.renderData;
                    unsigned int i = 0;
                    for (auto it = std::begin(renderData.sphereData); it != std::end(renderData.sphereData); ++it)
                    {
                        it->offset = {0.6f * (i / 4), 0, 0.6f * (i % 4)};
                        it->color = {(14-x)/14.0f+0.1f, (14-y)/14.0f+0.1, 0.1f*(((x+y)*7)%10)+0.1, 1};

                        ++i;
                    }
                }
            }
        }*/

        //make initialPlatform
        {
            CloudPlatformParameter params;
            params.translation={0,-1,0};
            params.scale={1,0.5,1};
            params.size = {5,5};
            params.type=CloudType::SQUARE;
            //params.color={100,150,30,255};
            TransformComponent& startPlatform = game.getCloudGenerator().makeCloudPlatform(game,params);

            game.generateNextLevel(startPlatform,params);

            SignGenerator::makeSign(game,{{-1.5,0,2},{0,45*TO_RADIANS,0},"Make it to a red cloud!"}).setParent(startPlatform,false);
            SignGenerator::makeSign(game,{{1.5,0,2},{0,-45*TO_RADIANS,0},"Move with WASD\n\n Look around with Mouse"}).setParent(startPlatform,false);
            SignGenerator::makeSign(game,{{-1,0,-4},{0,30*TO_RADIANS,0},"Press SPACE to charge a jump.\n\n Release to jump"}).setParent(startPlatform,false);
            SignGenerator::makeSign(game,{{1,0,-4},{0,-30*TO_RADIANS,0},"Look up/down to \njump higher or farther \n\n RIGHT CLICK to cancel jump."}).setParent(startPlatform,false);
        }

    }
};
REGISTERCLASS(MainScene)

//test cylinder
/*{
    EntityId id = game.createEntity();
    TransformComponent &transform = game.transformComps.addComp(id);
    transform.setTranslation({0,2,0});

    game.meshComps.addComp(id).mesh = game.getAssetManager().getMesh(MeshIds::UNIT_CYLINDER);

    MaterialComponent &material = game.materialComps.addComp(id);
    material.material = game.getAssetManager().getMaterial(MaterialIds::UNLIT);
    material.instanceProp.setTexture("diffuseTex",game.getAssetManager().getTexture(TextureIds::DEFAULT));
    material.retrieveUniformLocations();
}*/