//
// Created by simon on 31.03.19.
//

#include <ObjectInterfacePerModule.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include "RuntimeClasses.h"
#include "IDebugGUI.h"
#include "Game.h"
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_access.hpp>

class ImGuiDebugGUI : public TInterface<RuntimeClassIds ::IMGUI_DEBUG_GUI,IDebugGUI>
{
private:

    /**
     * stores numbers as strings from 0-99
     */
    char numbers[10*2+90*3];
    //region components viewer
    //bool componentTabsOpen[Components::COMPONENT_COUNT]={true};
    ComponentId componentIndices[Components::COMPONENT_COUNT]={0};
    //endregion

    const char* getNum(unsigned int number)
    {
        if(number>99)
            number=99;

        if(number>=10)
            return &numbers[20+(number-10)*3];
        else
            return &numbers[number*2];
    }

    void drawCMat(const glm::mat2 &mat)
    {
        glm::mat2 transposed = glm::transpose(mat);
        ImGui::InputFloat2("",&transposed[0][0]);
        ImGui::InputFloat2("",&transposed[1][0]);
    }

    bool drawMat(glm::mat2 & mat)
    {
        glm::mat2 transposed = glm::transpose(mat);
        if(ImGui::InputFloat2("",&transposed[0][0])||
           ImGui::InputFloat2("",&transposed[1][0]))
        {
            mat = glm::transpose(mat);
            return true;
        }
        return false;
    }

    void drawCMat(const glm::mat3 &mat)
    {
        glm::mat4 transposed = glm::transpose(mat);
        ImGui::InputFloat3("",&transposed[0][0]);
        ImGui::InputFloat3("",&transposed[1][0]);
        ImGui::InputFloat3("",&transposed[2][0]);
    }

    bool drawMat(glm::mat3 & mat)
    {
        glm::mat3 transposed = glm::transpose(mat);
        if(ImGui::InputFloat3("",&transposed[0][0])||
           ImGui::InputFloat3("",&transposed[1][0])||
           ImGui::InputFloat3("",&transposed[2][0]))
        {
            mat = glm::transpose(mat);
            return true;
        }
        return false;
    }

    void drawCMat(const glm::mat4x4 &mat)
    {
        glm::mat4 transposed = glm::transpose(mat);
        ImGui::InputFloat4("",&transposed[0][0]);
        ImGui::InputFloat4("",&transposed[1][0]);
        ImGui::InputFloat4("",&transposed[2][0]);
        ImGui::InputFloat4("",&transposed[3][0]);
    }

    bool drawMat(glm::mat4x4 & mat)
    {
        glm::mat4 transposed = glm::transpose(mat);
        if(ImGui::InputFloat4("",&transposed[0][0])||
            ImGui::InputFloat4("",&transposed[1][0])||
            ImGui::InputFloat4("",&transposed[2][0])||
            ImGui::InputFloat4("",&transposed[3][0]))
        {
            mat = glm::transpose(mat);
            return true;
        }
        return false;
    }

    void drawMesh(const MeshAsset& mesh)
    {
        ImGui::Text("VAO Handle: %d",mesh.getVAOHandle());

        bool hasFeature = false;

        hasFeature = ((mesh.surface.dataFormatBitmask&Surface::POSITIONS_FORMAT)!=0);
        ImGui::Checkbox("",&hasFeature);
        ImGui::SameLine();
        ImGui::Text("Vertices: %ld",mesh.surface.positions.size());

        hasFeature = ((mesh.surface.dataFormatBitmask&Surface::INDICES_FORMAT)!=0);
        ImGui::Checkbox("",&hasFeature);
        ImGui::SameLine();
        ImGui::Text("Indices: %ld",mesh.surface.indices.size());

        hasFeature = ((mesh.surface.dataFormatBitmask&Surface::NORMALS_FORMAT)!=0);
        ImGui::Checkbox("",&hasFeature);
        ImGui::SameLine();
        ImGui::Text("Normals: %ld",mesh.surface.normals.size());

        hasFeature = ((mesh.surface.dataFormatBitmask&Surface::TANGENTS_FORMAT)!=0);
        ImGui::Checkbox("",&hasFeature);
        ImGui::SameLine();
        ImGui::Text("Tangents: %ld",mesh.surface.tangents.size());

        hasFeature = ((mesh.surface.dataFormatBitmask&Surface::UVS_FORMAT)!=0);
        ImGui::Checkbox("",&hasFeature);
        ImGui::SameLine();
        ImGui::Text("UVs: %ld",mesh.surface.uvs.size());

        hasFeature = ((mesh.surface.dataFormatBitmask&Surface::COLORS_FORMAT)!=0);
        ImGui::Checkbox("",&hasFeature);
        ImGui::SameLine();
        ImGui::Text("Colors: %ld",mesh.surface.colors.size());
    }

