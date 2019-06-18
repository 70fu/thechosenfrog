

#include <ObjectInterfacePerModule.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Game.h"
#include "RuntimeClasses.h"
#include "util/CameraUtil.h"
#include <gtx/rotate_vector.hpp>
#include <PxRigidActor.h>
#include <PxScene.h>
#include <characterkinematic/PxController.h>
#include "util/GLMPXConversion.h"

class GameUpdater : public TInterface<RuntimeClassIds::GAME_UPDATER,IGameUpdater>
{
public:
    void update(Game& game) override
    {
        updateCameraControllers(game);

        updatePlayers(game);

        updateCharControllers(game);

        //update physics
        game.getPhysics().step();

        checkLooseCondition(game);


    }

private:
    //used for retrieving shapes
    physx::PxShape* shapeTmp[1];


    inline void updatePlayers(Game& game)
    {
        for(PlayerComponent& player : game.playerComps)
        {
            updatePlayerCharController(game,player);
        }
    }

    inline void updatePlayerCharController(Game& game, PlayerComponent& player)
    {
        if(game.hasComponents(player.entity,Components::CHAR_CONTROLLER_BIT))
        {
            CharControllerComponent& controller = game.charControllerComps[player.entity];
            switch(player.activeInput)
            {
                case PlayerComponent::PlayerInput::KEYBOARD_MOUSE:
                {
                    //increase jump strength
                    if(!player.jumpCancelled)
                        controller.jumpStrength+=glfwGetKey(game.getWindow(),player.input.keyboard.jumpKey)*PlayerComponent::JUMP_STRENGTH_INCREASE*FIXED_DELTA;
                    else
                        controller.jumpStrength = 0;

                    //set direction vector
                    controller.direction.x=glfwGetKey(game.getWindow(),player.input.keyboard.rightKey)-glfwGetKey(game.getWindow(),player.input.keyboard.leftKey);
                    controller.direction.y=glfwGetKey(game.getWindow(),player.input.keyboard.backKey)-glfwGetKey(game.getWindow(),player.input.keyboard.forwardKey);
                    break;
                }
                case PlayerComponent::PlayerInput::JOYSTICK:
                {
                    //TODO player joystick input
                    ImGuiAl::Log::getInstance().Warn("player joystick input is not yet implemented");
                    break;
                }
                default:
                    ImGuiAl::Log::getInstance().Warn("Unknown player input type");
            }

            //if there is also a camera controller on a child object, then set distanceHeightRation based on vertical looking angle
            //1=looking straight up, 0=looking straight or down
            if(game.hasComponents(player.entity,Components::TRANSFORM_BIT))
            {
                TransformComponent& transform = game.transformComps[player.entity];
                for(TransformComponent* child = transform.getFirstChild();child!=nullptr;child = child->getNextSibling())
                {
                    if(game.hasComponents(child->entity,Components::CAMERA_CONTROLLER_BIT))
                    {
                        CameraControllerComponent &camController = game.cameraControllerComps[child->entity];
                        static constexpr float ANGLE_MAX = PI/2;
                        static constexpr float ANGLE_MIN = -PI/4;
                        static constexpr float ANGLE_RANGE = ANGLE_MAX-ANGLE_MIN;
                        controller.distanceHeightRatio = (std::max(ANGLE_MIN, camController.coordinates.y)-ANGLE_MIN) / ANGLE_RANGE;//TODO replace constants if this becomes configurable in the component
                        break;
                    }
                }
            }
        }
    }

