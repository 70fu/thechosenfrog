#ifndef CGUE19_THECHOSENFROG_SHADER_H
#define CGUE19_THECHOSENFROG_SHADER_H


#include <glad/glad.h>
#include <string>

/**
 * Represents one shader file
 * This class also manages the shader on the graphics card
 *
 * Usage:
 * 1. set type
 * 2. set sourceCode
 * 3. compile()
 * 4. getShaderHandle() and do sth with it
 * 5. cleanup()
 * 6. Repeat as often as you want
 */
class Shader
{
private:
    GLuint shaderHandle;
    bool compiled = false;

    static bool checkShaderCompileStatus(GLuint shaderHandle);
public:
    GLenum type;
    std::string sourceCode;

    /**
     * Compiles shader on graphics card, logs compile errors
     * @return true if the compilation succeeded
     */
    bool compile();
    /**
     * Deletes shader on graphics card
     */
    void cleanup();

    /**
     * @return shader handle, only valid if compilation succeeded
     */
    GLuint getShaderHandle() const;
};


#endif //CGUE19_THECHOSENFROG_SHADER_H
