#include "PhysicsComponent.h"
#include "../Game.h"
#include <PxScene.h>
#include <PxRigidActor.h>

using namespace ImGuiAl;

physx::PxShape* PhysicsComponent::shapeTmp[MAX_SHAPES];

void PhysicsComponent::cleanup(Game &game)
{
    if(actor==nullptr)
        return;

    //remove actor from scene
    game.getPhysics().getScene()->removeActor(*actor);

    //decrease reference count
    actor->release();

    actor = nullptr;
}

PhysicsComponent::CollisionMask PhysicsComponent::getCollisionMask() const
{
#ifndef NDEBUG
    if(actor==nullptr)
    {
        Log::getInstance().Warn("actor must be set before calling layer and collision mask methods on PhysicsComponent");
        return 0;
    }
#endif

    actor->getShapes(&shapeTmp[0],1);
    return static_cast<Layers>(shapeTmp[0]->getSimulationFilterData().word1);
}

PhysicsComponent::CollisionMask PhysicsComponent::getLayer() const
{
#ifndef NDEBUG
    if(actor==nullptr)
    {
        Log::getInstance().Warn("actor must be set before calling layer and collision mask methods on PhysicsComponent");
        return 0;
    }
#endif

    actor->getShapes(&shapeTmp[0],1);
    return static_cast<Layers>(shapeTmp[0]->getSimulationFilterData().word0);
}

void PhysicsComponent::setLayerAndCollisionMask(PhysicsComponent::CollisionMask layer, PhysicsComponent::CollisionMask mask)
{
#ifndef NDEBUG
    if(actor==nullptr)
    {
        Log::getInstance().Warn("actor must be set before calling layer and collision mask methods on PhysicsComponent");
        return;
    }
#endif

    physx::PxFilterData filterData;
    filterData.word0 = layer;
    filterData.word1 = mask;

    unsigned char shapeCount = actor->getShapes(&shapeTmp[0],MAX_SHAPES);
    for(size_t i = 0;i<shapeCount;++i)
    {
        shapeTmp[i]->setSimulationFilterData(filterData);
        shapeTmp[i]->setQueryFilterData(filterData);
    }
}

physx::PxRigidActor *PhysicsComponent::getActor() const
{
    return actor;
}

void PhysicsComponent::setActor(physx::PxRigidActor *a)
{
#ifndef NDEBUG
    if(actor!=nullptr)
    {
        Log::getInstance().Warn("actor should only be set once");
        return;
    }
#endif

    actor = a;
    actor->userData = (void*)entity;
}
