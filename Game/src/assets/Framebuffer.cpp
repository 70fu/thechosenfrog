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
    colorBufferHandles = new GLuint[parameters.colorBufferCount];
    glGenTextures(parameters.colorBufferCount,&colorBufferHandles[0]);

    //generate renderbuffer or texture for depth and stencil
    if(parameters.useTextureForDepthAndStencil)
        glGenTextures(1,&depthStencilBufferHandle);
    else
        glGenRenderbuffers(1,&depthStencilBufferHandle);

    //allocate buffers
    allocated = true;//hack
    resize(currentSize);
    allocated = false;

    //attach
    for(int i = 0;i<parameters.colorBufferCount;++i)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i,GL_TEXTURE_2D,colorBufferHandles[i],0);
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
    allocateOnGPU();
}

void Framebuffer::resize(const glm::uvec2 &size)
{
    //TODO only in debug?
    if (!allocated)
    {
        Log::getInstance().Warn("resize called even though framebuffer has not been allocated, ignoring call");
        return;
    }

    currentSize = size;

    //resize/allocate color buffers
    for(int i = 0;i<parameters.colorBufferCount;++i)
    {
        glBindTexture(GL_TEXTURE_2D, colorBufferHandles[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, currentSize.x, currentSize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    //reallocate depth & stencil buffer
    if(parameters.useTextureForDepthAndStencil)
    {
        glBindTexture(GL_TEXTURE_2D,depthStencilBufferHandle);
        glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH24_STENCIL8,currentSize.x,currentSize.y,0,GL_DEPTH_STENCIL,GL_UNSIGNED_INT_24_8,nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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
    glDeleteTextures(parameters.colorBufferCount,&colorBufferHandles[0]);
    glDeleteFramebuffers(1,&handle);

    delete[] colorBufferHandles;

    allocated = false;
}

const GLuint &Framebuffer::getFBOHandle() const
{
    return handle;
}

const GLuint &Framebuffer::getColorBufferHandle(unsigned int index) const
{
    return colorBufferHandles[index];
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
