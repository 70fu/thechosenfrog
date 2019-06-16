#include "CloudPlatforms.h"
#include "Game.h"
#include <PxPhysicsAPI.h>
#include <PxRigidActor.h>
#include <PxPhysics.h>
#include "util/GLMPXConversion.h"

CloudPlatforms::CloudPlatforms()
{
    {
        for(int x = 0;x<4;++x)
        {
            for(int y = 0;y<4;++y)
            {
                squareCloudOffsets[x*4+y]={{x*SPHERE_DIST-0.9f,0,y*SPHERE_DIST-0.9f},{1,1,1,1}};
            }
        }
    }
}

TransformComponent& CloudPlatforms::makeSquaredCloudPlatform(Game& game,CloudPlatformParameter params)
{
    EntityId id = game.createEntity();
    TransformComponent &transform = game.transformComps.addComp(id);
    transform.setTranslation(params.translation);
    //transform.setScaling({params.size.x, 0.1f, params.size.y});
    //transform.setRotation({0,params.yRotation,0});

    PhysicsComponent &pComp = game.physicsComps.addComp(id);
    physx::PxRigidActor *actor = physx::PxCreateStatic(*game.getPhysics().getPhysics(),physx::PxTransform(GLMPXConversion::glmToPx(transform.getTranslation())),physx::PxBoxGeometry((2*SPHERE_RADIUS-SPHERE_DIST+(SQUARE_PLATFORM_SIZE+SPHERE_DIST)*params.size.x*params.scale.x)/2,0.05,(2*SPHERE_RADIUS-SPHERE_DIST+(SQUARE_PLATFORM_SIZE+SPHERE_DIST)*params.size.y*params.scale.z)/2),*game.getPhysics().getNullMaterial());
    game.getPhysics().getScene()->addActor(*actor);
    pComp.setActor(actor);
    pComp.setLayerAndCollisionMask(PhysicsComponent::PLATFORM, PhysicsComponent::ALL);

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
            std::copy(std::begin(squareCloudOffsets),std::end(squareCloudOffsets),std::begin(cloudComp.renderData->sphereData));
        }
    }

    return transform;
}
