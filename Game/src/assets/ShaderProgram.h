#ifndef CGUE19_THECHOSENFROG_SHADER_PROGRAM_H
#define CGUE19_THECHOSENFROG_SHADER_PROGRAM_H


#include <string>
#include <unordered_map>
#include <vector>
#include "Shader.h"

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
    void cleanup();

    GLint getUniformLocation(const std::string& name);

    void addShader(Shader* shader);
    bool hasShader(Shader* shader);

    const GLuint& getProgramHandle() const;

};


#endif //CGUE19_THECHOSENFROG_SHADER_PROGRAM_H
