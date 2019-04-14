
#ifndef CGUE19_THECHOSENFROG_COMPONENTTYPES_H
#define CGUE19_THECHOSENFROG_COMPONENTTYPES_H

#include <type_traits>
#include "../Constants.h"

namespace Components
{

	/*
	* Add new types of components here
	*/
    enum Types : unsigned char
    {
        MESH,
		TRANSFORM,
		MATERIAL,

        COMPONENT_COUNT
    };

    /**
     * Used for testing, if entity has a certain combination of components
     */
    //unsigned long is 32 bit at least
    enum MaskBits : unsigned long
    {
        MESH_BIT = 1<<MESH,
		TRANSFORM_BIT = 1 << TRANSFORM,
		MATERIAL_BIT = 1 << MATERIAL
    };

    using ComponentMask = std::underlying_type<Components::MaskBits>::type;

    /**
     * This is how many entities of a given component type are allocated
     * index corresponds to component type see Types enum
     */
    static constexpr unsigned int MAX_SIZES[] = {
                MAX_ENTITIES, //MESH
				MAX_ENTITIES, //TRANSFORM
				MAX_ENTITIES, //MATERIAL
            };

    constexpr MaskBits typeToMask(Types componentType){return static_cast<MaskBits>(1<<componentType);}
}

#endif //CGUE19_THECHOSENFROG_COMPONENTTYPES_H
