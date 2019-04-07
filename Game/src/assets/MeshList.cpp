//
// Created by simon on 22.03.19.
//

#ifndef CGUE19_THECHOSENFROG_MUSICLIST_H
#define CGUE19_THECHOSENFROG_MUSICLIST_H

#include "AssetList.h"
#include "../RuntimeClasses.h"
#include <ObjectInterfacePerModule.h>
#include "Assets.h"
#include "Mesh.h"
#include "MeshIds.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

class MeshList : public TInterface<RuntimeClassIds::MESH_LIST,AssetList<MeshAsset ,AssetType::MESH>>
{
protected:
    void loadAssets(MeshAsset *assets, size_t size, AssetManager& assetManager) override
    {
        loadAssetsFromFileHelper("/meshes/cross.obj",MeshIds::DEFAULT,assets[MeshIds::DEFAULT],assetManager);

        for(int i = 0;i<size;++i)
            assets[i].allocateOnGPU(GL_STATIC_DRAW);
        //...
    }

    //based off doxygen documentation at http://sir-kimmi.de and https://learnopengl.com/Model-Loading/Model
    bool loadAssetFromFile(const std::string &path, MeshAsset &asset) override
    {
        // Create an instance of the Importer class
        Assimp::Importer importer;
        // And have it read the given file with some example postprocessing
        // Usually - if speed is not the most important aspect for you - you'll
        // probably to request more postprocessing than we do in this example.
        const aiScene* scene = importer.ReadFile( path,
                                                  //aiProcess_CalcTangentSpace       |
                                                  aiProcess_Triangulate            |
                                                  aiProcess_JoinIdenticalVertices  |
                                                  aiProcess_FlipUVs |
                                                  aiProcess_GenNormals);
        // If the import failed, report it
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            ImGuiAl::Log::getInstance().Error("Loading mesh from %s has failed: $s",path.c_str(),importer.GetErrorString());
            return false;
        }
        if(scene->mNumMeshes==0)
        {
            ImGuiAl::Log::getInstance().Error("%s contains no meshes",path.c_str());
            return false;
        }

        // load first mesh
        aiMesh* mesh = scene->mMeshes[0];
        if(/*(asset.surface.dataFormatBitmask&Surface::POSITIONS_FORMAT)!=0*/true)
        {
            for(unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                glm::vec3 vertex;
                vertex.x = mesh->mVertices[i].x;
                vertex.y = mesh->mVertices[i].y;
                vertex.z = mesh->mVertices[i].z;
                asset.surface.positions.push_back(vertex);
            }
        }

        if(/*(asset.surface.dataFormatBitmask&Surface::NORMALS_FORMAT)!=0 && */mesh->HasNormals())
        {
            for(unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                glm::vec3 normal;
                normal.x = mesh->mNormals[i].x;
                normal.y = mesh->mNormals[i].y;
                normal.z = mesh->mNormals[i].z;
                asset.surface.normals.push_back(normal);
            }
        }
        else
            asset.surface.dataFormatBitmask&=~Surface::NORMALS_FORMAT;

        if(/*(asset.surface.dataFormatBitmask&Surface::TANGENTS_FORMAT)!=0 && */mesh->HasTangentsAndBitangents())
        {
            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                glm::vec3 tangent;
                tangent.x = mesh->mTangents[i].x;
                tangent.y = mesh->mTangents[i].y;
                tangent.z = mesh->mTangents[i].z;
                asset.surface.tangents.push_back(tangent);
            }
        }
        else
            asset.surface.dataFormatBitmask&=~Surface::TANGENTS_FORMAT;

        if(/*(asset.surface.dataFormatBitmask&Surface::UVS_FORMAT)!=0 && */mesh->HasTextureCoords(0))
        {
            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                glm::vec2 uv;
                uv.x = mesh->mTextureCoords[0][i].x;
                uv.y = mesh->mTextureCoords[0][i].y;
                asset.surface.uvs.push_back(uv);
            }
        }
        else
            asset.surface.dataFormatBitmask&=~Surface::UVS_FORMAT;

        if(/*(asset.surface.dataFormatBitmask&Surface::COLORS_FORMAT)!=0 && */mesh->HasVertexColors(0))
        {
            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                glm::vec4 color;
                color.r = mesh->mColors[0][i].r;
                color.g = mesh->mColors[0][i].g;
                color.b = mesh->mColors[0][i].b;
                color.a = mesh->mColors[0][i].a;
                asset.surface.colors.push_back(color);
            }
        }
        else
            asset.surface.dataFormatBitmask&=~Surface::COLORS_FORMAT;

        // We're done. Everything will be cleaned up by the importer destructor
        return true;
    }

    void loadDefault(MeshAsset &asset, AssetManager& assetManager) override
    {
        loadAssetFromFile(getFullAssetPath("/meshes/cross.obj"),asset);
        asset.allocateOnGPU(GL_STATIC_DRAW);
    }
};
REGISTERCLASS(MeshList);



#endif //CGUE19_THECHOSENFROG_MUSICLIST_H

