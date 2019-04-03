//
// Created by simon on 01.04.19.
//

#include "Surface.h"

size_t Surface::getVertexDataSize() {
    size_t size = 0;
    if((dataFormatBitmask&POSITIONS_FORMAT)!=0)
        size+=positions.size()*sizeof(glm::vec3);
    if((dataFormatBitmask&NORMALS_FORMAT)!=0)
        size+=normals.size()*sizeof(glm::vec3);
    if((dataFormatBitmask&TANGENTS_FORMAT)!=0)
        size+=tangents.size()*sizeof(glm::vec3);
    if((dataFormatBitmask&UVS_FORMAT)!=0)
        size+=uvs.size()*sizeof(glm::vec2);
    if((dataFormatBitmask&COLORS_FORMAT)!=0)
        size+=colors.size()*sizeof(glm::vec4);

    return size;
}
