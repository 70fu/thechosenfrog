#include "CameraUtil.h"

glm::mat4 CameraUtil::getViewMatrix(const glm::mat4 &transformationMatrix)
{
    return glm::inverse(transformationMatrix);
}
