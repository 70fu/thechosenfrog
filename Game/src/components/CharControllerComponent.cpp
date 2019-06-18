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
    controller->setUserData((void*)entity);
    controller->getActor()->userData = (void*)entity;
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

    calculateJump(jumpStrength,distanceHeightRatio,config,cachedJumpDistance,cachedJumpHeight,cachedJumpDuration);

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

glm::vec2 CharControllerComponent::calculateParabola(float jumpDistance, float jumpHeight, float jumpDuration)
{
    return calculateParabola(calculateGravityAndSpeed(jumpDistance,jumpHeight,jumpDuration));
}

glm::vec2 CharControllerComponent::calculateParabola(glm::vec3 gravityAndSpeed)
{
    return {0.5f*gravityAndSpeed.z,gravityAndSpeed.y};
}

void CharControllerComponent::calculateJump(float jumpStrength, float distanceHeightRatio,
                                            CharControllerConfiguration config,
                                            float &outJumpDistance, float &outJumpHeight, float &outJumpDuration)
{
    //clamp jump strength
    jumpStrength = std::clamp(jumpStrength,0.0f,1.0f);

    //calculate properties of jump from jump strength and parameters
    outJumpDistance = (config.maxJumpDistance-config.minJumpDistance)*jumpStrength+config.minJumpDistance;
    outJumpHeight = (config.maxJumpHeight-config.minJumpHeight)*jumpStrength+config.minJumpHeight;
    outJumpDuration = (config.maxJumpDuration-config.minJumpDuration)*jumpStrength+config.minJumpDuration;

    //apply distance height ratio
    outJumpDistance *= (config.maxDistLookingFactor-config.minDistLookingFactor)*glm::cubicEaseOut(1-distanceHeightRatio)+config.minDistLookingFactor;
    outJumpHeight *= (config.maxHeightLookingFactor-config.minHeightLookingFactor)*glm::cubicEaseOut(distanceHeightRatio)+config.minHeightLookingFactor;
}
