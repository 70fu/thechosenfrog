
#ifndef CGUE19_THECHOSENFROG_COMPONENTTYPES_H
#define CGUE19_THECHOSENFROG_COMPONENTTYPES_H

#include <type_traits>
#include "../Constants.h"

namespace Components
{
    enum Types : unsigned char
    {
        //TRANSFORM,
        MESH,

        COMPONENT_COUNT
    };

    /**
     * Used for testing, if entity has a certain combination of components
     */
    //unsigned long is 32 bit at least
    enum MaskBits : unsigned long
    {
        //TRANSFORM_BIT = 1<<TRANSFORM,
        MESH_BIT = 1<<MESH
    };

    using ComponentMask = std::underlying_type<Components::MaskBits>::type;

    /**
     * This is how many entities of a given component type are allocated
     * index corresponds to component type see Types enum
     */
    static constexpr unsigned int MAX_SIZES[] = {
                MAX_ENTITIES, //TRANSFORM
                MAX_ENTITIES, //MESH
            };

    //TODO why is constexpr not possible here?
    MaskBits typeToMask(Types componentType);
}

#endif //CGUE19_THECHOSENFROG_COMPONENTTYPES_H