#ifndef CGUE19_THECHOSENFROG_CHARCONTROLLERCOMPONENT_H
#define CGUE19_THECHOSENFROG_CHARCONTROLLERCOMPONENT_H

#include <vec2.hpp>
#include <vec3.hpp>
#include "Component.h"

namespace physx
{
    struct PxController;
    struct PxRigidActor;
}

class CharControllerComponent : public Component
{
private:
    physx::PxController* controller;
public:
    //TODO put configuration in struct
    //TODO max speed

    /* --------------------------------------------- */
    // Configuration
    /* --------------------------------------------- */
    //speeds are always given in units/s
    float groundWalkForce=0;
    float airWalkSpeed=0;
    float minJumpDistance=0;
    float maxJumpDistance=0;
    float minJumpHeight=0;
    float maxJumpHeight=0;
    float minJumpDuration = 0;
    float maxJumpDuration = 0;
    float maxGroundSpeed = 0;

    /* --------------------------------------------- */
    // State
    /* --------------------------------------------- */
    bool grounded = false;
    //the thing this entity is standing on
    physx::PxRigidActor* groundedOn = nullptr;
    bool jumping = false;
    //time in air, reset when leaving the ground
    float airTime = 0;
    glm::vec3 acc = {0,0,0};
    glm::vec3 vel = {0,0,0};

    /* --------------------------------------------- */
    // Inputs for this frame, can be set from multiple locations (AI, player input, ...)
    /* --------------------------------------------- */
    bool wantJump = false;
    /**
     * Relative to looking direction
     * y is z
     */
    glm::vec2 direction;

    /**
     * Should be in range 0-1
     */
    float jumpStrength = 0;

    /* --------------------------------------------- */
    // DEBUG
    /* --------------------------------------------- */
    float cachedJumpHeight;
    float cachedJumpDistance;
    float cachedJumpDuration;
    glm::vec3 jumpedFrom={0,0,0};
    glm::vec3 landedOn={0,0,0};
    float lastJumpHeight = 0;

    /**
     * calculateJump needs to be called before
     * @return the height of the jump if a jump is performed with current jumpStrength, linear interpolation of min and max jump heights
     */
    //float getJumpHeight() const;
    /**
     * calculateJump needs to be called before
     * @return the distance of the jump if a jump is performed with current jumpStrength, linear interpolation of min and max jump distance
     */
    //float getJumpDistance() const;

    /**
     * given that jump ends on the same height as it has begun
     * calculateJump needs to be called before
     * @return the duration of the jump if a jump is performed with current jumpStrength, linear interpolation of min and max jump distance
     */
    //float getJumpDuration() const;

    /**
     * calculateJump needs to be called before
     * @return {a,b} of the jump parabola equation if jump
     */
    //const glm::vec2& getJumpParabolaFactors() const;*/

    physx::PxController *getController() const;

    /**
     * Precondition: has not been set before
     * Set controller of this character controller component, this component takes ownership of the controller
     * sets user data of the controller and the underlying actor to entity id
     * @param controller
     */
    void setController(physx::PxController *controller);

    void cleanup(Game &game) override;
};


#endif //CGUE19_THECHOSENFROG_CHARCONTROLLERCOMPONENT_H
