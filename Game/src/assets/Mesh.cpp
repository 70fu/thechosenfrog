//
// Created by simon on 01.04.19.
//

#include "Mesh.h"
#include "../logger/imguial_log.h"

void Mesh::allocateOnGPU()
{
    if(allocated)
    {
        ImGuiAl::Log::getInstance().Warn("allocate called even though mesh has already been allocated, call ignored");
        return;
    }

    //TODO check if arrays have correct (same) sizes

    //generate VAO
    glGenVertexArrays(1, &vaoHandle);

    //bind vao
    glBindVertexArray(vaoHandle);

    //generate & upload vertex data
    glGenBuffers(1, &vVBOHandle);
    glBindBuffer(GL_ARRAY_BUFFER, vVBOHandle);

    //allocate buffer
    glBufferData(GL_ARRAY_BUFFER,surface.getVertexDataSize(),nullptr,usage);

    //set data aligned in buffer (vvvnnntttuuuccc)
    uploadSurfaceData();

    //generate & upload indices
    if((surface.dataFormatBitmask&Surface::INDICES_FORMAT) != 0)
    {
        glGenBuffers(1, &iVBOHandle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iVBOHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, surface.indices.size() * sizeof(unsigned int), surface.indices.data(), GL_STATIC_DRAW);
    }

    //unbind VAO and VBOs
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    allocated = true;
}

void Mesh::uploadSurfaceData() {
    //set data aligned in buffer (vvvnnntttuuuccc)
    int offset = 0;
    uploadBufferDataHelper(Surface::POSITIONS,offset,surface.positions.size()*sizeof(glm::vec3),surface.positions.data(),3);
    uploadBufferDataHelper(Surface::NORMALS,offset,surface.normals.size()*sizeof(glm::vec3),surface.normals.data(),3);
    uploadBufferDataHelper(Surface::TANGENTS,offset,surface.tangents.size()*sizeof(glm::vec3),surface.tangents.data(),3);
    uploadBufferDataHelper(Surface::UVS,offset,surface.uvs.size()*sizeof(glm::vec2),surface.uvs.data(),2);
    uploadBufferDataHelper(Surface::COLORS,offset,surface.colors.size()*sizeof(glm::vec4),surface.colors.data(),4);
}

void Mesh::uploadBufferDataHelper(Surface::SurfaceDataType dataType, int &offset, size_t size, const void *data,
                                  unsigned char componentNum) {
    if((surface.dataFormatBitmask&(1<<dataType)) != 0)
    {
        //TODO check data types of parameters
        //upload data
        glBufferSubData(GL_ARRAY_BUFFER,offset,size,data);
        if(!allocated)
        {
            //Bind to attribute position
            glVertexAttribPointer(dataType, componentNum, GL_FLOAT, GL_FALSE, componentNum * sizeof(float),
                                  (void *) offset);
            glEnableVertexAttribArray(dataType);
        }

        offset+=size;
    }
}

void Mesh::allocateOnGPU(GLuint usage) {
    this->usage = usage;
    allocateOnGPU();
}

void Mesh::updateOnGPU() {
    glBindBuffer(GL_ARRAY_BUFFER, vVBOHandle);
    uploadSurfaceData();
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Mesh::cleanup() {
    if(!allocated)
    {
        ImGuiAl::Log::getInstance().Warn("cleanup called even though mesh has not been allocated yet, call ignored");
        return;
    }

    glDeleteVertexArrays(1, &vaoHandle);
    glDeleteBuffers(1, &vVBOHandle);
    if((surface.dataFormatBitmask&Surface::INDICES_FORMAT) != 0)
        glDeleteBuffers(1, &iVBOHandle);

    surface.clear();

    allocated = false;
}

const GLuint &Mesh::getVAOHandle() const {
    return vaoHandle;
}
