
#ifndef CGUE19_THECHOSENFROG_SHADERPROGRAMIDS_H
#define CGUE19_THECHOSENFROG_SHADERPROGRAMIDS_H

#include <RuntimeInclude.h>
RUNTIME_MODIFIABLE_INCLUDE

/**
 * Contains Ids of Shader program assets
 */
namespace ShaderProgramIds
{
    enum ShaderProgramIds : unsigned char
    {
        DEFAULT,
		SKYBOX,
        UNLIT,
        FONT,
        PARABOLA,
        FBO,
        FBO_POST,//fbo shader with post processing
        CLOUD,

        SHADER_PROGRAM_COUNT
    };
}
#endif //CGUE19_THECHOSENFROG_SHADERPROGRAMIDS_H
