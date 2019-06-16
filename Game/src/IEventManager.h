
#ifndef CGUE19_THECHOSENFROG_EVENTMANAGER_H
#define CGUE19_THECHOSENFROG_EVENTMANAGER_H


#include <IObject.h>
#include <PxBroadPhase.h>
#include <PxSimulationEventCallback.h>
#include <characterkinematic/PxController.h>
#include "components/Entity.h"

class Game;
class CharControllerComponent;
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

	/**
	 * Called whenever the window size/resolution changes, and once right after initializing the game
	 * @param game
	 * @param width
	 * @param height
	 */
    virtual void windowResizeCallback(Game& game, int width, int height)=0;

    /**
     * Called whenever the game settings change
     * @param game
     */
    virtual void settingsChanged(Game& game)=0;

    /**
     * Called when the grounded flag is toggled on given character controller
     * @param game
     * @param charController
     */
    virtual void groundedChanged(Game &game, CharControllerComponent &charController)=0;

    virtual void onConstraintBreak(Game& game, physx::PxConstraintInfo *constraints, physx::PxU32 count)=0;
    virtual void onWake(Game& game,physx::PxActor **actors, physx::PxU32 count)=0;
    virtual void onSleep(Game& game,physx::PxActor **actors, physx::PxU32 count)=0;
    virtual void onContact(Game& game,const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs,physx::PxU32 nbPairs)=0;
    virtual void onTrigger(Game& game,physx::PxTriggerPair *pairs, physx::PxU32 count)=0;
    virtual void onAdvance(Game& game,const physx::PxRigidBody *const *bodyBuffer, const physx::PxTransform *poseBuffer,const physx::PxU32 count)=0;
    virtual void onShapeHit(Game& game,const physx::PxControllerShapeHit &hit)=0;
    virtual void onControllerHit(Game& game,const physx::PxControllersHit &hit)=0;
    virtual void onObstacleHit(Game& game,const physx::PxControllerObstacleHit &hit)=0;
};

#endif //CGUE19_THECHOSENFROG_EVENTMANAGER_H