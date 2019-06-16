#include "IEventManager.h"
#include "RuntimeClasses.h"
#include <ObjectInterfacePerModule.h>
#include <iostream>
#include "Game.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <PxScene.h>
#include <PxRigidActor.h>

class EventManager : public TInterface<EVENT_MANAGER, IEventManager>{

private:
	void removeTransformChildrenHelper(Game& game, TransformComponent& transform)
	{
		//remove children first before moving to sibling
		//if(transform.getFirstChild()!=nullptr)
		//	removeTransformChildrenHelper(game,*transform.getFirstChild());

		//remove next sibling
		if(transform.getNextSibling()!=nullptr)
			removeTransformChildrenHelper(game,*transform.getNextSibling());

		//destroy yourself
		game.deleteEntity(transform.entity);
	}

	void updateCameraViewports(Game& game, int width, int height)
    {
        for(CameraComponent& camera : game.cameraComps)
        {
            switch(game.settings.gameplay.viewportMode)
            {
                case GameplaySettings::ViewportMode::STRETCH:
                {
                    camera.setViewportSize(glm::vec2(width, height));
                    camera.setScreenViewportPos({0,0});
                    camera.setScreenViewportSize({1,1});
                    break;
                }
                case GameplaySettings::ViewportMode::FIXED_WIDTH:
                {
                    glm::vec2 newSize = glm::vec2(height*(game.settings.gameplay.horFov/camera.getFov()),height);
                    camera.setViewportSize(newSize);

                    //center horizontally
                    float sizeFraction = (newSize.x/width);
                    camera.setScreenViewportPos(glm::vec2((1-sizeFraction)/2.0f,0));
                    camera.setScreenViewportSize(glm::vec2(sizeFraction,1));
                    break;
                }
                default:
                    ImGuiAl::Log::getInstance().Error("Unknown viewport mode %d, cannot be handled",game.settings.gameplay.viewportMode);
            }
        }
    }

    void updateFramebufferSize(Game& game, int width, int height)
    {
	    for(int i = 0;i<FramebufferIds::FRAMEBUFFER_COUNT;++i)
        {
	        FramebufferAsset& fbo = *game.getAssetManager().getFramebuffer(i);
	        if(fbo.getParameters().resizeOnResolutionChange)
            {
	            fbo.resize(glm::uvec2(std::ceil(width*fbo.resizeWidthRatio),std::ceil(height*fbo.resizeHeightRatio)));
            }
        }
    }

public:
	void keyCallback(Game& game, int key, int scancode, int action, int mods) override
	{
		//toggle debug windows
		keyToggleDebugWindows(game,key,scancode,action,mods);

		keyDebugControls(game,key,scancode,action,mods);

        updatePlayerKeyInput(game, key, scancode, action, mods);
	}

	void mousePosCallback(Game& game, double x, double y) override
	{
		game.setMousePos(glm::vec2(x,y));
	}

	void mouseButtonCallback(Game& game, int button, int action, int mods) override
	{
        updatePlayerMouseInput(game,button,action,mods);
	}

	void mouseScrollCallback(Game& game, double xOffset, double yOffset) override
	{
		game.setMouseScrollDelta(glm::vec2(xOffset,yOffset));
	}

	void mouseCursorEnterCallback(Game& game, int entered) override
	{
		if(entered==GLFW_PRESS)
			game.setCursorEnteredWindow(true);
	}

	void entityAdded(Game &game, EntityId entityId) override
	{

	}

	void entityPreRemove(Game &game, EntityId entityId) override
	{
		//region remove entities which are children of transform
		{
			if(game.hasComponents(entityId,Components::TRANSFORM_BIT))
			{
				TransformComponent& transform = game.transformComps[entityId];
				if(transform.getFirstChild()!=nullptr)
					removeTransformChildrenHelper(game,*transform.getFirstChild());
			}
		}
		//endregion
	}

	void componentAdded(Game &game, EntityId entityId, ComponentId componentId) override
	{

	}

	void componentPreRemove(Game &game, EntityId entityId, ComponentId componentId) override
	{

	}

    void windowResizeCallback(Game &game, int width, int height) override
    {
        updateCameraViewports(game,width,height);
        updateFramebufferSize(game,width,height);
    }

    void settingsChanged(Game &game) override
    {
	    //update viewports again, in case camera changes have been made, but no window resize is triggered
        int width,height;
        glfwGetWindowSize(game.getWindow(),&width,&height);
        updateCameraViewports(game,width,height);
    }

    void groundedChanged(Game &game, CharControllerComponent &charController) override
    {
        //if this is a player and grounded is now true, then remove platforms which are below a threshold
        if(charController.grounded && game.hasComponents(charController.entity,Components::PLAYER_BIT))
        {
            for(CloudComponent& cloud: game.cloudComps)
            {
                EntityId entity = cloud.entity;

                //check for transform component
                //TODO only in debug
                if(!game.hasComponents(entity,Components::TRANSFORM_BIT))
                {
                    continue;
                }

                //assume a cloud has a transform
                TransformComponent& transform = game.transformComps[entity];

                if(charController.landedOn.y-transform.getGlobalTranslation().y>=CLOUD_DISAPPEAR_DIST)
                {
                    //TODO trigger cool disappear animation
                    game.deleteEntity(transform.getRoot().entity);
                }
            }

            //set loose height
            game.playerComps[charController.entity].looseHeight = charController.landedOn.y-CLOUD_DISAPPEAR_DIST-1;
        }
    }

