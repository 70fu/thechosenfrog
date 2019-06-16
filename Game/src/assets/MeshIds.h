
#ifndef CGUE19_THECHOSENFROG_MESHIDS_H
#define CGUE19_THECHOSENFROG_MESHIDS_H

#include <RuntimeInclude.h>
RUNTIME_MODIFIABLE_INCLUDE

/**
 * Contains Ids of mesh assets
 */
namespace MeshIds {
    enum MeshIds : unsigned char {
        DEFAULT,
        UNIT_CUBE,
        SKYBOX,
        SIGNPOST,
        SCREEN_QUAD,
        CLOUD_SPHERE,

        MESH_COUNT
    };
}

#endif //CGUE19_THECHOSENFROG_MESHIDS_H
