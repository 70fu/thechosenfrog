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

    void drawBitmapFont(const BitmapFontAsset& font)
    {
        drawTexture(font.getBitmap());
        //TODO other members
    }

    ComponentId componentCommon(unsigned int numActive,Components::Types type)
    {
        ImGui::Text("Active Components: %d", numActive);

        ComponentId step = 1;
        ImGui::InputScalar("Chosen Component", ImGuiDataType_S16, &componentIndices[type],
                           &step);

        //clamp value
        componentIndices[type] = std::clamp<ComponentId>(
                componentIndices[type], 0, numActive);

        if(numActive>0)
            return componentIndices[type];
        return INVALID_COMPONENT_ID;
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
                    //render transform component
                    ComponentId id = componentCommon(game->transformComps.getNumActive(),Components::TRANSFORM);
                    if (id!=INVALID_COMPONENT_ID)
                    {
                        TransformComponent &transform = game->transformComps[id];
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
                //region character controller components
                if (ImGui::BeginTabItem("Char Controller"))
                {

                    ComponentId id = componentCommon(game->charControllerComps.getNumActive(),Components::CHAR_CONTROLLER);
                    if (id!=INVALID_COMPONENT_ID)
                    {
                        CharControllerComponent& con = game->charControllerComps[id];

                        //render state
                        bool grounded = con.grounded;
                        ImGui::Checkbox("Grounded",&grounded);
                        bool jumping = con.jumping;
                        ImGui::Checkbox("Jumping",&jumping);
                        bool wantJump = con.wantJump;
                        float airTime = con.airTime;
                        ImGui::InputFloat("Air Time",&airTime);
                        ImGui::Checkbox("WantJump",&wantJump);
                        glm::vec2 direction = con.direction;
                        ImGui::InputFloat2("Direction",glm::value_ptr(direction));
                        float jumpStrength = con.jumpStrength;
                        ImGui::InputFloat("Jump Strength",&jumpStrength);
                        glm::vec3 acc = con.acc;
                        ImGui::InputFloat3("Acceleration",glm::value_ptr(acc));
                        glm::vec3 vel = con.vel;
                        ImGui::InputFloat3("Velocity",glm::value_ptr(vel));

                        glm::vec3 lastJump = con.landedOn-con.jumpedFrom;
                        float lastJumpHeight=con.lastJumpHeight;
                        float lastJumpDist = glm::length(lastJump);
                        ImGui::PushItemWidth(64);
                        ImGui::InputFloat("Dist",&lastJumpDist);
                        ImGui::SameLine();
                        ImGui::InputFloat("Height",&lastJumpHeight);
                        ImGui::SameLine();
                        ImGui::Text("Last Jump");
                        ImGui::PopItemWidth();
                        float cachedJumpHeight = con.cachedJumpHeight;
                        ImGui::InputFloat("Jump Height Cache",&cachedJumpHeight);
                        float cachedJumpDistance = con.cachedJumpDistance;
                        ImGui::InputFloat("Jump Cache",&cachedJumpDistance);
                        float cachedJumpDuration = con.cachedJumpDuration;
                        ImGui::InputFloat("Jump Cache",&cachedJumpDuration);

                        //render config
                        if(ImGui::CollapsingHeader("Config"))
                        {
                            ImGui::InputFloat("Ground Walk Force",&con.groundWalkForce);
                            ImGui::InputFloat("Air Walk Speed",&con.airWalkSpeed);
                            ImGui::InputFloat("Min Jump Distance",&con.minJumpDistance);
                            ImGui::InputFloat("Max Jump Distance",&con.maxJumpDistance);
                            ImGui::InputFloat("Min Jump Height",&con.minJumpHeight);
                            ImGui::InputFloat("Max Jump Height",&con.maxJumpHeight);
                            ImGui::InputFloat("Min Jump Duration",&con.minJumpDuration);
                            ImGui::InputFloat("Max Jump Duration",&con.maxJumpDuration);
                            ImGui::InputFloat("Max Ground Speed",&con.maxGroundSpeed);
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
                //region Bitmap Fonts
                if (ImGui::BeginTabItem("Bitmap Fonts"))
                {
                    for (unsigned int i = 0; i < BitmapFontIds::BITMAP_FONT_COUNT; ++i)
                    {
                        if (ImGui::CollapsingHeader(getNum(i)))
                            drawBitmapFont(*game->getAssetManager().getBitmapFont(i));
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