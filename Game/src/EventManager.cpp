#include "IEventManager.h"
#include "RuntimeClasses.h"
#include <ObjectInterfacePerModule.h>
#include <iostream>

class EventManager : public TInterface<EVENT_MANAGER, IEventManager> {
	void keyCallback(GLFWwindow * window, int key, int scancode, int action, int mods) override
	{

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

	void mousePosCallback(GLFWwindow * window, double x, double y) override
	{
	}

	void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods) override
	{
	}

};
REGISTERCLASS(EventManager);




