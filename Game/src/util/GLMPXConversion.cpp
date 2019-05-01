#include "GLMPXConversion.h"
#include <gtc/type_ptr.hpp>

physx::PxVec3 GLMPXConversion::glmToPx(const glm::vec3 &from)
{
    return physx::PxVec3(from.x,from.y,from.z);
}

physx::PxExtendedVec3 GLMPXConversion::glmToPxExtVec(const glm::vec3 &from)
{
    return physx::PxExtendedVec3(from.x,from.y,from.z);
}

physx::PxVec4 GLMPXConversion::glmToPxVec4(const glm::vec4 &from)
{
    return physx::PxVec4(from.x,from.y,from.z,from.w);
}

physx::PxMat44 GLMPXConversion::glmToPxMat4(const glm::mat4 &from)
{
    return physx::PxMat44(glmToPxVec4(from[0]),glmToPxVec4(from[1]),glmToPxVec4(from[2]),glmToPxVec4(from[3]));//TODO this is probably not safe
}

glm::vec3 GLMPXConversion::pxToGlm(const physx::PxExtendedVec3 &from)
{
    return glm::vec3(from.x,from.y,from.z);
}
