#ifndef CGUE19_THECHOSENFROG_CAMERAUTIL_H
#define CGUE19_THECHOSENFROG_CAMERAUTIL_H


#include <mat4x4.hpp>

namespace CameraUtil
{
    /**
     *  Calculates a camera view matrix from given transformation matrix
     */
    glm::mat4 getViewMatrix(const glm::mat4& transformationMatrix);
};


#endif //CGUE19_THECHOSENFROG_CAMERAUTIL_H
