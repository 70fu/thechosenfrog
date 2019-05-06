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
        //usage can be configured on the meshes before loading them
        loadAssetsFromFileHelper("/meshes/cross.obj",MeshIds::DEFAULT,assets[MeshIds::DEFAULT],assetManager);
        //region generate unit cube
        {
            MeshAsset& cube = assets[MeshIds::UNIT_CUBE];
            cube.surface.dataFormatBitmask = Surface::POSITIONS_FORMAT|Surface::NORMALS_FORMAT|Surface::INDICES_FORMAT|Surface::UVS_FORMAT;

            //partly taken from simons ECG solution
            //Generate vertices and indices
            float w = 0.5f;
            float h = w;
            float d = w;

            glm::vec3 xNormal = { 1,0,0 };
            glm::vec3 yNormal = { 0,1,0 };
            glm::vec3 zNormal = { 0,0,1 };

            cube.surface.positions= {
                    //-X face
                    {-w, -h, -d},
                    {-w, -h, d},
                    {-w, h,  d},
                    {-w, h,  -d},
                    //X face
                    {w,  -h, -d},
                    {w,  -h, d},
                    {w,  h,  d},
                    {w,  h,  -d},
                    //-Y face
                    {-w, -h, -d},
                    {w,  -h, -d},
                    {w,  -h, d},
                    {-w, -h, d},
                    //Y face
                    {-w, h,  -d},
                    {w,  h,  -d},
                    {w,  h,  d},
                    {-w, h,  d},
                    //-Z face
                    {-w, -h, -d},
                    {-w, h,  -d},
                    {w,  -h, -d},
                    {w,  h,  -d},
                    //Z face
                    {-w, -h, d},
                    {-w, h,  d},
                    {w,  -h, d},
                    {w,  h,  d}
            };
            cube.surface.normals= {
                    //-X face
                    -xNormal, -xNormal, -xNormal, -xNormal,
                    //X face
                    xNormal, xNormal, xNormal, xNormal,
                    //-Y face
                    -yNormal, -yNormal, -yNormal, -yNormal,
                    //Y face
                    yNormal, yNormal, yNormal, yNormal,
                    //-Z face
                    -zNormal, -zNormal, -zNormal, -zNormal,
                    //Z face
                    zNormal, zNormal, zNormal, zNormal
            };
            cube.surface.indices= {
                    //-X face
                    0, 1, 3,
                    2, 3, 1,
                    //X face
                    4, 7, 6,
                    4, 6, 5,
                    //-Y face
                    8, 9, 11,
                    10, 11, 9,
                    //Y face
                    12, 15, 14,
                    12, 14, 13,
                    //-Z face
                    16, 19, 18,
                    16, 17, 19,
                    //Z face
                    20, 22, 23,
                    20, 23, 21
            };
            cube.surface.uvs= {
                    {0, 0},
                    {1, 0},
                    {1, 1},
                    {0, 1},

                    {1, 0},
                    {0, 0},
                    {0, 1},
                    {1, 1},

                    {0, 0},
                    {1, 0},
                    {1, 1},
                    {0, 1},

                    {0, 0},
                    {0, 1},
                    {1, 1},
                    {1, 0},

                    {1, 0},
                    {1, 1},
                    {0, 0},
                    {0, 1},

                    {0, 0},
                    {0, 1},
                    {1, 0},
                    {1, 1}
            };

            cube.allocateOnGPU(GL_STATIC_DRAW);
        }
        //endregion
        //region generate skybox
        {
            MeshAsset& skybox = assets[MeshIds::SKYBOX];
            skybox.surface.dataFormatBitmask = Surface::POSITIONS_FORMAT;
            skybox.surface.positions = {
                    // positions
                    {-1.0f,  1.0f, -1.0f},
                    {-1.0f, -1.0f, -1.0f},
                    {1.0f, -1.0f, -1.0f},
                    {1.0f, -1.0f, -1.0f},
                    {1.0f,  1.0f, -1.0f},
                    {-1.0f,  1.0f, -1.0f},

                    {-1.0f, -1.0f,  1.0f},
                    {-1.0f, -1.0f, -1.0f},
                    {-1.0f,  1.0f, -1.0f},
                    {-1.0f,  1.0f, -1.0f},
                    {-1.0f,  1.0f,  1.0f},
                    {-1.0f, -1.0f,  1.0f},

                    {1.0f, -1.0f, -1.0f},
                    {1.0f, -1.0f,  1.0f},
                    {1.0f,  1.0f,  1.0f},
                    {1.0f,  1.0f,  1.0f},
                    {1.0f,  1.0f, -1.0f},
                    {1.0f, -1.0f, -1.0f},

                    {-1.0f, -1.0f,  1.0f},
                    {-1.0f,  1.0f,  1.0f},
                    {1.0f,  1.0f,  1.0f},
                    {1.0f,  1.0f,  1.0f},
                    {1.0f, -1.0f,  1.0f},
                    {-1.0f, -1.0f,  1.0f},

                    {-1.0f,  1.0f, -1.0f},
                    {1.0f,  1.0f, -1.0f},
                    {1.0f,  1.0f,  1.0f},
                    {1.0f,  1.0f,  1.0f},
                    {-1.0f,  1.0f,  1.0f},
                    {-1.0f,  1.0f, -1.0f},

                    {-1.0f, -1.0f, -1.0f},
                    {-1.0f, -1.0f,  1.0f},
                    {1.0f, -1.0f, -1.0f},
                    {1.0f, -1.0f, -1.0f},
                    {-1.0f, -1.0f,  1.0f},
                    {1.0f, -1.0f,  1.0f}
            };

            skybox.allocateOnGPU(GL_STATIC_DRAW);
        }
        //endregion
        loadAssetsFromFileHelper("/meshes/SignPost.obj",MeshIds::SIGNPOST,assets[MeshIds::SIGNPOST],assetManager);
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
            ImGuiAl::Log::getInstance().Error("Loading mesh from %s has failed: %s",path.c_str(),importer.GetErrorString());
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

        if(/*(asset.surface.dataFormatBitmask&Surface::INDICES_FORMAT)!=0*/mesh->HasFaces())
        {
            for(unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                const aiFace& face = mesh->mFaces[i];
                for(unsigned int j = 0;j<face.mNumIndices;++j)
                {
                    asset.surface.indices.push_back(face.mIndices[j]);
                }
            }
        }
        else
            asset.surface.dataFormatBitmask&=~Surface::INDICES_FORMAT;

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

        //allocate mesh on the gpu with usage configured before calling this method
        asset.allocateOnGPU();
        return true;
    }

    void loadDefault(MeshAsset &asset, AssetManager& assetManager) override
    {
        loadAssetFromFile(getFullAssetPath("/meshes/cross.obj"),asset);
        asset.allocateOnGPU(GL_STATIC_DRAW);
    }
};
REGISTERCLASS(MeshList);
