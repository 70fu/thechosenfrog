#ifndef CGUE19_THECHOSENFROG_CLOUDPLATFORMS_H
#define CGUE19_THECHOSENFROG_CLOUDPLATFORMS_H


#include "components/TransformComponent.h"
#include "components/CloudComponent.h"

struct CloudPlatformParameter {
    glm::vec3 translation={0,0,0};
    glm::vec3 scale={1,1,1};
    float yRotation = 0;
    glm::ivec2 size={1,1};//x and z size
};

/**
 * Generator class for individual platforms
 * This is not written beautifully (many hardcoded values, ...), no time :(
 */
class CloudPlatforms
{

    enum CloudType {
        SQUARE
    };

private:
    static constexpr float SPHERE_DIST = 0.6f;
    static constexpr float SPHERE_RADIUS = 0.5f;
    static constexpr float SQUARE_PLATFORM_SIZE = SPHERE_DIST*3;
    CloudRenderData::SphereData squareCloudOffsets[CloudRenderData::SPHERE_COUNT];
public:

    CloudPlatforms();

    TransformComponent& makeSquaredCloudPlatform(Game& game,CloudPlatformParameter params);
};


#endif //CGUE19_THECHOSENFROG_CLOUDPLATFORMS_H
