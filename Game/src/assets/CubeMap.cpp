#include "CubeMap.h"
#include "../logger/imguial_log.h"

using namespace ImGuiAl;

void CubeMap::allocateOnGPU()
{
    //TODO only in debug?
    if (allocated)
    {
        Log::getInstance().Warn("allocate called even though cube map has already been allocated, call ignored");
        return;
    }

    glGenTextures(1, &handle);

    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);

    //set parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, parameters.minFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, parameters.maxFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, parameters.sWrap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, parameters.tWrap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, parameters.rWrap);

    //allocate texture data on gpu
    for (int i = 0; i < 6; ++i)
    {
        ImageData& face = faces[i];
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,face.getInternalFormat(),face.width,face.height,0,face.getFormat(),GL_UNSIGNED_BYTE,face.data);
    }

    //unbind texture
    glBindTexture(GL_TEXTURE_CUBE_MAP,0);

    allocated = true;
}

void CubeMap::allocateOnGPU(CubeMapParameters cubeMapParameters)
{
    parameters = cubeMapParameters;
    allocateOnGPU();
}

void CubeMap::updateOnGPU()
{
    //TODO only in debug?
    if(!allocated)
    {
        ImGuiAl::Log::getInstance().Warn("updateOnGPU called even though cube map has not been allocated yet, call ignored");
        return;
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP,handle);

    //update texture data on cpu
    for (int i = 0; i < 6; ++i)
    {
        ImageData& face = faces[i];
        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0,0,0,face.width,face.height,face.getFormat(),GL_UNSIGNED_BYTE,face.data);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP,0);

}

void CubeMap::cleanup()
{
    //TODO only in debug?
    if(!allocated)
    {
        ImGuiAl::Log::getInstance().Warn("cleanup called even though texture has not been allocated yet, call ignored");
        return;
    }

    glDeleteTextures(1,&handle);

    allocated = false;
}

const GLuint &CubeMap::getHandle() const
{
    return handle;
}

const CubeMapParameters &CubeMap::getParameters() const
{
    return parameters;
}