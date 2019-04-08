#include "ShaderProgram.h"
#include "../logger/imguial_log.h"

using namespace ImGuiAl;

ShaderProgram::ShaderProgram(std::initializer_list<Shader*> shaders)
{
    for(Shader* s : shaders)
        addShader(s);
}

bool ShaderProgram::link()
{
    if(linked)
    {
        Log::getInstance().Warn("Cannot create shader program, which has not been cleaned up yet, call ignored");
        return false;
    }

    //link shaders to program object
    shaderProgramHandle = glCreateProgram();

    //attach shaders
    for (Shader* shader : shaders)
        glAttachShader(shaderProgramHandle, shader->getShaderHandle());

    //link shaders
    glLinkProgram(shaderProgramHandle);

    //check linking errors
    GLint succeded;
    glGetProgramiv(shaderProgramHandle, GL_LINK_STATUS, &succeded);
    if (!succeded)
    {
        GLint length;
        glGetProgramiv(shaderProgramHandle, GL_INFO_LOG_LENGTH, &length);

        GLchar* log = new char[length];
        glGetProgramInfoLog(shaderProgramHandle, length, NULL, log);

        Log::getInstance().Error("Shader link error: %s",log);

        delete[] log;

    }

    //detach shaders
    for (Shader* shader : shaders)
        glDetachShader(shaderProgramHandle, shader->getShaderHandle());

    return succeded;
}

void ShaderProgram::cleanup()
{
    if(!linked)
    {
        Log::getInstance().Warn("Cannot clean up shader program, that has not been linked yet, call ignored");
        return;
    }

    //delete program on GPU
    glDeleteProgram(shaderProgramHandle);

    //clear sources
    shaders.clear();
    uniformLocations.clear();

    linked = false;
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

void ShaderProgram::addShader(Shader* shader)
{
    shaders.push_back(shader);
}

bool ShaderProgram::hasShader(Shader *shader)
{
    for(Shader* attachedShader : shaders)
    {
        if(shader==attachedShader)
            return true;
    }
    return false;
}

const GLuint &ShaderProgram::getProgramHandle() const
{
    return shaderProgramHandle;
}
