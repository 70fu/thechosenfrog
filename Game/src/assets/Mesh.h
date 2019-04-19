//
// Created by simon on 01.04.19.
//

#ifndef CGUE19_THECHOSENFROG_MESH_H
#define CGUE19_THECHOSENFROG_MESH_H


#include <glad/glad.h>
#include "Surface.h"

/**
 * has surface data and manages it on the graphics card
 * allocateOnGPU needs to be called in order to return valid handles
 */
class Mesh
{
    friend class MeshList;
private:
    GLuint usage = GL_STATIC_DRAW;
    GLuint vaoHandle;

    //vbo handles
    /**
     * vbo handle to vertex data
     */
    GLuint vVBOHandle;
    /**
     * vbo handle to index data
     */
    GLuint iVBOHandle;

    bool allocated = false;

    /**
     * Allocates given surface on gpu with currently configured usage
     */
    void allocateOnGPU();
    void uploadSurfaceData();
    void uploadBufferDataHelper(Surface::SurfaceDataType dataType, int &offset, size_t size, const void *data,
                                unsigned char componentNum);
public:
    /**
     * Can be changed freely, but updateOnGPU needs to be called after a change
     */
    Surface surface;

    /**
     * Allocates given surface on the gpu with specified usage
     * @param usage
     */
    void allocateOnGPU(GLuint usage);
    /**
     * Updates vbo data on graphics card, is only possible if the usage is not static
     */
    void updateOnGPU();
    /**
     * Destroys buffers and vertex arrays on gpu, needs to be called before calling init again
     */
    void cleanup();

    const GLuint& getVAOHandle() const;
};


#endif //CGUE19_THECHOSENFROG_MESH_H
