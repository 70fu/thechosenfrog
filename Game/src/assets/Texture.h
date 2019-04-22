#ifndef CGUE19_THECHOSENFROG_TEXTURE_H
#define CGUE19_THECHOSENFROG_TEXTURE_H

#include <glad/glad.h>
#include "ImageData.h"

struct TextureParameters
{
public:
    GLenum target=GL_TEXTURE_2D;
    bool generateMipMaps=true;
    GLuint minFilter=GL_LINEAR_MIPMAP_LINEAR;
    GLuint maxFilter=GL_LINEAR;
    GLuint sWrap = GL_REPEAT;
    GLuint tWrap = GL_REPEAT;
};

/**
 * Manages a texture on the GPU
 * allocateOnGPU needs to be called in order to return valid handles
 * Usage:
 * 1. set data
 * 2. allocateOnGPU
 * 3. update data and call updateOnGPU as often as you want
 * 4. cleanup
 * 5. Repeat if desired
 */
class Texture
{
    friend class TextureList;
private:
    static constexpr GLenum channelCountToInternalFormat[]={0,GL_R8,GL_RG8,GL_RGB8,GL_RGBA8};
    static constexpr GLenum channelCountToFormat[]={0,GL_RED,GL_RG,GL_RGB,GL_RGBA};

    GLuint handle;

    TextureParameters parameters;

    bool allocated = false;

    /**
     * Allocates on gpu using currently set parameters
     */
    void allocateOnGPU();
public:
    /**
     * Can be changed freely, but updateOnGPU needs to be called after a change
     */
    ImageData data;

    /**
     * Allocates image data on the gpu with given parameters
     * @param textureParameters
     */
    void allocateOnGPU(TextureParameters textureParameters);

    /**
     * Updates image data on the gpu
     */
    void updateOnGPU();

    /**
     * Deletes texture on the gpu, needs to be called before calling allocate again
     */
    void cleanup();

    const GLuint& getTextureHandle() const;
    const TextureParameters& getTextureParameters() const;
};


#endif //CGUE19_THECHOSENFROG_TEXTURE_H
