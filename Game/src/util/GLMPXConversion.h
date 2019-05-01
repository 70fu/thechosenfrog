#ifndef CGUE19_THECHOSENFROG_GLMPXCONVERSION_H
#define CGUE19_THECHOSENFROG_GLMPXCONVERSION_H


//Functions to convert between glm and px vector and matrix types

#include <foundation/PxVec3.h>
#include <vec3.hpp>
#include <characterkinematic/PxExtended.h>

namespace GLMPXConversion
{
    physx::PxVec3 glmToPx(const glm::vec3 &from);
    physx::PxExtendedVec3 glmToPxExtVec(const glm::vec3 &from);

    glm::vec3 pxToGlm(const physx::PxExtendedVec3& from);
}

#endif //CGUE19_THECHOSENFROG_GLMPXCONVERSION_H