    void drawTexture(const TextureAsset& texture)
    {
        ImGui::Text("Handle: %d",texture.getTextureHandle());
        ImGui::Text("%dx%d, Channels: %d",texture.data.getWidth(),texture.data.getHeight(),texture.data.getChannelCount());
        //ImGui::Image((void *) texture.getTextureHandle(), ImVec2(128, 128*(texture.data.getHeight()/texture.data.getWidth())));
        ImGui::Image((void *) texture.getTextureHandle(), ImVec2(texture.data.getWidth(),texture.data.getHeight()));
    }

    void drawCubeMap(const CubeMapAsset& cubeMap)
    {
        ImGui::Text("Handle: %d",cubeMap.getHandle());
        ImGui::Text("Faces: %dx%d, Channels: %d",cubeMap.faces[0].getWidth(),cubeMap.faces[0].getHeight(),cubeMap.faces[0].getChannelCount());
        //ImGui::Image((void *) texture.getTextureHandle(), ImVec2(128, 128*(texture.data.getHeight()/texture.data.getWidth())));
        //ImGui::Image((void *) cubeMap.getHandle(), ImVec2(cubeMap.faces[0].getWidth(),cubeMap.faces[0].getHeight()));
    }

    void drawShaderProgram(Game& game, ShaderProgramAsset& program)
    {
        unsigned int i = 0;
        for(auto shaderIt = program.begin();shaderIt!=program.end();++shaderIt,++i)
        {
            if(ImGui::TreeNode(getNum((*shaderIt)-game.getAssetManager().getShader(ShaderIds::DEFAULT))))
            {
                ImGui::TextUnformatted((*shaderIt)->sourceCode.c_str());
                ImGui::TreePop();
            }
        }
    }

    void drawMaterial(Game& game, MaterialAsset& mat)
    {
        if(ImGui::TreeNode("Properties"))
        {
            for (auto &p : mat.data.floats)
            {
                ImGui::InputFloat(p.first.c_str(), &p.second.value);
                ImGui::SameLine();
                ImGui::Text("Uniform Location: %d", p.second.getLocation());
            }
            for (auto &p : mat.data.vec2s)
            {
                ImGui::InputFloat2(p.first.c_str(), glm::value_ptr(p.second.value));
                ImGui::SameLine();
                ImGui::Text("Uniform Location: %d", p.second.getLocation());
            }
            for (auto &p : mat.data.vec3s)
            {
                ImGui::InputFloat3(p.first.c_str(), glm::value_ptr(p.second.value));
                ImGui::SameLine();
                ImGui::Text("Uniform Location: %d", p.second.getLocation());
            }
            for (auto &p : mat.data.vec4s)
            {
                ImGui::InputFloat4(p.first.c_str(), glm::value_ptr(p.second.value));
                ImGui::SameLine();
                ImGui::Text("Uniform Location: %d", p.second.getLocation());
            }
            for (auto &p : mat.data.mat2s)
            {
                drawMat(p.second.value);
                ImGui::SameLine();
                ImGui::Text("Uniform Location: %d", p.second.getLocation());
            }
            for (auto &p : mat.data.mat3s)
            {
                drawMat(p.second.value);
                ImGui::SameLine();
                ImGui::Text("Uniform Location: %d", p.second.getLocation());
            }
            for (auto &p : mat.data.mat4s)
            {
                drawMat(p.second.value);
                ImGui::SameLine();
                ImGui::Text("Uniform Location: %d", p.second.getLocation());
            }
            for (auto &p : mat.data.textures)
            {
                if (ImGui::TreeNode(p.first.c_str()))
                {
                    ImGui::Text("Uniform Location: %d", p.second.getLocation());
                    drawTexture(*p.second.value);
                    ImGui::TreePop();
                }
            }

            ImGui::TreePop();
        }

        //draw atttached shader program
        if(ImGui::TreeNode("Shader Program"))
        {
            drawShaderProgram(game,*mat.shader);
            ImGui::TreePop();
        }

        if(ImGui::Button("Reload Uniform locations"))
            mat.data.retrieveLocations(*mat.shader);
    }
public:
    void Init(bool isFirstInit) override
    {
        IDebugGUI::Init(isFirstInit);

        //init character arrays
        //init number array
        char* p = numbers;
        for(int i = 0;i<100;++i)
        {
            p+=sprintf(p,"%d",i)+1;
        }
    }

    virtual void Serialize(ISimpleSerializer *pSerializer) override
    {
        IDebugGUI::Serialize(pSerializer);

        //TODO
    }

