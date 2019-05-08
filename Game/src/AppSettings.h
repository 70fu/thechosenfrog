#ifndef CGUE19_THECHOSENFROG_APPSETTINGS_H
#define CGUE19_THECHOSENFROG_APPSETTINGS_H

#include "Constants.h"

struct GameplaySettings
{
    /* --------------------------------------------- */
    // Viewport/FOV settings
    /* --------------------------------------------- */
    enum ViewportMode : char
    {
        STRETCH,
        FIXED_WIDTH,//shows bars on the left and right

        VIEWPORT_MODE_COUNT
    };
    float horFov = 100*TO_RADIANS;//used when fixed width is chosen
    ViewportMode viewportMode=FIXED_WIDTH;

    static constexpr const char* VIEWPORT_MODE_NAMES[VIEWPORT_MODE_COUNT]={"Stretch","Fixed Width"};
};

struct DisplaySettings
{
    int windowWidth=1280;
    int windowHeight=720;
    bool fullScreen=false;
    float brightnessFactor=1;
    //refreshes per second
    int refreshRate = 60;
    //TODO validation
};

struct AppSettings
{
    DisplaySettings display;
    GameplaySettings gameplay;
};


#endif //CGUE19_THECHOSENFROG_APPSETTINGS_H
