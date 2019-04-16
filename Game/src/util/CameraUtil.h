#ifndef CGUE19_THECHOSENFROG_CAMERAUTIL_H
#define CGUE19_THECHOSENFROG_CAMERAUTIL_H


#include <mat4x4.hpp>

namespace CameraUtil
{
    /**
     *  Calculates a camera view matrix from given transformation matrix
     */
    glm::mat4 getViewMatrix(const glm::mat4& transformationMatrix);

    /**
     * @param sphericalCoordinates, x=radius, y=theta (angle between z axis,around x axis), z=phi (angle around y axis)
     * @param focusPoint
     * @return transformation matrix from spherical coordinates
     */
    glm::mat4 calcRotationMatrix(const glm::vec3& sphericalCoordinates);

    /**
     *
     * @param sphericalCoordinates, x=radius, y=theta (angle between z axis,around x axis), z=phi (angle around y axis)
     * @param focusPoint
     * @return transformation matrix from spherical coordinates
     */
    glm::mat4 calcTransformMatrix(const glm::vec3& sphericalCoordinates, const glm::vec3& focusPoint);
};


#endif //CGUE19_THECHOSENFROG_CAMERAUTIL_H
