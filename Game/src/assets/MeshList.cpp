#include "AssetList.h"
#include "../RuntimeClasses.h"
#include <ObjectInterfacePerModule.h>
#include "Assets.h"
#include "Mesh.h"
#include "MeshIds.h"
#include "../Constants.h"
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
            skybox.surface.dataFormatBitmask = Surface::POSITIONS_FORMAT | Surface::NORMALS_FORMAT;
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
            skybox.surface.normals = {
                    {0,0,1},
                    {0,0,1},
                    {0,0,1},
                    {0,0,1},
                    {0,0,1},
                    {0,0,1},

                    {1,0,0},
                    {1,0,0},
                    {1,0,0},
                    {1,0,0},
                    {1,0,0},
                    {1,0,0},

                    {-1,0,0},
                    {-1,0,0},
                    {-1,0,0},
                    {-1,0,0},
                    {-1,0,0},
                    {-1,0,0},

                    {0,0,-1},
                    {0,0,-1},
                    {0,0,-1},
                    {0,0,-1},
                    {0,0,-1},
                    {0,0,-1},

                    {0,-1,0},
                    {0,-1,0},
                    {0,-1,0},
                    {0,-1,0},
                    {0,-1,0},
                    {0,-1,0},

                    {0,1,0},
                    {0,1,0},
                    {0,1,0},
                    {0,1,0},
                    {0,1,0},
                    {0,1,0},
            };

            skybox.allocateOnGPU(GL_STATIC_DRAW);
        }
        //endregion
        loadAssetsFromFileHelper("/meshes/SignPost.obj",MeshIds::SIGNPOST,assets[MeshIds::SIGNPOST],assetManager);
        //region generate screen quad
        {
            MeshAsset& quad = assets[MeshIds::SCREEN_QUAD];
            quad.surface.dataFormatBitmask = Surface::POSITIONS_FORMAT|Surface::UVS_FORMAT;

            //Generate vertices uvs
            quad.surface.positions= {
                    {-1.0f,-1.0f,0},//bottom left
                    {1.0f,1.0f,0},//top right
                    {-1.0f,1.0f,0},//top left
                    {-1.0f,-1.0f,0},//bottom left
                    {1.0f,-1.0f,0},//bottom right
                    {1.0f,1.0f,0}//top right
            };

            quad.surface.uvs= {
                    {0, 0},
                    {1, 1},
                    {0, 1},
                    {0, 0},
                    {1, 0},
                    {1, 1}
            };

            quad.allocateOnGPU(GL_STATIC_DRAW);
        }
        //endregion
        //make cloud sphere
        genSphere(assets[MeshIds::CLOUD_SPHERE],16,16,0.5);
        assets[MeshIds::CLOUD_SPHERE].allocateOnGPU(GL_STATIC_DRAW);
        //make cylinder
        genCylinder(assets[MeshIds::UNIT_CYLINDER],1,1,16);
        assets[MeshIds::UNIT_CYLINDER].allocateOnGPU(GL_STATIC_DRAW);
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

