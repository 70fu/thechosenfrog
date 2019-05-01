#ifndef CGUE19_THECHOSENFROG_CONSTANTS_H
#define CGUE19_THECHOSENFROG_CONSTANTS_H

//file containing static constants
static constexpr float FIXED_DELTA = 1.0/60.0;
static constexpr float HALF_FIXED_DELTA=1.0/120.0;
static constexpr float FIXED_DELTA_SQ = FIXED_DELTA*FIXED_DELTA;
static constexpr double UPDATE_TIME_ACCUM_MAX = 8*FIXED_DELTA;
static constexpr unsigned short MAX_ENTITIES = 256;

static constexpr double PI = 3.14159265358979323846;
static constexpr double TO_RADIANS = PI / 180;

#endif //CGUE19_THECHOSENFROG_CONSTANTS_H
