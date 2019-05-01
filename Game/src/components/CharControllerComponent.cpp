#include "CharControllerComponent.h"
#include "../logger/imguial_log.h"
#include <characterkinematic/PxController.h>
#include <PxRigidDynamic.h>

physx::PxController *CharControllerComponent::getController() const
{
    return controller;
}

void CharControllerComponent::setController(physx::PxController *c)
{
#ifndef NDEBUG
    if(controller!=nullptr)
    {
        ImGuiAl::Log::getInstance().Warn("character controller should only be set once");
        return;
    }
#endif

    controller = c;

    //set user data to entity id
    controller->setUserData(&entity);
    controller->getActor()->userData = &entity;
}

void CharControllerComponent::cleanup(Game &game)
{
    if(controller==nullptr)
        return;

    //decrease reference count
    controller->release();

    controller = nullptr;
}
