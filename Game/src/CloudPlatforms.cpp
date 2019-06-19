#include "CloudPlatforms.h"
#include "Game.h"
#include <PxPhysicsAPI.h>
#include <gtx/rotate_vector.hpp>
#include "util/GLMPXConversion.h"
#include <ctime>


glm::vec3 CloudPlatforms::getSquareCloudSize(CloudPlatformParameter params)
{
    return {2*SPHERE_RADIUS-SPHERE_DIST+(SQUARE_PLATFORM_SIZE+SPHERE_DIST)*params.size.x*params.scale.x,
        0,
        2*SPHERE_RADIUS-SPHERE_DIST+(SQUARE_PLATFORM_SIZE+SPHERE_DIST)*params.size.y*params.scale.z};
}

void CloudPlatforms::init(Game &game)
{
    if(initialized)
        return;
    initialized = true;
    CloudPlatforms::game=&game;

    {
        for(int x = 0;x<4;++x)
        {
            for(int y = 0;y<4;++y)
            {
                squareCloudOffsets[x*4+y]={{x*SPHERE_DIST-0.9f,0,y*SPHERE_DIST-0.9f},{1,1,1,1}};
            }
        }
    }

    {
        int sIndex = 0;
        for(int ring = 0;ring<CIRCLE_CLOUD_RING_COUNT;++ring)
        {
            float angle = 2.0f*PI/CIRCLE_CLOUD_SPHERE_COUNTS[ring];
            float radius = CIRCLE_CLOUD_RADIUSES[ring];
            for(int i = 0;i<CIRCLE_CLOUD_SPHERE_COUNTS[ring];++i)
            {
                circleCloudOffsets[sIndex++]={{radius*std::cos(angle*i),0,radius*std::sin(angle*i)},{1,1,1,1}};
            }
        }

        for(;sIndex<CloudRenderData::SPHERE_COUNT;++sIndex)
        {
            circleCloudOffsets[sIndex] = {{0,0,0},{0,0,0,0}};
        }
    }

    //init cylinder collider
    {
        //generate mesh
        MeshAsset& cylinderMesh = *game.getAssetManager().getMesh(MeshIds::UNIT_CYLINDER);
        physx::PxVec3* verts = new physx::PxVec3[cylinderMesh.surface.positions.size()];
        for(int i = 0;i<cylinderMesh.surface.positions.size();++i)
            verts[i] = GLMPXConversion::glmToPx(cylinderMesh.surface.positions[i]);
        physx::PxU32* indices = new physx::PxU32[cylinderMesh.surface.indices.size()];
        for(int i = 0;i<cylinderMesh.surface.indices.size();++i)
            indices[i] = cylinderMesh.surface.indices[i];

        physx::PxTriangleMeshDesc meshDesc;
        meshDesc.points.count           = cylinderMesh.surface.positions.size();
        meshDesc.points.stride          = sizeof(physx::PxVec3);
        meshDesc.points.data            = &verts[0];

        meshDesc.triangles.count        = cylinderMesh.surface.indices.size()/3;
        meshDesc.triangles.stride       = 3*sizeof(physx::PxU32);
        meshDesc.triangles.data         = &indices[0];


        ImGuiAl::Log::getInstance().Debug("Mesh Desc Valid: %d",meshDesc.isValid());
        ImGuiAl::Log::getInstance().Debug("Triangle Mesh Validation: %d",game.getPhysics().getCooking()->validateTriangleMesh(meshDesc));

        physx::PxTriangleMeshCookingResult::Enum result;
        cylinder = game.getPhysics().getCooking()->createTriangleMesh(meshDesc,game.getPhysics().getPhysics()->getPhysicsInsertionCallback(),&result);
        ImGuiAl::Log::getInstance().Debug("Cooking result: %d",result);

        delete[] verts;
        delete[] indices;
    }

}

void CloudPlatforms::cleanup(Game &game)
{
    if(initialized)
    {
        cylinder->release();
    }
}

