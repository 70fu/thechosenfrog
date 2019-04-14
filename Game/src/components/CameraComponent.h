#ifndef CGUE19_THECHOSENFROG_CAMERA_H
#define CGUE19_THECHOSENFROG_CAMERA_H


#include <mat4x4.hpp>
#include <vec2.hpp>
#include "Component.h"

class CameraComponent : public Component
{
private:
    //cached value, updated when fov,near,far or viewport size changes
    glm::mat4 projectionMatrix;

    //viewport coordinates and size is given relative to window size, so 1 means 100% and 0 is 0 percent
    /**
     * Left bottom corner of viewport
     */
    glm::vec2 viewportPos={0,0};
    glm::vec2 viewportSize={1,1};
    /**
     * Field of View, but I think its for the y axis in radians
     */
    float fov=1;
    float near=0.1f;
    float far=100;

    void updateProjectionMatrix();
public:
    const glm::mat4& getProjectionMatrix() const;

    const glm::vec2& getViewportPos() const;
    void setViewportPos(const glm::vec2& viewportPos);
    const glm::vec2& getViewportSize() const;
    void setViewportSize(const glm::vec2& viewportSize);
    float getFov() const;
    void setFov(float fov);
    float getNear() const;
    void setNear(float near);
    float getFar() const;
    void setFar(float far);
};


#endif //CGUE19_THECHOSENFROG_CAMERA_H