private:
    /**
     * Fills asset with vertices, normals, indices and uvs but does not allocate the mesh
     * @param asset
     * @param longSegments
     * @param latSegments
     * @param radius
     */
    static void genSphere(MeshAsset &asset, unsigned int longSegments, unsigned int latSegments, double radius)
    {
        //indices per iteration
        static constexpr int IPIT = 6;

        asset.surface.dataFormatBitmask = Surface::POSITIONS_FORMAT|Surface::NORMALS_FORMAT|Surface::INDICES_FORMAT|Surface::UVS_FORMAT;

        unsigned int vertexCount = longSegments * (latSegments - 1) + 2;
        asset.surface.positions=std::vector<glm::vec3>(vertexCount);
        asset.surface.normals=std::vector<glm::vec3>(vertexCount);
        asset.surface.indices=std::vector<unsigned int>(2 * longSegments*(latSegments - 1) * 3);
        asset.surface.uvs=std::vector<glm::vec2>(vertexCount);
        unsigned int topVertI = 0;
        unsigned int bottomVertI = longSegments * (latSegments - 1) + 2 - 1;

        asset.surface.positions[topVertI] = { 0,radius,0 };
        asset.surface.positions[bottomVertI] = { 0,-radius,0 };
        asset.surface.normals[topVertI] = { 0,1,0 };
        asset.surface.normals[bottomVertI] = { 0,-1,0 };
        asset.surface.uvs[topVertI] = {0,0};
        asset.surface.uvs[bottomVertI] = {0,1};
        for (unsigned int lat = 1; lat<latSegments;lat++)
        {
            double theta = PI * lat / latSegments;
            double y = std::cos(theta);
            for (unsigned int lon = 0; lon < longSegments; lon++)
            {
                double phi = 2 * PI * lon / longSegments;
                double x = std::sin(theta)*std::cos(phi);
                double z = std::sin(theta)*-std::sin(phi);

                //vertices & normals
                unsigned int i = (lat - 1)*longSegments + lon + 1;
                glm::vec3 vertex = { x,y,z };
                asset.surface.positions[i] = vertex*(float)radius;
                asset.surface.normals[i] = vertex;

                //uvs
                asset.surface.uvs[i] = { phi / (2 * PI), theta / PI};

                //indices
                if (lat == 1)
                    continue;

                unsigned int ii = (i-1-longSegments) * IPIT+longSegments*3;
                int right = i;
                int topRight = right - longSegments;
                int left = (lat - 1)*longSegments + (lon-1)%longSegments + 1;
                int topLeft = left - longSegments;

                asset.surface.indices[ii + 0] = right;
                asset.surface.indices[ii + 1] = topRight;
                asset.surface.indices[ii + 2] = left;

                asset.surface.indices[ii + 3] = left;
                asset.surface.indices[ii + 4] = topRight;
                asset.surface.indices[ii + 5] = topLeft;
            }
        }

        //make indices for last latitudal segment
        for (unsigned int lon = 0; lon < longSegments; lon++)
        {
            //top
            unsigned int ii = lon*3;
            asset.surface.indices[ii + 0] = lon+1;
            asset.surface.indices[ii + 1] = topVertI;
            asset.surface.indices[ii + 2] = (lon - 1) % longSegments + 1;

            //bottom
            ii = (2 * longSegments*(latSegments - 1) * 3) - longSegments * 3+lon*3;
            asset.surface.indices[ii + 0] = (latSegments - 2)*longSegments+lon+1;
            asset.surface.indices[ii + 1] = (latSegments - 2)*longSegments+(lon - 1) % longSegments + 1;
            asset.surface.indices[ii + 2] = bottomVertI;
        }
    }

    static void genCylinder(MeshAsset& asset, double height, double radius, unsigned int segments)
    {
        //indices per iteration
        static constexpr int IPIT = 12;

        asset.surface.dataFormatBitmask = Surface::POSITIONS_FORMAT|Surface::NORMALS_FORMAT|Surface::INDICES_FORMAT|Surface::UVS_FORMAT;


        unsigned int vertexCount = (2 * segments * 2 + 2);
        asset.surface.positions=std::vector<glm::vec3>(vertexCount);
        asset.surface.normals=std::vector<glm::vec3>(vertexCount);
        asset.surface.indices = std::vector<unsigned int>(4 * segments * 3);
        asset.surface.uvs=std::vector<glm::vec2>(vertexCount);
        unsigned int topVertI = 2 * segments * 2;
        unsigned int bottomVertI = topVertI + 1;

        double h = height / 2;
        //top and bottom center
        glm::vec3 topCenter = asset.surface.positions[topVertI] = { 0,h,0 };
        glm::vec3 bottomCenter = asset.surface.positions[bottomVertI] = { 0,-h,0 };
        asset.surface.normals[topVertI] = { 0,1,0 };
        asset.surface.normals[bottomVertI] = { 0,-1,0 };
        asset.surface.uvs[topVertI] = asset.surface.uvs[bottomVertI] = { 0.5f,0.5f };
        for (unsigned int i = 0; i < topVertI; i += 4)
        {
            //vertices
            double angle = 2*PI*i / topVertI;

            double x = std::cos(angle);
            double z = -std::sin(angle);
            asset.surface.positions[i] = asset.surface.positions[i+2] = { x*radius,h, z*radius };
            asset.surface.positions[i + 1] = asset.surface.positions[i+3] = { x*radius,-h,z*radius };
            //normals
            asset.surface.normals[i] = asset.surface.normals[i + 1] = { x,0,z };// glm::normalize(glm::vec3(x, h, z) - topCenter);
            asset.surface.normals[i + 2] = { 0,1,0 };
            asset.surface.normals[i + 3] = { 0,-1,0 };

            //uvs
            double uCoordMantle = static_cast<double>(i) / topVertI;
            asset.surface.uvs[i] = { uCoordMantle ,1 };//top mantle
            asset.surface.uvs[i + 1] = { uCoordMantle,0 };//bottom mantle
            asset.surface.uvs[i + 2] = asset.surface.uvs[i + 3] = { (x+1)/2,(z+1)/2 };//top & bottom


            //indices
            unsigned int ii = IPIT * i / 4;
            unsigned int top = i;
            unsigned int bottom = top + 1;
            unsigned int circleTop = top + 2;
            unsigned int circleBottom = bottom + 2;
            unsigned int lastTop = (i - 4) % topVertI;
            unsigned int lastBottom = lastTop + 1;
            unsigned int lastCircleTop = lastTop + 2;
            unsigned int lastCircleBottom = lastBottom + 2;
            //top
            asset.surface.indices[ii] = circleTop;
            asset.surface.indices[ii +1] = topVertI;
            asset.surface.indices[ii + 2] = lastCircleTop;

            //bottom
            asset.surface.indices[ii + 3] = circleBottom;
            asset.surface.indices[ii + 4] = lastCircleBottom;
            asset.surface.indices[ii + 5] = bottomVertI;

            //mantle faces
            asset.surface.indices[ii + 6] = top;
            asset.surface.indices[ii + 7] = lastTop;
            asset.surface.indices[ii + 8] = bottom;

            asset.surface.indices[ii + 9] = lastTop;
            asset.surface.indices[ii + 10] = lastBottom;
            asset.surface.indices[ii + 11] = bottom;
        }

    }
};
REGISTERCLASS(MeshList)
