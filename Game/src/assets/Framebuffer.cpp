#include "Framebuffer.h"
#include "../logger/imguial_log.h"

using namespace ImGuiAl;

void Framebuffer::allocateOnGPU()
{
    //TODO only in debug?
    if (allocated)
    {
        Log::getInstance().Warn("allocate called even though framebuffer has already been allocated, call ignored");
        return;
    }

    currentSize = glm::uvec2(parameters.width,parameters.height);

    //generate framebuffer
    glGenFramebuffers(1,&handle);
    glBindFramebuffer(GL_FRAMEBUFFER, handle);

    //generate buffers
    glGenTextures(1,&colorBufferHandle);

    //generate renderbuffer or texture for depth and stencil
    if(parameters.useTextureForDepthAndStencil)
        glGenTextures(1,&depthStencilBufferHandle);
    else
        glGenRenderbuffers(1,&depthStencilBufferHandle);

    //allocate buffers
    resize(currentSize);

    //attach
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,colorBufferHandle,0);
    if(parameters.useTextureForDepthAndStencil)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,GL_TEXTURE_2D,depthStencilBufferHandle,0);
    else
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilBufferHandle);

    //TODO only in debug
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Log::getInstance().Error("Failed to allocate framebuffer object");

    //unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER,0);

    allocated = true;
}

void Framebuffer::allocateOnGPU(FramebufferParameters framebufferParameters)
{
    parameters = framebufferParameters;
}

void Framebuffer::resize(const glm::uvec2 &size)
{
    currentSize = size;

    //resize/allocate color buffer
    glBindTexture(GL_TEXTURE_2D,colorBufferHandle);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,currentSize.x,currentSize.y,0,GL_RGB,GL_UNSIGNED_BYTE,nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    //reallocate depth & stencil buffer
    if(parameters.useTextureForDepthAndStencil)
    {
        glBindTexture(GL_TEXTURE_2D,depthStencilBufferHandle);
        glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH24_STENCIL8,currentSize.x,currentSize.y,0,GL_DEPTH_STENCIL,GL_UNSIGNED_INT_24_8,nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        glBindRenderbuffer(GL_RENDERBUFFER, depthStencilBufferHandle);
        glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8, currentSize.x,currentSize.y);
        glBindRenderbuffer(GL_RENDERBUFFER,0);
    }
}

void Framebuffer::cleanup()
{
    //TODO only in debug?
    if(!allocated)
    {
        ImGuiAl::Log::getInstance().Warn("cleanup called even though framebuffer has not been allocated yet, call ignored");
        return;
    }

    if(parameters.useTextureForDepthAndStencil)
        glDeleteTextures(1,&depthStencilBufferHandle);
    else
        glDeleteRenderbuffers(1,&depthStencilBufferHandle);
    glDeleteTextures(1,&colorBufferHandle);
    glDeleteFramebuffers(1,&handle);

    allocated = false;
}

const GLuint &Framebuffer::getFBOHandle() const
{
    return handle;
}

const GLuint &Framebuffer::getColorBufferHandle() const
{
    return colorBufferHandle;
}

const GLuint &Framebuffer::getDepthStencilBufferHandle() const
{
    return depthStencilBufferHandle;
}

const FramebufferParameters &Framebuffer::getParameters() const
{
    return parameters;
}

const glm::uvec2 &Framebuffer::getSize() const
{
    return currentSize;
}
