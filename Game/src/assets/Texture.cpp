#include "Texture.h"
#include "../logger/imguial_log.h"

using namespace ImGuiAl;

void Texture::allocateOnGPU()
{
    //TODO only in debug?
    if(allocated)
    {
        Log::getInstance().Warn("allocate called even though texture has already been allocated, call ignored");
        return;
    }

    glGenTextures(1,&handle);

    glBindTexture(parameters.target,handle);

    //set parameters
    glTexParameteri(parameters.target, GL_TEXTURE_MIN_FILTER, parameters.minFilter);
    glTexParameteri(parameters.target, GL_TEXTURE_MAG_FILTER, parameters.maxFilter);
    glTexParameteri(parameters.target, GL_TEXTURE_WRAP_S, parameters.sWrap);
    glTexParameteri(parameters.target, GL_TEXTURE_WRAP_S, parameters.tWrap);

    //allocate texture data structure on gpu
    glTexImage2D(parameters.target,0,channelCountToInternalFormat[data.channelCount],data.width,data.height,0,channelCountToFormat[data.channelCount],GL_UNSIGNED_BYTE,data.data);

    if(parameters.generateMipMaps)
        glGenerateMipmap(parameters.target);

    //unbind texture
    glBindTexture(parameters.target,0);

    allocated = true;
}

void Texture::allocateOnGPU(TextureParameters textureParameters)
{
    parameters = textureParameters;
    allocateOnGPU();
}

void Texture::updateOnGPU()
{
    //TODO only in debug?
    if(!allocated)
    {
        ImGuiAl::Log::getInstance().Warn("updateOnGPU called even though texture has not been allocated yet, call ignored");
        return;
    }

    glBindTexture(parameters.target,handle);

    glTexSubImage2D(parameters.target,0,0,0,data.width,data.height,channelCountToFormat[data.channelCount],GL_UNSIGNED_BYTE,data.data);

    glBindTexture(parameters.target,0);

}

void Texture::cleanup()
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

const GLuint &Texture::getTextureHandle() const
{
    return handle;
}

const TextureParameters &Texture::getTextureParameters() const
{
    return parameters;
}
