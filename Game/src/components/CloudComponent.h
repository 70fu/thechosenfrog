#ifndef CGUE19_THECHOSENFROG_CLOUDCOMPONENT_H
#define CGUE19_THECHOSENFROG_CLOUDCOMPONENT_H


#include <vec3.hpp>
#include <vec4.hpp>
#include "Component.h"

/**
 * Render data for a cloud, a cloud only consists of a collection of spheres arranged with the offsets array
 */
struct CloudRenderData
{
public:
    /**
     * A cloud always consists of this many spheres
     */
    static constexpr unsigned int SPHERE_COUNT = 16;

    struct SphereData
    {
        glm::vec3 offset;
        glm::vec4 color;
    };

    SphereData sphereData[SPHERE_COUNT];
};

class CloudComponent : public Component
{
public://TODO should not be public to prevent misuse
    /**
     * never null after initialization
     */
    CloudRenderData* renderData = nullptr;
};


#endif //CGUE19_THECHOSENFROG_CLOUDCOMPONENT_H
