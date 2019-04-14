#include <gtc/matrix_transform.hpp>
#include "CameraComponent.h"


void CameraComponent::updateProjectionMatrix()
{
    projectionMatrix = glm::perspectiveFov(fov, viewportSize.x, viewportSize.y,near,far);
}

const glm::mat4 &CameraComponent::getProjectionMatrix() const
{
    return projectionMatrix;
}

const glm::vec2 &CameraComponent::getViewportPos() const
{
    return viewportPos;
}

void CameraComponent::setViewportPos(const glm::vec2& newViewportPos)
{
    viewportPos = newViewportPos;
}

const glm::vec2 &CameraComponent::getViewportSize() const
{
    return viewportSize;
}

void CameraComponent::setViewportSize(const glm::vec2& newViewportSize)
{
    viewportSize = newViewportSize;
    updateProjectionMatrix();
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
    return near;
}

void CameraComponent::setNear(float newNear)
{
    near=newNear;
    updateProjectionMatrix();
}

float CameraComponent::getFar() const
{
    return far;
}

void CameraComponent::setFar(float newFar)
{
    far=newFar;
    updateProjectionMatrix();
}