void CloudPlatforms::setParameter(const CloudGeneratorParameter &generatorParameter)
{
    genParams = generatorParameter;

    //setup random engine and distributions
    rEngine.seed(std::time(0));
    rFloat = std::uniform_real_distribution<float>(0,1);
    rCloudAngleVariance = std::normal_distribution<float>(0,genParams.cloudAngleDeviation);
    rAngle = std::uniform_real_distribution<float>(0,2*PI);
    rCloudParabolaPlacement = std::uniform_real_distribution<float>(0.5f,1);
    rBool = std::uniform_int_distribution<int>(0,1);
    rFromPointRadius = std::uniform_real_distribution<float>(generatorParameter.fromPointRadiusMin,generatorParameter.fromPointRadiusMax);
    rJumpStrength = std::uniform_real_distribution<float>(generatorParameter.jumpStrengthMin,generatorParameter.jumpStrengthMax);
}

TransformComponent& CloudPlatforms::makeCloudPlatform(Game &game, CloudPlatformParameter params)
{
    EntityId id = game.createEntity();
    TransformComponent &transform = game.transformComps.addComp(id);
    transform.setTranslation(params.translation);
    //transform.setScaling({params.size.x, 0.1f, params.size.y});
    //transform.setRotation({0,params.yRotation,0});

    PhysicsComponent &pComp = game.physicsComps.addComp(id);
    physx::PxRigidActor *actor=nullptr;
    if(params.type==CloudType::SQUARE)
    {
        glm::vec3 size = getSquareCloudSize(params);
        actor = physx::PxCreateStatic(*game.getPhysics().getPhysics(),
                                      physx::PxTransform(GLMPXConversion::glmToPx(transform.getTranslation())),
                                      physx::PxBoxGeometry(size.x / 2, 0.05,
                                                           size.z / 2), *game.getPhysics().getNullMaterial());
    }
    else if(params.type==CloudType::CIRCLE)
    {
        static constexpr float BASE_RADIUS = CIRCLE_CLOUD_RADIUSES[0]+SPHERE_RADIUS;
        physx::PxMeshScale scale(GLMPXConversion::glmToPx(params.scale*glm::vec3(BASE_RADIUS,SPHERE_RADIUS,BASE_RADIUS)), physx::PxQuat(physx::PxIdentity));
        physx::PxTriangleMeshGeometry geom(cylinder,scale);
        actor = physx::PxCreateStatic(*game.getPhysics().getPhysics(),physx::PxTransform(GLMPXConversion::glmToPx(transform.getTranslation())),geom,*game.getPhysics().getNullMaterial());
    }
    game.getPhysics().getScene()->addActor(*actor);
    pComp.setActor(actor);
    pComp.setLayerAndCollisionMask(PhysicsComponent::PLATFORM, PhysicsComponent::ALL);

    if(params.type==CloudType::SQUARE)
    {
        for(int x = 0;x<params.size.x;++x)
        {
            for(int y = 0;y<params.size.y;++y)
            {
                EntityId cloud = game.createEntity();
                TransformComponent& cloudTransform = game.transformComps.addComp(cloud);
                cloudTransform.setTranslation({(SQUARE_PLATFORM_SIZE+SPHERE_DIST)*params.scale.x*(x-(params.size.x-1)/2.0f),0,(SQUARE_PLATFORM_SIZE+SPHERE_DIST)*params.scale.z*(y-(params.size.y-1)/2.0f)});
                cloudTransform.setScaling(params.scale);
                cloudTransform.setParent(transform,false);

                CloudComponent &cloudComp = game.cloudComps.addComp(cloud);
                cloudComp.isCheckpoint = params.isCheckpoint;
                std::copy(std::begin(squareCloudOffsets), std::end(squareCloudOffsets),std::begin(cloudComp.renderData->sphereData));

                for(auto it = std::begin(cloudComp.renderData->sphereData);it!=std::end(cloudComp.renderData->sphereData);++it)
                    it->color = glm::vec4(params.color)/255.0f;
            }
        }
    }
    else if(params.type==CloudType::CIRCLE)
    {
        EntityId cloud = game.createEntity();
        TransformComponent& cloudTransform = game.transformComps.addComp(cloud);
        cloudTransform.setScaling(params.scale);
        cloudTransform.setParent(transform,false);

        CloudComponent &cloudComp = game.cloudComps.addComp(cloud);
        cloudComp.isCheckpoint = params.isCheckpoint;
        std::copy(std::begin(circleCloudOffsets), std::end(circleCloudOffsets),std::begin(cloudComp.renderData->sphereData));
        for(auto it = std::begin(cloudComp.renderData->sphereData);it!=std::end(cloudComp.renderData->sphereData);++it)
            it->color = glm::vec4(params.color)/255.0f;
    }

    return transform;
}

