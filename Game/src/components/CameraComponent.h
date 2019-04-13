#ifndef CGUE19_THECHOSENFROG_CAMERA_H
#define CGUE19_THECHOSENFROG_CAMERA_H


#include <mat4x4.hpp>
#include "Component.h"

class CameraComponent : public Component
{
private:
    //cached value, updated when fov,near,far or viewport size changes
    glm::mat4 projectionMatrix;

    int viewportWidth;
    int viewportHeight;
    /**
     * Field of View, but I think its for the y axis in radians
     */
    float fov;
    float near;
    float far;

    void updateProjectionMatrix();
public:
    const glm::mat4& getProjectionMatrix() const;

    int getViewportWidth() const;
    void setViewportWidth(int viewportWidth);
    int getViewportHeight() const;
    void setViewportHeight(int viewportHeight);
    float getFov() const;
    void setFov(float fov);
    float getNear() const;
    void setNear(float near);
    float getFar() const;
    void setFar(float far);
};


#endif //CGUE19_THECHOSENFROG_CAMERA_H