    inline void updateCameraControllers(Game& game)
    {
        for(CameraControllerComponent& controller : game.cameraControllerComps)
        {
            glm::vec3 coordChange = controller.coordinates;

            //update coordinates and focus point if control flags are set
            if((controller.controlFlags&CameraControllerComponent::ROTATE_HOR)!=0)
            {
                controller.coordinates.z += -game.getMouseMoveDelta().x * controller.rotationSensitivity;
            }
            if((controller.controlFlags&CameraControllerComponent::ROTATE_VER)!=0)
            {
                controller.coordinates.y += -game.getMouseMoveDelta().y * controller.rotationSensitivity;

                //clamp theta to 90 degrees
                if (controller.coordinates.y > PI / 2)//TODO make constant (PI/2) configurable in component
                    controller.coordinates.y = PI / 2;
                else if (controller.coordinates.y < -PI / 2)
                    controller.coordinates.y = -PI / 2;
            }
            if((controller.controlFlags&CameraControllerComponent::STRAFE)!=0)
            {
                int leftRight = glfwGetKey(game.getWindow(),GLFW_KEY_D)-glfwGetKey(game.getWindow(),GLFW_KEY_A);
                int backForth = glfwGetKey(game.getWindow(),GLFW_KEY_S)-glfwGetKey(game.getWindow(),GLFW_KEY_W);
                controller.focusPoint += static_cast<glm::vec3>(CameraUtil::calcRotationMatrix(controller.coordinates)*glm::vec4(leftRight*controller.strafeSensitivity, 0,backForth*controller.strafeSensitivity,1));
            }
            if((controller.controlFlags&CameraControllerComponent::RADIUS)!=0)
            {
                controller.coordinates.x += game.getMouseScrollDelta().x * controller.scrollSensitivity;
            }

            coordChange = controller.coordinates-coordChange;

            //update transform if it exists
            if(game.hasComponents(controller.entity,Components::TRANSFORM_BIT))
            {
                TransformComponent& transform = game.transformComps[controller.entity];

                //TODO only apply relative changes
                if((controller.controlFlags&CameraControllerComponent::STRAFE)!=0)
                    transform.setTranslation(CameraUtil::calcTransformMatrix(controller.coordinates,controller.focusPoint)[3]);
                transform.setRotation(glm::vec3(controller.coordinates.y,controller.coordinates.z,0));
            }
        }
    }

