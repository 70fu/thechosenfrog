#ifndef CGUE19_THECHOSENFROG_FRAMEBUFFER_H
#define CGUE19_THECHOSENFROG_FRAMEBUFFER_H


#include <glad/glad.h>
#include <vec2.hpp>

struct FramebufferParameters
{
public:
    /**
     * false, RenderbufferObject is used
     * true, Texture is used
     */
    bool useTextureForDepthAndStencil = false;
    unsigned int width = 1600;
    unsigned int height = 900;
    /**
     * If true, framebuffer is resized on resolution changes
     */
    bool resizeOnResolutionChange = false;
    unsigned int colorBufferCount = 1;
};

class Framebuffer
{
private:
    GLuint handle;
    GLuint* colorBufferHandles;
    GLuint depthStencilBufferHandle;

    FramebufferParameters parameters;

    bool allocated = false;

    glm::uvec2 currentSize;

    /**
    * Allocates on gpu using currently set parameters
    */
    void allocateOnGPU();
public:
    /**
     * If resizeOnResolutionChange is true and the window resizes, the framebuffer is resized to
     * newWidth = newWindowWidth*resizeWidthRatio, newHeight = newWindowHeight*resizeHeightRatio
     * rounded up
     */
    float resizeWidthRatio = 1;
    float resizeHeightRatio = 1;

    /**
     * Allocates framebuffer on the gpu with given parameters
     * @param framebufferParameters
     */
    void allocateOnGPU(FramebufferParameters framebufferParameters);

    /**
     * Updates image data of the cube map on the gpu
     */
    void resize(const glm::uvec2& size);

    /**
     * Deletes framebuffer on the gpu, needs to be called before calling allocate again
     */
    void cleanup();

    const GLuint& getFBOHandle() const;
    const GLuint& getColorBufferHandle(unsigned int index=0) const;
    const GLuint& getDepthStencilBufferHandle() const;

    const FramebufferParameters& getParameters() const;

    const glm::uvec2& getSize() const;
};


#endif //CGUE19_THECHOSENFROG_FRAMEBUFFER_H
