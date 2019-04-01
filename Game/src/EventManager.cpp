#include "IEventManager.h"
#include "RuntimeClasses.h"
#include <ObjectInterfacePerModule.h>
#include <iostream>
#include "Game.h"

class EventManager : public TInterface<EVENT_MANAGER, IEventManager>{
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
		//std::cout << "Mouse is moving: x: " << x << " y: " << y << std::endl;
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
			//...
		}
	}
};

REGISTERCLASS(EventManager);




