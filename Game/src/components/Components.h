
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
		CAMERA,
		CAMERA_CONTROLLER,
		PHYSICS,
		CHAR_CONTROLLER,
		PLAYER,
		TEXT,
		CLOUD,

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
		MATERIAL_BIT = 1 << MATERIAL,
		CAMERA_BIT = 1<<CAMERA,
		CAMERA_CONTROLLER_BIT = 1<<CAMERA_CONTROLLER,
		PHYSICS_BIT = 1<<PHYSICS,
		CHAR_CONTROLLER_BIT=1<<CHAR_CONTROLLER,
		PLAYER_BIT=1<<PLAYER,
		TEXT_BIT=1<<TEXT,
		CLOUD_BIT=1<<CLOUD
    };

    using ComponentMask = std::underlying_type<Components::MaskBits>::type;

    /**
     * This is how many entities of a given component type are allocated
     * index corresponds to component type see Types enum
     */
    static constexpr unsigned int MAX_SIZES[] = {
                MAX_ENTITIES,   //MESH
				MAX_ENTITIES,   //TRANSFORM
				MAX_ENTITIES,   //MATERIAL
				1,              //CAMERA
				2,				//CAMERA_CONTROLLER
				MAX_ENTITIES,   //PHYSICS
				1,              //CHAR_CONTROLLER
                1,              //PLAYER
                16,              //TEXT
                MAX_ENTITIES    //CLOUD
            };

    constexpr MaskBits typeToMask(Types componentType){return static_cast<MaskBits>(1<<componentType);}
}

#endif //CGUE19_THECHOSENFROG_COMPONENTTYPES_H
