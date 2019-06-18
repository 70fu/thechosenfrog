#ifndef CGUE19_THECHOSENFROG_CLOUDPLATFORMS_H
#define CGUE19_THECHOSENFROG_CLOUDPLATFORMS_H


#include <geometry/PxTriangleMesh.h>
#include "components/TransformComponent.h"
#include "components/CloudComponent.h"
#include "components/CharControllerComponent.h"
#include <random>

/**
 * Generator class for individual platforms
 * This is not written beautifully (many hardcoded values, ...), no time :(
 */
class CloudPlatforms
{
private:
    static constexpr float SPHERE_DIST = 0.55f;
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

    CloudGeneratorParameter genParams;

    //random number engine and distribution
    std::default_random_engine rEngine;
    std::uniform_real_distribution<float> rFloat;
    std::normal_distribution<float> rCloudAngleVariance;
    std::uniform_real_distribution<float> rAngle;
    std::uniform_real_distribution<float> rCloudParabolaPlacement;
    std::uniform_int_distribution<int> rBool;
    std::uniform_real_distribution<float> rFromPointRadius;
    std::uniform_real_distribution<float> rJumpStrength;

    static glm::vec3 getSquareCloudSize(CloudPlatformParameter params);
    void generateCloudPath(Game& game, const TransformComponent& from,CloudPlatformParameter fromParams,  const CharControllerConfiguration& charConf, float angle,float maxHeight);
public:

    CloudPlatforms()=default;

    void init(Game& game);
    void cleanup(Game& game);
    void setParameter(const CloudGeneratorParameter& generatorParameter);

    CloudPlatforms(const CloudPlatforms&) = delete;
    CloudPlatforms& operator=(const CloudPlatforms&) = delete;

    //these methods should not be called before initialization
    TransformComponent& makeCloudPlatform(Game &game, CloudPlatformParameter params);
    void generateCloudPlatforms(Game& game, const TransformComponent& from,CloudPlatformParameter fromParams, unsigned int cloudAmount, const CharControllerConfiguration& charConf);
};


#endif //CGUE19_THECHOSENFROG_CLOUDPLATFORMS_H
