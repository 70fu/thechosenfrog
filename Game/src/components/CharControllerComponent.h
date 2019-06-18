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
    physx::PxController* controller=nullptr;
public:
    //TODO put configuration in struct

    /* --------------------------------------------- */
    // Configuration
    /* --------------------------------------------- */
    CharControllerConfiguration config;

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

    /**
     * Should be in range 0-1
     * 0 means distance=calculated max distance * maxDistLookingFactor AND height=calculated max height*minHeightLookingFactor
     * 1 means distance=calculated max distance * minDistLookingFactor AND height=calculated max height*maxHeightLookingFactor
     * values inbetween are interpolated using cubicEaseOut between mmin and max looking factors
     * This value is set by for the player depending on where he/she looks, straight up = 1 -> jump high not far, forward=0 -> jump far not high
     */
    float distanceHeightRatio = 0;

    /* --------------------------------------------- */
    // DEBUG
    /* --------------------------------------------- */
    float cachedJumpHeight;
    float cachedJumpDistance;
    float cachedJumpDuration;
    glm::vec3 jumpedFrom={0,0,0};
    glm::vec3 landedOn={0,0,0};
    float lastJumpHeight = 0;

    /* --------------------------------------------- */
    // Statistics
    /* --------------------------------------------- */
    //TODO debug gui
    unsigned int jumpCount = 0;


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

    /**
     * Calculates the initial speed and gravity of a jump, if the controller would jump with currently set parameters and jumpStrength
     * updates and sets the cache variables.
     * jumpStrength is clamped between 0 and 1
     * @return initial speed and gravity of the jump
     */
    glm::vec3 calculateJump();

    /**
     * Calculates the gravity and velocity needed to perform a jump with given distance height at peak of the jump in given time
     * the calculations are based on this talk: https://www.youtube.com/watch?v=hG9SzQxaCm8
     * @param jumpDistance
     * @param jumpHeight
     * @param jumpDuration
     * @return {horizontal initial speed,vertical initial speed, gravity}
     */
    static glm::vec3 calculateGravityAndSpeed(float jumpDistance, float jumpHeight, float jumpDuration);
    /**
     * @param jumpDistance
     * @param jumpHeight
     * @param jumpDuration
     * @return a and b of parabola formula
     */
    static glm::vec2 calculateParabola(float jumpDistance,float jumpHeight,float jumpDuration);

    /**
     * @param gravityAndSpeed
     * @return a and b of parabola formula
     */
    static glm::vec2 calculateParabola(glm::vec3 gravityAndSpeed);

    static void calculateJump(float jumpStrength,float distanceHeightRatio,CharControllerConfiguration conf,float& outJumpDistance,float& outJumpHeight, float& outJumpDuration);
};


#endif //CGUE19_THECHOSENFROG_CHARCONTROLLERCOMPONENT_H
