#include "Physics.h"
#include "logger/imguial_log.h"
#include "Constants.h"
#include <PxPhysicsAPI.h>

using namespace physx;
using namespace ImGuiAl;



void Physics::init()
{
    static physx::PxDefaultAllocator gDefaultAllocatorCallback;
    foundation = PxCreateFoundation(PX_PHYSICS_VERSION,gDefaultAllocatorCallback,*this);
    if(!foundation)
        Log::getInstance().Error("Could not initialize physx foundation");

    physics = PxCreatePhysics(PX_PHYSICS_VERSION,*foundation,PxTolerancesScale(),true);//TODO create tolerance scale suitable for the game
    if(!physics)
        Log::getInstance().Error("Could not initialize physx physics object");

    if(!PxInitExtensions(*physics,nullptr))
        Log::getInstance().Error("Could not initilize physx extensions");

    PxSceneDesc desc=PxSceneDesc(PxTolerancesScale());
    desc.filterShader = PxDefaultSimulationFilterShader;
    //TODO edit scene desc
    cpuDispatcher = PxDefaultCpuDispatcherCreate(4);//TODO get number of cpu threads
    if(!cpuDispatcher)
        Log::getInstance().Error("Could not initialize physx cpu dispatcher");
    desc.cpuDispatcher = cpuDispatcher;

    scene = physics->createScene(desc);

    //create controller manager
    controllerManager = PxCreateControllerManager(*scene);

    //init null material
    nullMaterial = physics->createMaterial(0,0,0);

    //init cooking
    cooking = PxCreateCooking(PX_PHYSICS_VERSION,*foundation,PxCookingParams(PxTolerancesScale()));
    if(!cooking)
        Log::getInstance().Error("Could not initialize physx Cooking");
}

void Physics::step()
{
    scene->simulate(FIXED_DELTA);
    scene->fetchResults(true);//wait for simulation to finish
}

void Physics::cleanup()
{
    nullMaterial->release();
    controllerManager->release();
    PxCloseExtensions();
    physics->release();
    cooking->release();
    foundation->release();
}

PxScene *Physics::getScene() const
{
    return scene;
}

physx::PxControllerManager *Physics::getControllerManager() const
{
    return controllerManager;
}

physx::PxCooking *Physics::getCooking() const
{
    return cooking;
}

physx::PxTolerancesScale Physics::getTolerancesScale() const
{
    return PxTolerancesScale();
}

physx::PxMaterial *Physics::getNullMaterial() const
{
    return nullMaterial;
}

void Physics::reportError(PxErrorCode::Enum code, const char *message, const char *file, int line)
{
    if(code==PxErrorCode::eDEBUG_INFO)
        Log::getInstance().Debug("%s (%d): %s",file,line,message);
    else if(code==PxErrorCode::eDEBUG_WARNING)
        Log::getInstance().Warn("%s (%d): %s",file,line,message);
    else
        Log::getInstance().Error("%s (%d): %s",file,line,message);
}

PxPhysics *Physics::getPhysics() const
{
    return physics;
}
