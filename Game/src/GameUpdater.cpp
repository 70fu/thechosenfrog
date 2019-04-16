

#include <ObjectInterfacePerModule.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Game.h"
#include "RuntimeClasses.h"
#include "util/CameraUtil.h"

class GameUpdater : public TInterface<RuntimeClassIds::GAME_UPDATER,IGameUpdater>
{
public:
    void update(Game& game) override
    {
        //region update camera controllers
        {
            for(CameraControllerComponent& controller : game.cameraControllerComps)
            {
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

                //update transform if it exists
                if(game.hasComponents(controller.entity,Components::TRANSFORM_BIT))
                {
                    game.transformComps[controller.entity].setTranslationAndRotation(
                            CameraUtil::calcTransformMatrix(controller.coordinates,controller.focusPoint)[3],
                            glm::vec3(controller.coordinates.y,controller.coordinates.z,0)
                    );
                }
            }
        }
        //endregion
    }
};
REGISTERCLASS(GameUpdater)