    void init(Game *game) override
    {
        //Setup imgui
        ImGui::CreateContext();

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(game->getWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 430");
    }

    void cleanup(Game *game) override
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void render(Game* game) override
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //draw windows
        //region logger
        {
            if (isWindowEnabled(DebugWindowIds::LOGGER))
            {
                ImGui::Begin("Logger (1)");
                int action = ImGuiAl::Log::getInstance().Draw();
                ImGui::End();
            }
        }
        //endregion
        //region component viewer
        {
            if (isWindowEnabled(DebugWindowIds::COMPONENT_VIEWER))
            {
                ImGui::Begin("ComponentViewer");
                ImGui::BeginTabBar("Components", ImGuiTabBarFlags_None);
                //region transform components
                if (ImGui::BeginTabItem("Transforms"))
                {
                    ImGui::Text("Active Components: %d", game->transformComps.getNumActive());

                    ComponentId step = 1;
                    ImGui::InputScalar("Chosen Component", ImGuiDataType_S16, &componentIndices[Components::TRANSFORM],
                                       &step);

                    //clamp value
                    componentIndices[Components::TRANSFORM] = std::clamp<ComponentId>(
                            componentIndices[Components::TRANSFORM], 0, game->transformComps.getNumActive());

                    //render transform component
                    if (game->transformComps.getNumActive() > 0)
                    {
                        TransformComponent &transform = game->transformComps[componentIndices[Components::TRANSFORM]];
                        glm::vec3 pos = transform.getTranslation();
                        if (ImGui::InputFloat3("Position: ", glm::value_ptr(pos)))
                            transform.setTranslation(pos);
                        glm::vec3 rot = transform.getRotation();
                        if (ImGui::InputFloat3("Rotation: ", glm::value_ptr(rot)))
                            transform.setRotation(rot);
                        glm::vec3 scale = transform.getScaling();
                        if (ImGui::InputFloat3("Scale: ", glm::value_ptr(scale)))
                            transform.setScaling(scale);

                        ImGui::Text("Local Matrix");
                        drawCMat(transform.getTransformationMatrix());

                        ImGui::Text("Global Matrix");
                        drawCMat(transform.getGlobalTransform());
                    }

                    ImGui::EndTabItem();
                }
                //endregion
                ImGui::EndTabBar();
                ImGui::End();
            }
        }
        //endregion
        //region asset viewer
        {
            if (isWindowEnabled(DebugWindowIds::ASSET_VIEWER))
            {
                ImGui::Begin("Asset Viewer (3)");
                ImGui::BeginTabBar("Assets", ImGuiTabBarFlags_None);
                //region Meshes
                if (ImGui::BeginTabItem("Meshes"))
                {
                    for(unsigned int i = 0;i<MeshIds::MESH_COUNT;++i)
                    {
                        if(ImGui::CollapsingHeader(getNum(i)))
                            drawMesh(*game->getAssetManager().getMesh(i));
                    }
                    ImGui::EndTabItem();
                }
                //endregion
                //region textures
                if (ImGui::BeginTabItem("Textures"))
                {
                    for(unsigned int i = 0;i<TextureIds::TEXTURE_COUNT;++i)
                    {
                        if(ImGui::CollapsingHeader(getNum(i)))
                            drawTexture(*game->getAssetManager().getTexture(i));
                    }
                    ImGui::EndTabItem();
                }
                //endregion
                //region cube maps
                if (ImGui::BeginTabItem("Cube Maps"))
                {
                    for(unsigned int i = 0;i<CubeMapIds::CUBE_MAP_COUNT;++i)
                    {
                        if(ImGui::CollapsingHeader(getNum(i)))
                            drawCubeMap(*game->getAssetManager().getCubeMap(i));
                    }
                    ImGui::EndTabItem();
                }
                //endregion
                //region Materials
                if (ImGui::BeginTabItem("Materials"))
                {
                    for(unsigned int i = 0;i<MaterialIds::MATERIAL_COUNT;++i)
                    {
                        if(ImGui::TreeNode(getNum(i)))
                        {
                            drawMaterial(*game,*game->getAssetManager().getMaterial(i));
                            ImGui::TreePop();
                        }
                    }
                    ImGui::EndTabItem();
                }
                //endregion
                //region Shaders
                if (ImGui::BeginTabItem("Shaders"))
                {
                    for(unsigned int i = 0;i<ShaderIds::SHADER_COUNT;++i)
                    {
                        if(ImGui::CollapsingHeader(getNum(i)))
                            ImGui::TextUnformatted(game->getAssetManager().getShader(i)->sourceCode.c_str());
                    }
                    ImGui::EndTabItem();
                }
                //endregion
                //region Shader Programs
                if (ImGui::BeginTabItem("Shader Programs"))
                {
                    for(unsigned int i = 0;i<ShaderProgramIds::SHADER_PROGRAM_COUNT;++i)
                    {
                        if(ImGui::TreeNode(getNum(i)))
                        {
                            drawShaderProgram(*game,*game->getAssetManager().getShaderProgram(i));
                            ImGui::TreePop();
                        }
                    }
                    ImGui::EndTabItem();
                }
                //endregion

                ImGui::EndTabBar();
                ImGui::End();
            }
        }
        //endregion

        //Generate Render data and render using opengl implementation
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
};
REGISTERCLASS(ImGuiDebugGUI)