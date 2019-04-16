
#ifndef CGUE19_THECHOSENFROG_EVENTMANAGER_H
#define CGUE19_THECHOSENFROG_EVENTMANAGER_H


#include <IObject.h>

class Game;

class IEventManager : public IObject {
public:
	virtual void keyCallback(Game& game, int key, int scancode, int action, int mods) = 0;
	virtual void mousePosCallback(Game& game, double x, double y) = 0;
	virtual void mouseButtonCallback(Game& game, int button, int action, int mods) = 0;
	virtual void mouseScrollCallback(Game& game, double xOffset, double yOffset)=0;
	virtual void mouseCursorEnterCallback(Game& game, int entered)=0;
};

#endif //CGUE19_THECHOSENFROG_EVENTMANAGER_H