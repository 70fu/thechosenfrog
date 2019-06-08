#include "Shader.h"
#include "../logger/imguial_log.h"

using namespace ImGuiAl;

//checks the status of the shader compilation and prints an error message if compilation failed
bool Shader::checkShaderCompileStatus(GLuint shaderHandle)
{
    GLint succeded;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &succeded);
    if (!succeded)
    {
        GLint length;
        glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);

        GLchar* log = new char[length];
        glGetShaderInfoLog(shaderHandle, length, NULL, log);

        Log::getInstance().Error("Shader compile error: %s",log);

        delete[] log;
    }

    return succeded;
}

bool Shader::compile()
{
    if(compiled)
    {
        Log::getInstance().Warn("Cannot compile shader, which has not been cleaned up yet, call ignored");
        return false;
    }

    shaderHandle = glCreateShader(type);

    //compile shader
    const char* srcPtr = sourceCode.c_str();
    glShaderSource(shaderHandle, 1, &srcPtr, NULL);
    glCompileShader(shaderHandle);

    //return shader compilation status
    return (compiled = checkShaderCompileStatus(shaderHandle));
}

void Shader::cleanup()
{
    if(!compiled)
    {
        Log::getInstance().Warn("Cannot clean up shader, that has not been compiled yet, call ignored");
        return;
    }

    glDeleteShader(shaderHandle);

    compiled = false;
}

GLuint Shader::getShaderHandle() const
{
    return shaderHandle;
}
