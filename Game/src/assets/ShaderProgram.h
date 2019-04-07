#ifndef CGUE19_THECHOSENFROG_SHADER_H
#define CGUE19_THECHOSENFROG_SHADER_H


#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <vector>

struct ShaderSource
{
    GLenum type;
    std::string source;
};
//TODO Seperate ShaderProgram and shader as two different resources
class ShaderProgram
{
private:
    bool compiled = false;

    std::unordered_map<GLenum,std::string> shaderCode;

    GLuint shaderProgramHandle;
    std::vector<GLuint> shaderHandles;

    std::unordered_map<std::string,GLint> uniformLocations;

public:
    ShaderProgram()=default;
    ShaderProgram(std::initializer_list<ShaderSource> shaders);

    bool compile();
    void cleanup();

    GLint getUniformLocation(const std::string& name);

    void setShader(const ShaderSource& shaderSource);

    const GLuint& getProgramHandle() const;

};


#endif //CGUE19_THECHOSENFROG_SHADER_H
