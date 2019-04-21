#include "IEventManager.h"
#include "RuntimeClasses.h"
#include <ObjectInterfacePerModule.h>
#include <iostream>
#include "Game.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class EventManager : public TInterface<EVENT_MANAGER, IEventManager>{

private:
	void removeTransformChildrenHelper(Game& game, TransformComponent& transform)
	{
		//remove children first before moving to sibling
		if(transform.getFirstChild()!=nullptr)
			removeTransformChildrenHelper(game,*transform.getFirstChild());

		//remove next sibling
		if(transform.getNextSibling()!=nullptr)
			removeTransformChildrenHelper(game,*transform.getNextSibling());

		//destroy yourself
		game.deleteEntity(transform.entity);
	}

public:
	void keyCallback(Game& game, int key, int scancode, int action, int mods) override
	{
		//toggle debug windows
		keyToggleDebugWindows(game,key,scancode,action,mods);

		// move forward
		if (key == GLFW_KEY_W && action == GLFW_PRESS) {
			std::cout << "You pressed W" << std::endl;
		}

		// move left
		if (key == GLFW_KEY_A && action == GLFW_PRESS) {
			std::cout << "You pressed A" << std::endl;
		}

		// move back
		if (key == GLFW_KEY_S && action == GLFW_PRESS) {
			std::cout << "You pressed S" << std::endl;
		}

		// move right
		if (key == GLFW_KEY_D && action == GLFW_PRESS) {
			std::cout << "You pressed D" << std::endl;
		}

		// jump
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
			std::cout << "You pressed Space" << std::endl;
		}

		// special key
		if (key == GLFW_KEY_F && action == GLFW_PRESS) {
			std::cout << "You pressed F" << std::endl;
		}
	}

	void mousePosCallback(Game& game, double x, double y) override
	{
		game.setMousePos(glm::vec2(x,y));
	}

	void mouseButtonCallback(Game& game, int button, int action, int mods) override
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			std::cout << "You pressed the right Mouse-Button" << std::endl;
		}

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			std::cout << "You pressed the left Mouse-Button" << std::endl;
		}

		if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
			std::cout << "You pressed the middle Mouse-Button" << std::endl;
		}
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
		//region if transform component is removed, clear parent of children
		{
			if(componentId==Components::TRANSFORM)
			{
				TransformComponent& transform = game.transformComps[entityId];
				for(TransformComponent* child = transform.getFirstChild();child!=nullptr;child=child->getNextSibling())
					child->clearParent(true);
			}
		}
		//endregion
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
			//...
		}
	}
};

REGISTERCLASS(EventManager);