void CloudPlatforms::generateCloudPlatforms(Game& game, const TransformComponent& from,CloudPlatformParameter fromParams, unsigned int cloudAmount, const CharControllerConfiguration& charConf)
{
    if(cloudAmount==0)
        return;

    float angleStep = 2.0f*PI/cloudAmount;
    float angleShift = 2.0f*PI*rFloat(rEngine);
    for(int i = 0;i<cloudAmount;++i)
    {
        float angle = angleStep*i+angleStep*rCloudAngleVariance(rEngine)+angleShift;
        generateCloudPath(game,from,fromParams,charConf,angle,fromParams.translation.y+genParams.heightTillEnd);
    }
}

void CloudPlatforms::generateCloudPath(Game &game, const TransformComponent &from, CloudPlatformParameter fromParams,
                                       const CharControllerConfiguration &charConf, float angle,float maxHeight)
{
    //direction where cloud is placed relative to from platform
    glm::vec3 direction = glm::rotateY(glm::vec3(1,0,0),angle);

    //choose point to jump from
    glm::vec3 fromPos;
    if(fromParams.type==CloudType::CIRCLE)
    {
        fromPos = fromParams.translation + direction * CIRCLE_CLOUD_RADIUSES[0]*std::min(fromParams.scale.x,fromParams.scale.z) * rFromPointRadius(rEngine);
    }
    else if(fromParams.type==CloudType::SQUARE)
    {
        glm::vec3 size = getSquareCloudSize(fromParams);
        float a = angle/(PI/2);
        a = (a-std::floor(a))*PI/2;
        float xRadius = (size.x/2)/std::cos(a);
        float zRadius = (size.z/2)/std::cos(PI/2-a);
        float radius = std::min(xRadius,zRadius);
        fromPos = fromParams.translation + direction * radius * rFromPointRadius(rEngine);
    }

    //choose a possible jump parabola
    float jumpStrength = rJumpStrength(rEngine);
    float distanceHeightRatio = rFloat(rEngine)*rFloat(rEngine);//"square" for more far jumps
    float jumpDistance;
    float jumpHeight;
    float jumpDuration;
    CharControllerComponent::calculateJump(jumpStrength,distanceHeightRatio,charConf,jumpDistance,jumpHeight,jumpDuration);
    glm::vec3 jump = CharControllerComponent::calculateGravityAndSpeed(jumpDistance,jumpHeight,jumpDuration);
    glm::vec2 parabola = CharControllerComponent::calculateParabola(jump);

    //choose x value (where on given parabola should the cloud be placed?)
    float parabolaX = jumpDuration*rCloudParabolaPlacement(rEngine);

    //create cloud
    CloudPlatformParameter params;
    glm::vec3 pos = fromPos + direction * (parabolaX / jumpDuration) * jumpDistance +
                         glm::vec3(0, parabola.x * parabolaX * parabolaX + parabola.y * parabolaX, 0);
    if(fromParams.translation.y>maxHeight)
    {
        //generate finishing platform
        params=CHECKPOINT_CLOUD_PARAMETER;
        params.translation = pos;

        makeCloudPlatform(game,params);
    }
    else
    {
        params.type=CloudType::CIRCLE;
        params.translation = pos;
        params.scale = {1.5f, 1, 1.5f};
        TransformComponent &cloud = makeCloudPlatform(game, params);

        //recursion
        generateCloudPath(game, cloud, params,charConf,2*PI*rFloat(rEngine),maxHeight);
    }
}