    void onConstraintBreak(Game &game, physx::PxConstraintInfo *constraints, physx::PxU32 count) override
    {

    }

    void onWake(Game &game, physx::PxActor **actors, physx::PxU32 count) override
    {

    }

    void onSleep(Game &game, physx::PxActor **actors, physx::PxU32 count) override
    {

    }

    void onContact(Game &game, const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs,
                   physx::PxU32 nbPairs) override
    {

    }

    void onTrigger(Game &game, physx::PxTriggerPair *pairs, physx::PxU32 count) override
    {

    }

    void onAdvance(Game &game, const physx::PxRigidBody *const *bodyBuffer, const physx::PxTransform *poseBuffer,
                   const physx::PxU32 count) override
    {

    }

    void onShapeHit(Game &game, const physx::PxControllerShapeHit &hit) override
    {
        //TODO if platform, set platform of character controller component
        //set grounded on
        //game.charControllerComps[*((EntityId*)hit.controller->getUserData())].groundedOn = hit.actor;
    }

    void onControllerHit(Game &game, const physx::PxControllersHit &hit) override
    {

    }

    void onObstacleHit(Game &game, const physx::PxControllerObstacleHit &hit) override
    {

    }

private:
	/**
	 * this is only used to make the key callback function more readable (
	 * toggles respective debug windows if Control+<Number> is pressed
	 */
	inline void keyToggleDebugWindows(Game& game, int key, int scancode, int action, int mods)
	{
		if((mods&GLFW_MOD_CONTROL)!=0 && action==GLFW_PRESS)
		{
			if(key==GLFW_KEY_1)
				game.getDebugGUI()->toggleWindow(DebugWindowIds::LOGGER);
			if(key==GLFW_KEY_2)
				game.getDebugGUI()->toggleWindow(DebugWindowIds::COMPONENT_VIEWER);
			if(key==GLFW_KEY_3)
				game.getDebugGUI()->toggleWindow(DebugWindowIds::ASSET_VIEWER);
            if(key==GLFW_KEY_4)
                game.getDebugGUI()->toggleWindow(DebugWindowIds::APP_SETTINGS);
			//...
		}
	}

	inline void toggleOpenGL(GLenum v)
    {
        if(glIsEnabled(v))
            glDisable(v);
        else
            glEnable(v);
    }

	/**
	 * F11 - Reload Scene
	 * F10 - Toggle Backface culling
	 * F9 - Toggle wireframe rendering
	 * F8 - Toggle cursor
	 * @param game
	 * @param key
	 * @param scancode
	 * @param action
	 * @param mods
	 */
	inline void keyDebugControls(Game& game, int key, int scancode, int action, int mods)
    {
	    if(action==GLFW_PRESS)
        {
	        if(key==GLFW_KEY_F11)
            {
	            game.reloadScene();
            }
	        else if(key==GLFW_KEY_F10)
            {
	            toggleOpenGL(GL_CULL_FACE);
            }
	        else if(key==GLFW_KEY_F9)
            {
	            GLint mode[2];
	            glGetIntegerv(GL_POLYGON_MODE,&mode[0]);
                glPolygonMode(GL_FRONT_AND_BACK, mode[0]==GL_LINE?GL_FILL:GL_LINE);
            }
	        else if(key==GLFW_KEY_F8)
            {
                int mode = glfwGetInputMode(game.getWindow(),GLFW_CURSOR);
                glfwSetInputMode(game.getWindow(),GLFW_CURSOR,mode==GLFW_CURSOR_DISABLED?GLFW_CURSOR_NORMAL:GLFW_CURSOR_DISABLED);
            }
        }
    }

    inline void updatePlayerKeyInput(Game &game, int key, int scancode, int action, int mods)
    {
	    for(PlayerComponent& player : game.playerComps)
        {
	        if(player.activeInput==PlayerComponent::PlayerInput::KEYBOARD_MOUSE)
            {
	            //jump button
                if (player.input.keyboard.jumpKey == key &&
                        action == GLFW_RELEASE &&
                        game.hasComponents(player.entity, Components::CHAR_CONTROLLER_BIT))
                {
                    game.charControllerComps[player.entity].wantJump = true;
                }

                //tongue key
                if(player.input.keyboard.tongueKey==key && action==GLFW_PRESS)
                {
                    //TODO activate tongue
                }
            }

        }
    }

    inline void updatePlayerMouseInput(Game& game, int button, int action, int mods)
    {
        for(PlayerComponent& player : game.playerComps)
        {
            if(player.activeInput==PlayerComponent::PlayerInput::KEYBOARD_MOUSE)
            {
                //tongue mouse button
                if(player.input.keyboard.tongueMouseButton==button && action==GLFW_PRESS)
                {
                    //TODO activate tongue
                }
            }

        }
    }
};

REGISTERCLASS(EventManager);




