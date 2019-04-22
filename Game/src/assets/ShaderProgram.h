#ifndef CGUE19_THECHOSENFROG_SHADER_PROGRAM_H
#define CGUE19_THECHOSENFROG_SHADER_PROGRAM_H


#include <string>
#include <unordered_map>
#include <vector>
#include "Shader.h"

/**
 * Uniforms, every shader program should be able to accept
 */
namespace CommonShaderUniforms
{
    enum CommonShaderUniforms : unsigned char
    {
        MODEL_MAT,//mat4
        NORMAL_MAT,//mat4
        PROJECTION_VIEW_MATRIX, //mat4
        VIEWER_POS //vec3
    };
}

/**
 * Class, that manages a shader program (linking and such) on the graphics card
 * Usage:
 * 1. add Shaders
 * 2. link()
 * 3. Do sth with ShaderProgram (getUniformLocation(...) or/and getProgramHandle())
 * 4. cleanup
 * 5. Repeat
 */
class ShaderProgram
{
private:
    bool linked = false;

    std::vector<Shader*> shaders;

    GLuint shaderProgramHandle;

    std::unordered_map<std::string,GLint> uniformLocations;

public:
    ShaderProgram()=default;
    ShaderProgram(std::initializer_list<Shader*> shaders);

    bool link();
    /**
     * Relink shader program with the same shaders
     * @return
     */
    bool relink();
    void cleanup();

    GLint getUniformLocation(const std::string& name);

    void addShader(Shader* shader);
    bool hasShader(Shader* shader);

    const GLuint& getProgramHandle() const;

};


#endif //CGUE19_THECHOSENFROG_SHADER_PROGRAM_H
