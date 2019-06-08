#ifndef CGUE19_THECHOSENFROG_CAMERA_H
#define CGUE19_THECHOSENFROG_CAMERA_H


#include <mat4x4.hpp>
#include <vec2.hpp>
#include "Component.h"
#include "../assets/Assets.h"

class CameraComponent : public Component
{
private:
    //cached value, updated when fov,near,far or viewport size changes
    glm::mat4 projectionMatrix;

    glm::vec2 viewportSize={1,1};

    //How the viewport is mapped to the screen
    //viewport coordinates and size is given relative to window size, so 1 means 100% and 0 is 0 percent
    /**
     * Left bottom corner of viewport
     */
    glm::vec2 screenViewportPos={0,0};
    glm::vec2 screenViewportSize={1,1};
    /**
     * Field of View for the y axis in radians
     */
    float fov=1;
    float nearPlane=0.1f;
    float farPlane=100;

    /**
     * The frame buffer this camera is rendering to
     * if nullptr, then the default framebuffer is used
     */
    FramebufferAsset* framebuffer = nullptr;

    void updateProjectionMatrix();
public:

    const glm::mat4& getProjectionMatrix() const;

    const glm::vec2 &getViewportSize() const;

    void setViewportSize(const glm::vec2 &viewportSize);

    const glm::vec2& getScreenViewportPos() const;
    void setScreenViewportPos(const glm::vec2 &viewportPos);
    const glm::vec2& getScreenViewportSize() const;
    void setScreenViewportSize(const glm::vec2 &viewportSize);
    float getFov() const;
    void setFov(float fov);
    float getNear() const;
    void setNear(float nearPlane);
    float getFar() const;
    void setFar(float farPlane);

    FramebufferAsset *getFramebuffer() const;

    void setFramebuffer(FramebufferAsset *framebuffer);
};


#endif //CGUE19_THECHOSENFROG_CAMERA_H
