
#ifndef CGUE19_THECHOSENFROG_EVENTMANAGER_H
#define CGUE19_THECHOSENFROG_EVENTMANAGER_H


#include <IObject.h>
#include <GLFW/glfw3.h>


class IEventManager : public IObject {
public:
	virtual void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
	virtual void mousePosCallback(GLFWwindow* window, double x, double y) = 0;
	virtual void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) = 0;

};

#endif //CGUE19_THECHOSENFROG_EVENTMANAGER_H