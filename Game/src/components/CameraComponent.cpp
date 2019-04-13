#include <gtc/matrix_transform.hpp>
#include "CameraComponent.h"


void CameraComponent::updateProjectionMatrix()
{
    projectionMatrix = glm::perspectiveFov(fov, static_cast<float>(viewportWidth), static_cast<float>(viewportHeight),near,far);
}

const glm::mat4 &CameraComponent::getProjectionMatrix() const
{
    return projectionMatrix;
}

int CameraComponent::getViewportWidth() const
{
    return viewportWidth;
}

void CameraComponent::setViewportWidth(int newViewportWidth)
{
    viewportWidth = newViewportWidth;
    updateProjectionMatrix();
}

int CameraComponent::getViewportHeight() const
{
    return viewportHeight;
}

void CameraComponent::setViewportHeight(int newViewportHeight)
{
    viewportHeight = newViewportHeight;
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
