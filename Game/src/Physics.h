#ifndef CGUE19_THECHOSENFROG_PHYSICS_H
#define CGUE19_THECHOSENFROG_PHYSICS_H


#include <foundation/PxErrorCallback.h>

namespace physx
{
    class PxFoundation;
    class PxPhysics;
    class PxScene;
    class PxDefaultCpuDispatcher;
    class PxControllerManager;
    class PxMaterial;
    class PxCooking;
    class PxTolerancesScale;
}

class Physics : public physx::PxErrorCallback
{
private:
    physx::PxFoundation* foundation;
    physx::PxPhysics* physics;
    physx::PxScene* scene;
    physx::PxDefaultCpuDispatcher* cpuDispatcher;
    physx::PxControllerManager* controllerManager;
    //material, with all values set to 0
    physx::PxMaterial* nullMaterial;
    physx::PxCooking* cooking;
public:
    void init();
    void step();
    void cleanup();

    physx::PxPhysics *getPhysics() const;

    physx::PxScene* getScene() const;
    physx::PxControllerManager* getControllerManager() const;
    physx::PxCooking* getCooking() const;
    physx::PxMaterial* getNullMaterial() const;
    //TODO dont return by value
    physx::PxTolerancesScale getTolerancesScale() const;

    void reportError(physx::PxErrorCode::Enum code, const char *message, const char *file, int line) override;
};


#endif //CGUE19_THECHOSENFROG_PHYSICS_H
