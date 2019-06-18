
#ifndef CGUE19_THECHOSENFROG_CLOUDGEN_H
#define CGUE19_THECHOSENFROG_CLOUDGEN_H

#include <vec3.hpp>
#include "util/Color.h"
#include <ext/vector_int2.hpp>

enum CloudType {
    SQUARE,
    CIRCLE
};

struct CloudPlatformParameter {
    glm::vec3 translation={0,0,0};
    glm::vec3 scale={1,1,1};
    float yRotation = 0;
    glm::ivec2 size={1,1};//x and z size
    Color color={255,255,255,255};
    CloudType type=SQUARE;
    bool isCheckpoint = false;
};

struct CloudGeneratorParameter {
    float cloudAngleDeviation=0.5;//in %
    float fromPointRadiusMin = 0.75;
    float fromPointRadiusMax = 0.95;
    float heightTillEnd = 60;//relative to start
    float jumpStrengthMin = 0.5;
    float jumpStrengthMax = 1;
};

#endif //CGUE19_THECHOSENFROG_CLOUDGEN_H
