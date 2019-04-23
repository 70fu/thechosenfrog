
#ifndef CGUE19_THECHOSENFROG_SHADERIDS_H
#define CGUE19_THECHOSENFROG_SHADERIDS_H

#include <RuntimeInclude.h>
RUNTIME_MODIFIABLE_INCLUDE

/**
 * Contains Ids of shader assets
 */
namespace ShaderIds
{
    enum ShaderIds : unsigned char
    {
        DEFAULT,
        DEFAULT_VERT,
        DEFAULT_FRAG,
		SKYBOX_FRAG,
		SKYBOX_VERT,

        SHADER_COUNT
    };
}

#endif //CGUE19_THECHOSENFROG_SHADERIDS_H
