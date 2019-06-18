#ifndef CGUE19_THECHOSENFROG_CONSTANTS_H
#define CGUE19_THECHOSENFROG_CONSTANTS_H


#include "CharControllerConfiguration.h"
#include "util/Color.h"
#include "CloudGen.h"

//file containing static constants
static constexpr float FIXED_DELTA = 1.0/60.0;
static constexpr float HALF_FIXED_DELTA=1.0/120.0;
static constexpr float FIXED_DELTA_SQ = FIXED_DELTA*FIXED_DELTA;
static constexpr double UPDATE_TIME_ACCUM_MAX = 8*FIXED_DELTA;
static constexpr unsigned short MAX_ENTITIES = 1024;

static constexpr double PI = 3.14159265358979323846;
static constexpr double TO_RADIANS = PI / 180;
static constexpr double TO_DEGREES = 180/PI;

//gameplay constants
//clouds below this threshold disappear
static constexpr float CLOUD_DISAPPEAR_DIST = 5;
static constexpr CharControllerConfiguration PLAYER_CONFIG =
        {
            15,     //groundWalkForce
            1,      //airWalkSpeed
            2,      //minJumpDistance
            20,     //maxJumpDistance
            0.5f,   //minJumpHeight
            10,     //maxJumpHeight
            0.5f,   //minJumpDuration
            2,      //maxJumpDuration
            3,      //maxGroundSpeed
            1,      //maxDistLookingFactor
            0.1f,   //minDistLookingFactor
            1,      //maxHeightLookingFactor
            0.1f,   //minHeightLookingFactor
        };
static const Color CHECKPOINT_CLOUD_COLOR = {200,30,30,255};
static const CloudPlatformParameter CHECKPOINT_CLOUD_PARAMETER =
        {
                {0,0,0},    //translation
                {1,1,1},    //scale
                0,          //rotation
                {3,3},      //size
                CHECKPOINT_CLOUD_COLOR, //color
                CloudType::SQUARE,  //type
                true,       //isCheckpoint
        };

#endif //CGUE19_THECHOSENFROG_CONSTANTS_H
