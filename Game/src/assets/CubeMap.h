#ifndef CGUE19_THECHOSENFROG_CUBEMAP_H
#define CGUE19_THECHOSENFROG_CUBEMAP_H

#include "ImageData.h"

struct CubeMapParameters
{
public:
    GLuint minFilter=GL_NEAREST;
    GLuint maxFilter=GL_NEAREST;
    GLuint sWrap = GL_REPEAT;
    GLuint tWrap = GL_REPEAT;
    GLuint rWrap = GL_REPEAT;
};

class CubeMap
{
private:
    GLuint handle;

    CubeMapParameters parameters;

    bool allocated = false;

    /**
     * Allocates on gpu using currently set parameters
     */
    void allocateOnGPU();
public:
    enum FaceIds:unsigned char
    {
        POS_X,
        NEG_X,
        POS_Y,
        NEG_Y,
        POS_Z,
        NEG_Z
    };
    /**
     * Contains the image data of the faces of the cube,
     * can be edited, but updateOnGPU needs to be called afterwards for changes to take effect
     * All images need to be square in dimensions and the same size
     * {+x,-x,+y,-y,+z,-z}
     */
    ImageData faces[6];

    /**
     * Allocates cube map on the gpu with given parameters
     * @param cubeMapParameters
     */
    void allocateOnGPU(CubeMapParameters cubeMapParameters);

    /**
     * Updates image data of the cube map on the gpu
     */
    void updateOnGPU();

    /**
     * Deletes cube map on the gpu, needs to be called before calling allocate again
     */
    void cleanup();

    const GLuint& getHandle() const;
    const CubeMapParameters& getParameters() const;
};


#endif //CGUE19_THECHOSENFROG_CUBEMAP_H
