#include "ShaderProgram.h"
#include "../logger/imguial_log.h"

using namespace ImGuiAl;

ShaderProgram::ShaderProgram(std::initializer_list<ShaderSource> shaders)
{
    for(const ShaderSource& ss : shaders)
        setShader(ss);
}

bool ShaderProgram::compile()
{
    if(compiled)
    {
        Log::getInstance().Warn("Cannot compile shader, which has not been cleaned up yet, call ignored");
        return false;
    }

    //TODO compile

    return false;
}

void ShaderProgram::cleanup()
{
    if(!compiled)
    {
        Log::getInstance().Warn("Cannot clean up shader, that has not been compiled yet, call ignored");
        return;
    }

    //delete shaders on GPU

    for(GLuint handle : shaderHandles)
        glDeleteShader(handle);
    glDeleteProgram(shaderProgramHandle);

    //clear sources
    shaderCode.clear();
    uniformLocations.clear();

    compiled = false;
}

GLint ShaderProgram::getUniformLocation(const std::string& name)
{
    if(uniformLocations.count(name)==0)
    {
        //retrieve from shader
        GLint loc = glGetUniformLocation(shaderProgramHandle,name.c_str());
        if(loc!=-1)
            uniformLocations[name]=loc;
        return loc;
    }
    else
        return uniformLocations[name];
}

void ShaderProgram::setShader(const ShaderSource &shaderSource)
{
    shaderCode[shaderSource.type]=shaderSource.source;
}

const GLuint &ShaderProgram::getProgramHandle() const
{
    return shaderProgramHandle;
}
