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


struct MaterialData
{
    friend class Material;
private:
    template <class T>
    struct UniformValue
    {
        friend class MaterialData;
    private:
        GLint location=-1;
    public:
        T value;

        GLint getLocation() const;
    };

public:
    //TODO data hiding of maps
    //uniform values
    std::unordered_map<std::string,UniformValue<float>> floats;
    std::unordered_map<std::string,UniformValue<glm::vec2>> vec2s;
    std::unordered_map<std::string,UniformValue<glm::vec3>> vec3s;
    std::unordered_map<std::string,UniformValue<glm::vec4>> vec4s;
    std::unordered_map<std::string,UniformValue<glm::mat2>> mat2s;
    std::unordered_map<std::string,UniformValue<glm::mat3>> mat3s;
    std::unordered_map<std::string,UniformValue<glm::mat4>> mat4s;
    std::unordered_map<std::string,UniformValue<TextureAsset*>> textures;

    void retrieveLocations(ShaderProgramAsset& shaderAsset);
    /**
     * Removes all property values
     */
    void clearProps();

    //set methods, override a value for a material property or add the property if it is not present
    /*void setFloat(const std::string& name, float value);
    void setVec2(const std::string& name, glm::vec2 vec2);
    void setVec3(const std::string& name, glm::vec3 vec3);
    void setVec4(const std::string& name, glm::vec4 vec4);
    void setMat2(const std::string& name, glm::mat2 mat2);
    void setMat3(const std::string& name, glm::mat3 mat3);
    void setMat4(const std::string& name, glm::mat4 mat4);
    void setTexture(const std::string& name, TextureAsset* texture);

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
