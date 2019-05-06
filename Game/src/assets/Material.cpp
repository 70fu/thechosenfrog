#include "Material.h"


void MaterialData::clearLocations()
{
    for(auto& u : floats)
        u.second.location = -1;
    for(auto& u : vec2s)
        u.second.location = -1;
    for(auto& u : vec3s)
        u.second.location = -1;
    for(auto& u : vec4s)
        u.second.location = -1;
    for(auto& u : mat2s)
        u.second.location = -1;
    for(auto& u : mat3s)
        u.second.location = -1;
    for(auto& u : mat4s)
        u.second.location = -1;
    for(auto& u : textures)
        u.second.location = -1;
    for(auto& u : colors)
        u.second.location = -1;
}

void MaterialData::retrieveLocations(ShaderProgramAsset &shaderAsset)
{
    for(auto& u : floats)
        u.second.location = shaderAsset.getUniformLocation(u.first);
    for(auto& u : vec2s)
        u.second.location = shaderAsset.getUniformLocation(u.first);
    for(auto& u : vec3s)
        u.second.location = shaderAsset.getUniformLocation(u.first);
    for(auto& u : vec4s)
        u.second.location = shaderAsset.getUniformLocation(u.first);
    for(auto& u : mat2s)
        u.second.location = shaderAsset.getUniformLocation(u.first);
    for(auto& u : mat3s)
        u.second.location = shaderAsset.getUniformLocation(u.first);
    for(auto& u : mat4s)
        u.second.location = shaderAsset.getUniformLocation(u.first);
    for(auto& u : textures)
        u.second.location = shaderAsset.getUniformLocation(u.first);
    for(auto& u : colors)
        u.second.location = shaderAsset.getUniformLocation(u.first);
}

void MaterialData::clearProps()
{
    floats.clear();
    vec2s.clear();
    vec3s.clear();
    vec4s.clear();
    mat2s.clear();
    mat3s.clear();
    mat4s.clear();
    textures.clear();
    colors.clear();
}

void Material::clearMaterialProps()
{
    data.clearProps();
}

void MaterialData::setFloat(const std::string &name, float value)
{
    floats[name].value=value;
}

void MaterialData::setVec2(const std::string &name, const glm::vec2& vec2)
{
    vec2s[name].value=vec2;
}

void MaterialData::setVec3(const std::string &name, const glm::vec3& vec3)
{
    vec3s[name].value=vec3;
}

void MaterialData::setVec4(const std::string &name, const glm::vec4& vec4)
{
    vec4s[name].value=vec4;
}

void MaterialData::setMat2(const std::string &name, const glm::mat2& mat2)
{
    mat2s[name].value=mat2;
}

void MaterialData::setMat3(const std::string &name, const glm::mat3& mat3)
{
    mat3s[name].value=mat3;
}

void MaterialData::setMat4(const std::string &name, const glm::mat4& mat4)
{
    mat4s[name].value=mat4;
}

void MaterialData::setTexture(const std::string &name, TextureAsset *texture)
{
    textures[name].value=texture;
}

void MaterialData::setColor(const std::string &name, const Color &color)
{
    colors[name].value=color;
}

/*
auto MaterialData::begin()
{
    return floats.begin();
}

auto MaterialData::end()
{
    return floats.end();
}

const auto MaterialData::begin() const
{
    return floats.cbegin();
}

const auto MaterialData::end() const
{
    return floats.cend();
}*/
