#include "CharControllerComponent.h"
#include "../logger/imguial_log.h"
#include <characterkinematic/PxController.h>
#include <PxRigidDynamic.h>
#include <algorithm>
#include <gtx/easing.hpp>

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

glm::vec3 CharControllerComponent::calculateJump()
{
    //TODO move these calculations where jump strength is charged up, such that these can be hot reloaded (I could imagine, that the interpolation needs much tweaking)

    //clamp jump strength
    jumpStrength = std::clamp(jumpStrength,0.0f,1.0f);

    //calculate properties of jump from jump strength and parameters
    cachedJumpDistance = (maxJumpDistance-minJumpDistance)*jumpStrength+minJumpDistance;
    cachedJumpHeight = (maxJumpHeight-minJumpHeight)*jumpStrength+minJumpHeight;
    cachedJumpDuration = (maxJumpDuration-minJumpDuration)*jumpStrength+minJumpDuration;

    //apply distance height ratio
    cachedJumpDistance *= (maxDistLookingFactor-minDistLookingFactor)*glm::cubicEaseOut(1-distanceHeightRatio)+minDistLookingFactor;
    cachedJumpHeight *= (maxHeightLookingFactor-minHeightLookingFactor)*glm::cubicEaseOut(distanceHeightRatio)+minHeightLookingFactor;

    return calculateGravityAndSpeed(cachedJumpDistance,cachedJumpHeight,cachedJumpDuration);
}

glm::vec3 CharControllerComponent::calculateGravityAndSpeed(float jumpDistance, float jumpHeight, float jumpDuration)
{
    return {
        jumpDistance/jumpDuration,//horizontal initial speed
        (4*jumpHeight)/jumpDuration,//vertical initial speed
        -(8*jumpHeight)/(jumpDuration*jumpDuration)//gravity
    };
}
