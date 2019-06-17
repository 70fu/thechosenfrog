#include "CloudPlatforms.h"
#include "Game.h"
#include <PxPhysicsAPI.h>
#include "util/GLMPXConversion.h"

CloudPlatforms::~CloudPlatforms()
{
    if(initialized)
    {
        cylinder->release();
    }
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
        physx::PxCookingParams params(game.getPhysics().getTolerancesScale());
// disable mesh cleaning - perform mesh validation on development configurations
        params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
// disable edge precompute, edges are set for each triangle, slows contact generation
        params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
// lower hierarchy for internal mesh

        game.getPhysics().getCooking()->setParams(params);

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
        meshDesc.points.data            = verts;

        meshDesc.triangles.count        = cylinderMesh.surface.indices.size()/3;
        meshDesc.triangles.stride       = 3*sizeof(physx::PxU32);
        meshDesc.triangles.data         = indices;

        delete[] verts;
        delete[] indices;

        cylinder = game.getPhysics().getCooking()->createTriangleMesh(meshDesc,game.getPhysics().getPhysics()->getPhysicsInsertionCallback());
    }

}

TransformComponent& CloudPlatforms::makeCloudPlatform(Game &game, CloudPlatformParameter params,CloudType type)
{
    EntityId id = game.createEntity();
    TransformComponent &transform = game.transformComps.addComp(id);
    transform.setTranslation(params.translation);
    //transform.setScaling({params.size.x, 0.1f, params.size.y});
    //transform.setRotation({0,params.yRotation,0});

    PhysicsComponent &pComp = game.physicsComps.addComp(id);
    physx::PxRigidActor *actor=nullptr;
    if(type==CloudType::SQUARE)
        actor = physx::PxCreateStatic(*game.getPhysics().getPhysics(),physx::PxTransform(GLMPXConversion::glmToPx(transform.getTranslation())),physx::PxBoxGeometry((2*SPHERE_RADIUS-SPHERE_DIST+(SQUARE_PLATFORM_SIZE+SPHERE_DIST)*params.size.x*params.scale.x)/2,0.05,(2*SPHERE_RADIUS-SPHERE_DIST+(SQUARE_PLATFORM_SIZE+SPHERE_DIST)*params.size.y*params.scale.z)/2),*game.getPhysics().getNullMaterial());
    else if(type==CloudType::CIRCLE)
    {
        static constexpr float BASE_RADIUS = CIRCLE_CLOUD_RADIUSES[0]+SPHERE_RADIUS;
        physx::PxMeshScale scale(GLMPXConversion::glmToPx(params.scale*glm::vec3(BASE_RADIUS,SPHERE_RADIUS,BASE_RADIUS)), physx::PxQuat(physx::PxIdentity));
        physx::PxTriangleMeshGeometry geom(cylinder,scale);
        actor = physx::PxCreateStatic(*game.getPhysics().getPhysics(),physx::PxTransform(GLMPXConversion::glmToPx(transform.getTranslation())),geom,*game.getPhysics().getNullMaterial());
    }
    game.getPhysics().getScene()->addActor(*actor);
    pComp.setActor(actor);
    pComp.setLayerAndCollisionMask(PhysicsComponent::PLATFORM, PhysicsComponent::ALL);

    if(type==CloudType::SQUARE)
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
                std::copy(std::begin(squareCloudOffsets), std::end(squareCloudOffsets),std::begin(cloudComp.renderData->sphereData));

                for(auto it = std::begin(cloudComp.renderData->sphereData);it!=std::end(cloudComp.renderData->sphereData);++it)
                    it->color = glm::vec4(params.color)/255.0f;
            }
        }
    }
    else if(type==CloudType::CIRCLE)
    {
        EntityId cloud = game.createEntity();
        TransformComponent& cloudTransform = game.transformComps.addComp(cloud);
        cloudTransform.setScaling(params.scale);
        cloudTransform.setParent(transform,false);

        CloudComponent &cloudComp = game.cloudComps.addComp(cloud);
        std::copy(std::begin(circleCloudOffsets), std::end(circleCloudOffsets),std::begin(cloudComp.renderData->sphereData));
        for(auto it = std::begin(cloudComp.renderData->sphereData);it!=std::end(cloudComp.renderData->sphereData);++it)
            it->color = glm::vec4(params.color)/255.0f;
    }

    return transform;
}
