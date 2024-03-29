#ifndef CGUE19_THECHOSENFROG_MATERIAL_H
#define CGUE19_THECHOSENFROG_MATERIAL_H


#include <glad/glad.h>
#include <string>
#include <vec2.hpp>
#include <vec3.hpp>
#include <vec4.hpp>
#include <unordered_map>
#include <mat2x2.hpp>
#include <mat3x3.hpp>
#include <mat4x4.hpp>
#include "Assets.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "../util/Color.h"


struct MaterialData
{
    friend class Material;
public:
    template <class T>
    struct UniformValue
    {
        friend class MaterialData;
    private:
        GLint location=-1;
    public:
        T value;

        GLint getLocation() const{return location;}
    };

public:
    //TODO data hiding of maps
    //uniform values
    std::unordered_map<std::string,UniformValue<int>> ints;
    std::unordered_map<std::string,UniformValue<float>> floats;
    std::unordered_map<std::string,UniformValue<glm::vec2>> vec2s;
    std::unordered_map<std::string,UniformValue<glm::vec3>> vec3s;
    std::unordered_map<std::string,UniformValue<glm::vec4>> vec4s;
    std::unordered_map<std::string,UniformValue<glm::mat2>> mat2s;
    std::unordered_map<std::string,UniformValue<glm::mat3>> mat3s;
    std::unordered_map<std::string,UniformValue<glm::mat4>> mat4s;
    std::unordered_map<std::string,UniformValue<TextureAsset*>> textures;
    std::unordered_map<std::string,UniformValue<Color>> colors;

    /**
     * resets uniform locations of material properties to -1
     */
    void clearLocations();
    /**
     * calls getUniformLocation on given shader for each material property and caches the uniform location
     * @param shaderAsset
     */
    void retrieveLocations(ShaderProgramAsset& shaderAsset);
    /**
     * Removes all property values
     */
    void clearProps();

    //set methods, override a value for a material property or add the property if it is not present
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setVec2(const std::string& name, const glm::vec2& vec2);
    void setVec3(const std::string& name, const glm::vec3& vec3);
    void setVec4(const std::string& name, const glm::vec4& vec4);
    void setMat2(const std::string& name, const glm::mat2& mat2);
    void setMat3(const std::string& name, const glm::mat3& mat3);
    void setMat4(const std::string& name, const glm::mat4& mat4);
    void setTexture(const std::string& name, TextureAsset* texture);
    void setColor(const std::string& name, const Color& color);

    /*
    //iterators
    auto beginFloats();
    auto endFloats();
    const auto beginFloats() const;
    const auto endFloats() const;

    auto beginVec2();
    auto endVec2();
    const auto beginVec2() const;
    const auto endVec2() const;*/
    //...


};

class Material
{
public:
    MaterialData data;
    ShaderProgramAsset* shader;

    /**
     * Removes all property values
     */
    void clearMaterialProps();
};


#endif //CGUE19_THECHOSENFROG_MATERIAL_H
