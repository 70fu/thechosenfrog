#ifndef CGUE19_THECHOSENFROG_CHARCONTROLLERCONFIGURATION_H
#define CGUE19_THECHOSENFROG_CHARCONTROLLERCONFIGURATION_H


struct CharControllerConfiguration
{
    //speeds are always given in units/s
    float groundWalkForce = 0;
    float airWalkSpeed = 0;
    float minJumpDistance = 0;
    float maxJumpDistance = 0;
    float minJumpHeight = 0;
    float maxJumpHeight = 0;
    float minJumpDuration = 0;
    float maxJumpDuration = 0;
    float maxGroundSpeed = 0;

    //see doc on distanceHeightRatio
    float maxDistLookingFactor = 1;
    float minDistLookingFactor = 1;
    float maxHeightLookingFactor = 1;
    float minHeightLookingFactor = 1;
};


#endif //CGUE19_THECHOSENFROG_CHARCONTROLLERCONFIGURATION_H
