
#ifndef CGUE19_THECHOSENFROG_EVENTMANAGER_H
#define CGUE19_THECHOSENFROG_EVENTMANAGER_H


#include <IObject.h>
#include "components/Entity.h"

class Game;

class IEventManager : public IObject {
public:
	virtual void keyCallback(Game& game, int key, int scancode, int action, int mods) = 0;
	virtual void mousePosCallback(Game& game, double x, double y) = 0;
	virtual void mouseButtonCallback(Game& game, int button, int action, int mods) = 0;
	virtual void mouseScrollCallback(Game& game, double xOffset, double yOffset)=0;
	virtual void mouseCursorEnterCallback(Game& game, int entered)=0;
	/**
	 * Called whenever a new entity has been added to the game
	 * @param game
	 * @param entityId
	 */
	virtual void entityAdded(Game& game, EntityId entityId)=0;
	/**
	 * Called before an entity is going to be deleted, so components can still be accessed
	 * @param game
	 * @param entityId
	 */
	virtual void entityPreRemove(Game &game, EntityId entityId)=0;
	/**
	 * Called whenever a component has been added to an entity
	 * @param game
	 * @param entityId
	 * @param componentId
	 */
	virtual void componentAdded(Game& game, EntityId entityId, ComponentId componentId)=0;
	/**
	 * Called right before removing component of given id from given entity
	 * @param game
	 * @param entityId
	 * @param componentId
	 */
	virtual void componentPreRemove(Game &game, EntityId entityId, ComponentId componentId)=0;
};

#endif //CGUE19_THECHOSENFROG_EVENTMANAGER_H