    /**
     * Looks at the inputs of all char controllers and performs movement
     * resets jump inputs afterwards
     * @param game
     */
    inline void updateCharControllers(Game& game)
    {
        using namespace GLMPXConversion;

        static constexpr float FALLING_ACC = -9.81f;
        //static constexpr float GROUNDED_DIST = 0.01f;
        static constexpr float MIN_DIST = 0.000001f;//TODO adapt
        for(CharControllerComponent& c : game.charControllerComps)
        {
            if(game.hasComponents(c.entity,Components::TRANSFORM_BIT))
            {
                TransformComponent& transform = game.transformComps[c.entity];

                //cache old acceleration for integration later
                glm::vec3 oldAcc = c.acc;

                //CURRENTLY UNUSED, PHYSX CHECKS CONTROLLER STATE ON MOVEMENT
                //manage grounded and jumping flag
                /*if(game.hasComponents(c.entity,Components::PHYSICS_BIT))
                {
                    PhysicsComponent& physics = game.physicsComps[c.entity];
                    //perform test
                    physics.getActor()->getShapes(&shapeTmp[0],1);
                    physx::PxSweepBuffer sweepBuffer;
                    physx::PxTransform pTrans;//TODOs init
                    physx::PxVec3 dir = {0,-1,0};
                    if(game.getPhysics().getScene()->sweep(shapeTmp[0]->getGeometry().any(),pTrans,dir,1,sweepBuffer))//TODOs adapt dist
                    {
                        if(sweepBuffer.block.distance<GROUNDED_DIST)
                        {
                            if(!c.grounded)
                            {
                                //set grounded platform
                                c.groundedPlatform = *((EntityId*)sweepBuffer.block.actor->userData);

                                //TODOs emit land event

                                //reset y vel and y acc
                                c.acc.y=c.vel.y=oldAcc.y=0;

                                c.grounded = true;
                            }
                        }
                    }
                    else
                    {
                        //if now !grounded, apply constant gravity
                        if(c.grounded)
                        {
                            c.acc.y = FALLING_ACC;
                            c.grounded = false;
                        }
                    }
                }*/

                //perform jump if requested and possible
                if(c.wantJump && c.grounded)
                {
                    //initial velocity and acceleration to achieve desired jump properties
                    glm::vec3 jump = c.calculateJump();
                    c.vel = glm::rotateY(glm::vec3({0,0,-jump.x}),transform.getRotation().y);
                    c.vel.y = jump.y;
                    oldAcc.y = c.acc.y = jump.z;

                    //set state
                    c.jumping = true;
                    c.jumpStrength = 0;

                    //debug
                    c.jumpedFrom=c.landedOn=transform.getTranslation();
                    c.lastJumpHeight = 0;

                    //TODO emit jump event
                }

                //movement is the only force except gravity that is applied to controllers
                //so x and z component can be set directly
                glm::vec3 dir = {0,0,0};
                if(c.direction.x!=0 || c.direction.y!=0)
                {
                    dir = glm::normalize(
                            glm::rotateY(glm::vec3(c.direction.x, 0, c.direction.y), transform.getRotation().y));
                    dir *= (c.grounded ? c.config.groundWalkForce : c.config.airWalkSpeed);
                }
                else
                {
                    //apply friction
                    //TODO remove hard coded value
                    if(c.grounded && !c.jumping)
                        c.vel*=0.9;
                }
                //if c is grounded apply dir as force, otherwise apply dir directly to position
                glm::vec3 airMove = {0,0,0};
                if(c.grounded)
                {
                    c.acc.x = dir.x;
                    c.acc.z = dir.z;
                }
                else
                {
                    c.acc.x=c.acc.z=0;
                    airMove=dir*FIXED_DELTA;
                }


                //CURRENTLY UNUSED INTEGRATION, POSITION INTEGRATION IS DONE BY PHYSX
                //integrate using velocity verlet integration
                /*glm::vec3 halfStepVel = c.vel+oldAcc* FIXED_DELTA/2.0f;
                transform.setTranslation(transform.getTranslation()+halfStepVel*FIXED_DELTA);
                c.vel = halfStepVel+c.acc*FIXED_DELTA/2.0f;*/

                //integrate velocity using velocity verlet integration
                c.vel = c.vel+(oldAcc+c.acc)*(FIXED_DELTA/2.0f);

                //cap velocity if grounded
                if(c.grounded && !c.jumping && glm::length(c.vel)>c.config.maxGroundSpeed)
                    c.vel = glm::normalize(c.vel)*c.config.maxGroundSpeed;

                //move using character controller from physx
                physx::PxControllerCollisionFlags flags = c.getController()->move(glmToPx(c.vel*FIXED_DELTA+airMove),MIN_DIST,FIXED_DELTA,physx::PxControllerFilters());//TODO set controller filters

                //set jumping and grounded state, depending on collision flags
                bool touchFloor = flags&physx::PxControllerCollisionFlag::eCOLLISION_DOWN;

                //apply gravity if falling
                if(!touchFloor)
                {
                    //reset jumpStrength
                    c.jumpStrength = 0;

                    if(c.grounded)
                    {
                        //TODO emit not grounded anymore event
                        c.airTime = 0;
                    }
                    if(!c.jumping)
                        c.acc.y = FALLING_ACC;
                }
                else
                {
                    //reset y vel and
                    c.vel.y=0;
                    c.jumping = false;

                    if(!c.grounded)
                    {
                        //debug
                        c.landedOn = pxToGlm(c.getController()->getPosition());
                    }
                }

                //set grounded flag and emit event if flag changed
                bool emitGroundedChangedEvent = touchFloor!=c.grounded;
                c.grounded = touchFloor;
                if(emitGroundedChangedEvent)
                    game.getEventManager()->groundedChanged(game, c);

                //set position of transform component
                transform.setTranslation(pxToGlm(c.getController()->getPosition()));

                //TODO this is jump debug
                if(c.jumping)
                {
                    c.lastJumpHeight = std::max(c.lastJumpHeight,transform.getTranslation().y-c.jumpedFrom.y);
                }
            }

            //reset jump inputs
            c.wantJump = false;

            //increase or reset air time
            if(!c.grounded)
                c.airTime+=FIXED_DELTA;
        }
    }

    inline void checkLooseCondition(Game& game)
    {
        for(PlayerComponent& player : game.playerComps)
        {
            if(!player.hasLost && game.hasComponents(player.entity,Components::TRANSFORM_BIT))
            {
                TransformComponent& transform = game.transformComps[player.entity];

                //check height
                if(transform.getGlobalTranslation().y<player.looseHeight)
                {
                    player.hasLost = true;

                    //make a screenspace text
                    EntityId id = game.createEntity();
                    TransformComponent &textTrans = game.transformComps.addComp(id);
                    textTrans.setTranslation({game.settings.display.windowWidth/2-200,game.settings.display.windowHeight/2,0});

                    TextComponent& text = game.textComps.addComp(id);
                    text.text="      You Loose!\n\nPress F11 to Restart";
                    text.inScreenspace = true;
                    text.font = game.getAssetManager().getBitmapFont(BitmapFontIds::DEFAULT);
                    text.color = {0,0,0,255};
                }
            }
        }
    }
};
REGISTERCLASS(GameUpdater)

