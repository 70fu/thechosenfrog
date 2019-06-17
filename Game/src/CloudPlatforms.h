#ifndef CGUE19_THECHOSENFROG_CLOUDPLATFORMS_H
#define CGUE19_THECHOSENFROG_CLOUDPLATFORMS_H


#include <geometry/PxTriangleMesh.h>
#include "components/TransformComponent.h"
#include "components/CloudComponent.h"

enum CloudType {
    SQUARE,
    CIRCLE
};

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
private:
    static constexpr float SPHERE_DIST = 0.6f;
    static constexpr float SPHERE_RADIUS = 0.5f;
    static constexpr float SQUARE_PLATFORM_SIZE = SPHERE_DIST*3;
    static constexpr unsigned int CIRCLE_CLOUD_RING_COUNT = 2;
    static constexpr float CIRCLE_CLOUD_RADIUSES[] = {0.975,0.38};
    static constexpr unsigned int CIRCLE_CLOUD_SPHERE_COUNTS[] = {11,5};
    CloudRenderData::SphereData squareCloudOffsets[CloudRenderData::SPHERE_COUNT];
    CloudRenderData::SphereData circleCloudOffsets[CloudRenderData::SPHERE_COUNT];

    physx::PxTriangleMesh* cylinder=nullptr;

    bool initialized = false;

    Game* game = nullptr;
public:

    CloudPlatforms()=default;
    ~CloudPlatforms();

    void init(Game& game);

    CloudPlatforms(const CloudPlatforms&) = delete;
    CloudPlatforms& operator=(const CloudPlatforms&) = delete;

    //these methods should not be called before initialization
    TransformComponent& makeCloudPlatform(Game &game, CloudPlatformParameter params,CloudType type);
};


#endif //CGUE19_THECHOSENFROG_CLOUDPLATFORMS_H
