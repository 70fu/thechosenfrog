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

class Texture
{
    friend class TextureList;
private:
    GLuint handle;

    TextureParameters parameters;

    bool allocated = false;

    /**
     * Allocates on gpu using current parameters
     */
    void allocateOnGPU();
public:
    /**
     * Can be changed freely, but updateOnGPU needs to be called after a change
     */
    ImageData data;

    void allocateOnGPU(TextureParameters textureParameters);

    void updateOnGPU();

    void cleanup();

    const GLuint& getTextureHandle() const;
    const TextureParameters& getTextureParameters() const;
};


#endif //CGUE19_THECHOSENFROG_TEXTURE_H
