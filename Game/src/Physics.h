#ifndef CGUE19_THECHOSENFROG_PHYSICS_H
#define CGUE19_THECHOSENFROG_PHYSICS_H


#include <foundation/PxErrorCallback.h>

namespace physx
{
    class PxFoundation;
    class PxPhysics;
    class PxScene;
    class PxDefaultCpuDispatcher;
}

class Physics : public physx::PxErrorCallback
{
private:
    physx::PxFoundation* foundation;
    physx::PxPhysics* physics;
    physx::PxScene* scene;
    physx::PxDefaultCpuDispatcher* cpuDispatcher;
public:
    void init();
    void step();
    void cleanup();

    physx::PxScene* getScene() const;

    void reportError(physx::PxErrorCode::Enum code, const char *message, const char *file, int line) override;
};


#endif //CGUE19_THECHOSENFROG_PHYSICS_H
