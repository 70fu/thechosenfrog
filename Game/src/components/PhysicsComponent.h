#ifndef CGUE19_THECHOSENFROG_PHYSICSCOMPONENT_H
#define CGUE19_THECHOSENFROG_PHYSICSCOMPONENT_H

#include "Component.h"

namespace physx
{
    struct PxRigidActor;
    struct PxShape;
}

/**
 * A physx actor wrapped in a component.
 * Only use methods when actor is set
 */
class PhysicsComponent : public Component
{
public:
    static constexpr unsigned char MAX_SHAPES = 16;
private:
    //static array used for Collision mask and layer methods to acquire shapes from actor
    static physx::PxShape* shapeTmp[];

    physx::PxRigidActor* actor;
public:
    enum Layers : unsigned char
    {
        PLAYER=1<<0,
        PLATFORM=1<<1,
        TOWEL=1<<2,
        TONGUE=1<<3,

        ALL=255
    };

    physx::PxRigidActor *getActor() const;

    /**
     * Precondition: has not been set before
     * Set actor of this physics component, this component takes ownership of the actor
     * sets user data to entity id
     * @param actor
     */
    void setActor(physx::PxRigidActor *actor);

    using CollisionMask=std::underlying_type<Layers>::type;

    void cleanup(Game &game) override;

    //these methods edit all shapes and return values from the first shape and are not safe to call from multiple threads
    CollisionMask getCollisionMask() const;
    //void setCollisionMask(CollisionMask mask);
    //void enableCollisionWith(CollisionMask mask);
    //void disableCollisionWith(CollisionMask mask);

    CollisionMask getLayer() const;
    //void setLayer(Layers layer);

    void setLayerAndCollisionMask(CollisionMask layer, CollisionMask mask);
};


#endif //CGUE19_THECHOSENFROG_PHYSICSCOMPONENT_H
