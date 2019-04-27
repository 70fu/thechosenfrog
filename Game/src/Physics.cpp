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
}

void Physics::step()
{
    scene->simulate(FIXED_DELTA);
    scene->fetchResults(true);//wait for simulation to finish
}

void Physics::cleanup()
{
    PxCloseExtensions();
    physics->release();
    foundation->release();
}

PxScene *Physics::getScene() const
{
    return scene;
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
