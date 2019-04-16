#include <ext/matrix_transform.hpp>
#include "CameraUtil.h"

glm::mat4 CameraUtil::getViewMatrix(const glm::mat4 &transformationMatrix)
{
    return glm::inverse(transformationMatrix);
}

glm::mat4 CameraUtil::calcRotationMatrix(const glm::vec3 &sphericalCoordinates)
{
    return glm::rotate(
            glm::rotate(
                    glm::mat4(1),
                    sphericalCoordinates.z,
                    glm::vec3(0, 1, 0)
            ),
            sphericalCoordinates.y,
            glm::vec3(1, 0, 0)
    );
}

glm::mat4 CameraUtil::calcTransformMatrix(const glm::vec3 &sphericalCoordinates, const glm::vec3 &focusPoint)
{
    return glm::translate(
            glm::translate(glm::mat4(1), focusPoint)*calcRotationMatrix(sphericalCoordinates),
            glm::vec3(0, 0, sphericalCoordinates.x)
    );
}
