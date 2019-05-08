//
// Created by simon on 31.03.19.
//

#ifndef CGUE19_THECHOSENFROG_DEBUGWINDOWIDS_H
#define CGUE19_THECHOSENFROG_DEBUGWINDOWIDS_H

#include <RuntimeInclude.h>
RUNTIME_MODIFIABLE_INCLUDE

/**
 * Ids for different debug windows, should be set such that bitmasks are possible
 */
namespace DebugWindowIds
{
    enum DebugWindowIds : unsigned int
    {
        LOGGER=1<<0,
        COMPONENT_VIEWER=1<<1,
        ASSET_VIEWER=1<<2,
        APP_SETTINGS=1<<3
    };
}

#endif //CGUE19_THECHOSENFROG_DEBUGWINDOWIDS_H
