#ifndef CGUE19_THECHOSENFROG_CAMERACONTROLLERCOMPONENT_H
#define CGUE19_THECHOSENFROG_CAMERACONTROLLERCOMPONENT_H


#include <vec3.hpp>
#include <mat4x4.hpp>
#include "../Constants.h"
#include "Component.h"

/**
 * Camera Controller using spherical coordinates, taken from Simons ECG submission.
 */
class CameraControllerComponent : public Component
{
public:
    enum ControlFlags : unsigned char
    {
        ROTATE_HOR = 1<<0,
        ROTATE_VER = 1<<1,
        STRAFE = 1<<2,
        RADIUS = 1<<3//increase and decrease radius
    };

    /**
     * These are spherical coordinates
     */
    glm::vec3 coordinates={0,0,0};//x=radius, y=theta (angle between z axis,around x axis), z=phi (angle around y axis)
    glm::vec3 focusPoint={0,0,0};
    float rotationSensitivity = 2*PI/1600;
    float strafeSensitivity = 1/(30.0f);
    float scrollSensitivity = -1;

    //control flags
    std::underlying_type<ControlFlags>::type controlFlags = ~0;//enable all controls
};


#endif //CGUE19_THECHOSENFROG_CAMERACONTROLLERCOMPONENT_H
