#include <gtc/matrix_transform.hpp>
#include "CameraComponent.h"


void CameraComponent::updateProjectionMatrix()
{
    projectionMatrix = glm::perspectiveFov(fov, viewportSize.x, viewportSize.y,nearPlane,farPlane);
}

const glm::mat4 &CameraComponent::getProjectionMatrix() const
{
    return projectionMatrix;
}

const glm::vec2 &CameraComponent::getScreenViewportPos() const
{
    return screenViewportPos;
}

void CameraComponent::setScreenViewportPos(const glm::vec2 &viewportPos)
{
    screenViewportPos = viewportPos;
}

const glm::vec2 &CameraComponent::getScreenViewportSize() const
{
    return screenViewportSize;
}

void CameraComponent::setScreenViewportSize(const glm::vec2 &viewportSize)
{
    screenViewportSize = viewportSize;
}

float CameraComponent::getFov() const
{
    return fov;
}

void CameraComponent::setFov(float newFov)
{
    fov=newFov;
    updateProjectionMatrix();
}

float CameraComponent::getNear() const
{
    return nearPlane;
}

void CameraComponent::setNear(float newNear)
{
    nearPlane=newNear;
    updateProjectionMatrix();
}

float CameraComponent::getFar() const
{
    return farPlane;
}

void CameraComponent::setFar(float newFar)
{
    farPlane=newFar;
    updateProjectionMatrix();
}

const glm::vec2 &CameraComponent::getViewportSize() const
{
    return viewportSize;
}

void CameraComponent::setViewportSize(const glm::vec2 &viewportSize)
{
    CameraComponent::viewportSize = viewportSize;
    updateProjectionMatrix();
}

FramebufferAsset *CameraComponent::getFramebuffer() const
{
    return framebuffer;
}

void CameraComponent::setFramebuffer(FramebufferAsset *framebuffer)
{
    CameraComponent::framebuffer = framebuffer;
}
