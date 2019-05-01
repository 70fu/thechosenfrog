#include "GLMPXConversion.h"

physx::PxVec3 GLMPXConversion::glmToPx(const glm::vec3 &from)
{
    return physx::PxVec3(from.x,from.y,from.z);
}

physx::PxExtendedVec3 GLMPXConversion::glmToPxExtVec(const glm::vec3 &from)
{
    return physx::PxExtendedVec3(from.x,from.y,from.z);
}

glm::vec3 GLMPXConversion::pxToGlm(const physx::PxExtendedVec3 &from)
{
    return glm::vec3(from.x,from.y,from.z);
}
