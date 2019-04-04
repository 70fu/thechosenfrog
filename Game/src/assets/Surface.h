//
// Created by simon on 01.04.19.
//

#ifndef CGUE19_THECHOSENFROG_GEOMETRYDATA_H
#define CGUE19_THECHOSENFROG_GEOMETRYDATA_H

#include <vec3.hpp>
#include <vector>
#include <vec2.hpp>
#include <vec4.hpp>

/*!
 * Stores all data for a geometry object
 */
struct Surface {
    enum SurfaceDataType : unsigned char
    {
        POSITIONS = 0,
        INDICES = 1,
        NORMALS = 2,
        TANGENTS = 3,
        UVS = 4,
        COLORS = 5
    };

    enum SurfaceDataFormat : unsigned char
    {
        POSITIONS_FORMAT=1<<POSITIONS,
        INDICES_FORMAT = 1<<INDICES,
        NORMALS_FORMAT=1<<NORMALS,
        TANGENTS_FORMAT=1<<TANGENTS,
        UVS_FORMAT=1<<UVS,
        COLORS_FORMAT=1<<COLORS
    };

    /**
     * Bitmask of SurfaceData values, that describe which vectors are filled
     */
    unsigned char dataFormatBitmask=~0;

    /*!
     * Vertex positions
     */
    std::vector<glm::vec3> positions;
    /*!
     * Geometry indices
     */
    std::vector<unsigned int> indices;
    /*!
     * Vertex normals
     */
    std::vector<glm::vec3> normals;
    /**
     * Tangents
     */
    std::vector<glm::vec3> tangents;
    /*!
     * Vertex UV coordinates
     */
    std::vector<glm::vec2> uvs;
    /**
     * Vertex Colors
     */
    std::vector<glm::vec4> colors;

    /**
     *
     * @return size  of buffer needed for storing all data except the index array
     */
    size_t getVertexDataSize();

    /**
     * Clears all data and sets dataFormatBitmask to 0
     */
    void clear();
};


#endif //CGUE19_THECHOSENFROG_GEOMETRYDATA_H
