#include <ObjectInterfacePerModule.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include "RuntimeClasses.h"
#include "IDebugGUI.h"
#include "Game.h"
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_access.hpp>

using namespace ImGui;

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
        InputFloat2("",&transposed[0][0]);
        InputFloat2("",&transposed[1][0]);
    }

    bool drawMat(glm::mat2 & mat)
    {
        glm::mat2 transposed = glm::transpose(mat);
        if(InputFloat2("",&transposed[0][0])||
           InputFloat2("",&transposed[1][0]))
        {
            mat = glm::transpose(mat);
            return true;
        }
        return false;
    }

    void drawCMat(const glm::mat3 &mat)
    {
        glm::mat4 transposed = glm::transpose(mat);
        InputFloat3("",&transposed[0][0]);
        InputFloat3("",&transposed[1][0]);
        InputFloat3("",&transposed[2][0]);
    }

    bool drawMat(glm::mat3 & mat)
    {
        glm::mat3 transposed = glm::transpose(mat);
        if(InputFloat3("",&transposed[0][0])||
           InputFloat3("",&transposed[1][0])||
           InputFloat3("",&transposed[2][0]))
        {
            mat = glm::transpose(mat);
            return true;
        }
        return false;
    }

    void drawCMat(const glm::mat4x4 &mat)
    {
        glm::mat4 transposed = glm::transpose(mat);
        InputFloat4("",&transposed[0][0]);
        InputFloat4("",&transposed[1][0]);
        InputFloat4("",&transposed[2][0]);
        InputFloat4("",&transposed[3][0]);
    }

    bool drawMat(glm::mat4x4 & mat)
    {
        glm::mat4 transposed = glm::transpose(mat);
        if(InputFloat4("",&transposed[0][0])||
            InputFloat4("",&transposed[1][0])||
            InputFloat4("",&transposed[2][0])||
            InputFloat4("",&transposed[3][0]))
        {
            mat = glm::transpose(mat);
            return true;
        }
        return false;
    }

    void drawMesh(const MeshAsset& mesh)
    {
        Text("VAO Handle: %d",mesh.getVAOHandle());

        bool hasFeature = false;

        hasFeature = ((mesh.surface.dataFormatBitmask&Surface::POSITIONS_FORMAT)!=0);
        Checkbox("",&hasFeature);
        SameLine();
        Text("Vertices: %ld",mesh.surface.positions.size());

        hasFeature = ((mesh.surface.dataFormatBitmask&Surface::INDICES_FORMAT)!=0);
        Checkbox("",&hasFeature);
        SameLine();
        Text("Indices: %ld",mesh.surface.indices.size());

        hasFeature = ((mesh.surface.dataFormatBitmask&Surface::NORMALS_FORMAT)!=0);
        Checkbox("",&hasFeature);
        SameLine();
        Text("Normals: %ld",mesh.surface.normals.size());

        hasFeature = ((mesh.surface.dataFormatBitmask&Surface::TANGENTS_FORMAT)!=0);
        Checkbox("",&hasFeature);
        SameLine();
        Text("Tangents: %ld",mesh.surface.tangents.size());

        hasFeature = ((mesh.surface.dataFormatBitmask&Surface::UVS_FORMAT)!=0);
        Checkbox("",&hasFeature);
        SameLine();
        Text("UVs: %ld",mesh.surface.uvs.size());

        hasFeature = ((mesh.surface.dataFormatBitmask&Surface::COLORS_FORMAT)!=0);
        Checkbox("",&hasFeature);
        SameLine();
        Text("Colors: %ld",mesh.surface.colors.size());
    }

    void drawTexture(const TextureAsset& texture)
    {
        Text("Handle: %d",texture.getTextureHandle());
        Text("%dx%d, Channels: %d",texture.data.getWidth(),texture.data.getHeight(),texture.data.getChannelCount());
        //Image((void *) texture.getTextureHandle(), ImVec2(128, 128*(texture.data.getHeight()/texture.data.getWidth())));
        Image((void *) texture.getTextureHandle(), ImVec2(texture.data.getWidth(),texture.data.getHeight()));
    }

    void drawCubeMap(const CubeMapAsset& cubeMap)
    {
        Text("Handle: %d",cubeMap.getHandle());
        Text("Faces: %dx%d, Channels: %d",cubeMap.faces[0].getWidth(),cubeMap.faces[0].getHeight(),cubeMap.faces[0].getChannelCount());
        //Image((void *) texture.getTextureHandle(), ImVec2(128, 128*(texture.data.getHeight()/texture.data.getWidth())));
        //Image((void *) cubeMap.getHandle(), ImVec2(cubeMap.faces[0].getWidth(),cubeMap.faces[0].getHeight()));
    }

    void drawShaderProgram(Game& game, ShaderProgramAsset& program)
    {
        unsigned int i = 0;
        for(auto shaderIt = program.begin();shaderIt!=program.end();++shaderIt,++i)
        {
            if(TreeNode(getNum((*shaderIt)-game.getAssetManager().getShader(ShaderIds::DEFAULT))))
            {
                TextUnformatted((*shaderIt)->sourceCode.c_str());
                TreePop();
            }
        }
    }

    void drawMaterial(Game& game, MaterialAsset& mat)
    {
        if(TreeNode("Properties"))
        {
            for (auto &p : mat.data.floats)
            {
                InputFloat(p.first.c_str(), &p.second.value);
                SameLine();
                Text("Uniform Location: %d", p.second.getLocation());
            }
            for (auto &p : mat.data.vec2s)
            {
                InputFloat2(p.first.c_str(), glm::value_ptr(p.second.value));
                SameLine();
                Text("Uniform Location: %d", p.second.getLocation());
            }
            for (auto &p : mat.data.vec3s)
            {
                InputFloat3(p.first.c_str(), glm::value_ptr(p.second.value));
                SameLine();
                Text("Uniform Location: %d", p.second.getLocation());
            }
            for (auto &p : mat.data.vec4s)
            {
                InputFloat4(p.first.c_str(), glm::value_ptr(p.second.value));
                SameLine();
                Text("Uniform Location: %d", p.second.getLocation());
            }
            for (auto &p : mat.data.mat2s)
            {
                drawMat(p.second.value);
                SameLine();
                Text("Uniform Location: %d", p.second.getLocation());
            }
            for (auto &p : mat.data.mat3s)
            {
                drawMat(p.second.value);
                SameLine();
                Text("Uniform Location: %d", p.second.getLocation());
            }
            for (auto &p : mat.data.mat4s)
            {
                drawMat(p.second.value);
                SameLine();
                Text("Uniform Location: %d", p.second.getLocation());
            }
            for (auto &p : mat.data.textures)
            {
                if (TreeNode(p.first.c_str()))
                {
                    Text("Uniform Location: %d", p.second.getLocation());
                    drawTexture(*p.second.value);
                    TreePop();
                }
            }

            TreePop();
        }

        //draw atttached shader program
        if(TreeNode("Shader Program"))
        {
            drawShaderProgram(game,*mat.shader);
            TreePop();
        }

        if(Button("Reload Uniform locations"))
            mat.data.retrieveLocations(*mat.shader);
    }

    void drawBitmapFont(const BitmapFontAsset& font)
    {
        drawTexture(font.getBitmap());
        //TODO other members
    }

    ComponentId componentCommon(unsigned int numActive,Components::Types type)
    {
        Text("Active Components: %d", numActive);

        ComponentId step = 1;
        InputScalar("Chosen Component", ImGuiDataType_S16, &componentIndices[type],
                           &step);

        //clamp value
        componentIndices[type] = std::clamp<ComponentId>(
                componentIndices[type], 0, numActive>0?numActive-1:0);

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
        CreateContext();

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(game->getWindow(), true);
        ImGui_ImplOpenGL3_Init("#version 430");
    }

    void cleanup(Game *game) override
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        DestroyContext();
    }

    void render(Game* game) override
    {
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        NewFrame();

        //draw windows
        //region logger
        {
            if (isWindowEnabled(DebugWindowIds::LOGGER))
            {
                Begin("Logger (1)");
                int action = ImGuiAl::Log::getInstance().Draw();
                End();
            }
        }
        //endregion
        //region component viewer
        {
            if (isWindowEnabled(DebugWindowIds::COMPONENT_VIEWER))
            {
                Begin("ComponentViewer");
                BeginTabBar("Components", ImGuiTabBarFlags_None);
                //region transform components
                if (BeginTabItem("Transforms"))
                {
                    //render transform component
                    ComponentId id = componentCommon(game->transformComps.getNumActive(),Components::TRANSFORM);
                    if (id!=INVALID_COMPONENT_ID)
                    {
                        TransformComponent &transform = game->transformComps[id];
                        glm::vec3 pos = transform.getTranslation();
                        if (InputFloat3("Position: ", glm::value_ptr(pos)))
                            transform.setTranslation(pos);
                        glm::vec3 rot = transform.getRotation();
                        if (InputFloat3("Rotation: ", glm::value_ptr(rot)))
                            transform.setRotation(rot);
                        glm::vec3 scale = transform.getScaling();
                        if (InputFloat3("Scale: ", glm::value_ptr(scale)))
                            transform.setScaling(scale);

                        Text("Local Matrix");
                        drawCMat(transform.getTransformationMatrix());

                        Text("Global Matrix");
                        drawCMat(transform.getGlobalTransform());
                    }

                    EndTabItem();
                }
                //endregion
                //region character controller components
                if (BeginTabItem("Char Controller"))
                {

                    ComponentId id = componentCommon(game->charControllerComps.getNumActive(),Components::CHAR_CONTROLLER);
                    if (id!=INVALID_COMPONENT_ID)
                    {
                        CharControllerComponent& con = game->charControllerComps[id];

                        //render state
                        bool grounded = con.grounded;
                        Checkbox("Grounded",&grounded);
                        bool jumping = con.jumping;
                        Checkbox("Jumping",&jumping);
                        bool wantJump = con.wantJump;
                        float airTime = con.airTime;
                        InputFloat("Air Time",&airTime);
                        Checkbox("WantJump",&wantJump);
                        glm::vec2 direction = con.direction;
                        InputFloat2("Direction",glm::value_ptr(direction));
                        float jumpStrength = con.jumpStrength;
                        InputFloat("Jump Strength",&jumpStrength);
                        glm::vec3 acc = con.acc;
                        InputFloat3("Acceleration",glm::value_ptr(acc));
                        glm::vec3 vel = con.vel;
                        InputFloat3("Velocity",glm::value_ptr(vel));

                        glm::vec3 lastJump = con.landedOn-con.jumpedFrom;
                        float lastJumpHeight=con.lastJumpHeight;
                        float lastJumpDist = glm::length(lastJump);
                        PushItemWidth(64);
                        InputFloat("Dist",&lastJumpDist);
                        SameLine();
                        InputFloat("Height",&lastJumpHeight);
                        SameLine();
                        Text("Last Jump");
                        PopItemWidth();
                        float cachedJumpHeight = con.cachedJumpHeight;
                        InputFloat("Jump Height Cache",&cachedJumpHeight);
                        float cachedJumpDistance = con.cachedJumpDistance;
                        InputFloat("Jump Cache",&cachedJumpDistance);
                        float cachedJumpDuration = con.cachedJumpDuration;
                        InputFloat("Jump Cache",&cachedJumpDuration);

                        //render config
                        if(CollapsingHeader("Config"))
                        {
                            InputFloat("Ground Walk Force",&con.groundWalkForce);
                            InputFloat("Air Walk Speed",&con.airWalkSpeed);
                            InputFloat("Min Jump Distance",&con.minJumpDistance);
                            InputFloat("Max Jump Distance",&con.maxJumpDistance);
                            InputFloat("Min Jump Height",&con.minJumpHeight);
                            InputFloat("Max Jump Height",&con.maxJumpHeight);
                            InputFloat("Min Jump Duration",&con.minJumpDuration);
                            InputFloat("Max Jump Duration",&con.maxJumpDuration);
                            InputFloat("Max Ground Speed",&con.maxGroundSpeed);
                        }
                    }

                    EndTabItem();
                }
                //endregion
                //region Camera components
                if (BeginTabItem("Cameras"))
                {

                    ComponentId id = componentCommon(game->charControllerComps.getNumActive(),
                                                     Components::CHAR_CONTROLLER);
                    if (id != INVALID_COMPONENT_ID)
                    {
                        CameraComponent& camera = game->cameraComps[id];

                        glm::vec2 viewportSize = camera.getViewportSize();
                        if (InputFloat2("Viewport size: ", glm::value_ptr(viewportSize)))
                            camera.setViewportSize(viewportSize);
                        glm::vec2 screenViewportPos = camera.getScreenViewportPos();
                        if (InputFloat2("Screen viewport pos: ", glm::value_ptr(screenViewportPos)))
                            camera.setScreenViewportPos(screenViewportPos);
                        glm::vec2 screenViewportSize = camera.getScreenViewportSize();
                        if (InputFloat2("Screen Viewport size: ", glm::value_ptr(screenViewportSize)))
                            camera.setScreenViewportSize(screenViewportSize);
                        float fov = camera.getFov()*TO_DEGREES;
                        if(InputFloat("FOV",&fov,1))
                            camera.setFov(fov*TO_RADIANS);
                        float near = camera.getFov();
                        if(InputFloat("Near",&near))
                            camera.setNear(near);
                        float far = camera.getFar();
                        if(InputFloat("Far",&far))
                            camera.setFar(far);

                        Text("Projection Matrix");
                        drawCMat(camera.getProjectionMatrix());
                    }

                    EndTabItem();
                }
                //endregion
                EndTabBar();
                End();
            }
        }
        //endregion
        //region asset viewer
        {
            if (isWindowEnabled(DebugWindowIds::ASSET_VIEWER))
            {
                Begin("Asset Viewer (3)");
                BeginTabBar("Assets", ImGuiTabBarFlags_None);
                //region Meshes
                if (BeginTabItem("Meshes"))
                {
                    for(unsigned int i = 0;i<MeshIds::MESH_COUNT;++i)
                    {
                        if(CollapsingHeader(getNum(i)))
                            drawMesh(*game->getAssetManager().getMesh(i));
                    }
                    EndTabItem();
                }
                //endregion
                //region textures
                if (BeginTabItem("Textures"))
                {
                    for(unsigned int i = 0;i<TextureIds::TEXTURE_COUNT;++i)
                    {
                        if(CollapsingHeader(getNum(i)))
                            drawTexture(*game->getAssetManager().getTexture(i));
                    }
                    EndTabItem();
                }
                //endregion
                //region cube maps
                if (BeginTabItem("Cube Maps"))
                {
                    for(unsigned int i = 0;i<CubeMapIds::CUBE_MAP_COUNT;++i)
                    {
                        if(CollapsingHeader(getNum(i)))
                            drawCubeMap(*game->getAssetManager().getCubeMap(i));
                    }
                    EndTabItem();
                }
                //endregion
                //region Materials
                if (BeginTabItem("Materials"))
                {
                    for(unsigned int i = 0;i<MaterialIds::MATERIAL_COUNT;++i)
                    {
                        if(TreeNode(getNum(i)))
                        {
                            drawMaterial(*game,*game->getAssetManager().getMaterial(i));
                            TreePop();
                        }
                    }
                    EndTabItem();
                }
                //endregion
                //region Shaders
                if (BeginTabItem("Shaders"))
                {
                    for(unsigned int i = 0;i<ShaderIds::SHADER_COUNT;++i)
                    {
                        if(CollapsingHeader(getNum(i)))
                            TextUnformatted(game->getAssetManager().getShader(i)->sourceCode.c_str());
                    }
                    EndTabItem();
                }
                //endregion
                //region Shader Programs
                if (BeginTabItem("Shader Programs"))
                {
                    for(unsigned int i = 0;i<ShaderProgramIds::SHADER_PROGRAM_COUNT;++i)
                    {
                        if(TreeNode(getNum(i)))
                        {
                            drawShaderProgram(*game,*game->getAssetManager().getShaderProgram(i));
                            TreePop();
                        }
                    }
                    EndTabItem();
                }
                //endregion
                //region Bitmap Fonts
                if (BeginTabItem("Bitmap Fonts"))
                {
                    for (unsigned int i = 0; i < BitmapFontIds::BITMAP_FONT_COUNT; ++i)
                    {
                        if (CollapsingHeader(getNum(i)))
                            drawBitmapFont(*game->getAssetManager().getBitmapFont(i));
                    }
                    EndTabItem();
                }
                //endregion

                EndTabBar();
                End();
            }
        }
        //endregion
        //region App settings
        if (isWindowEnabled(DebugWindowIds::APP_SETTINGS))
        {
            Begin("App Settings (4)");

            AppSettings& settings = game->settings;

            if(Button("Apply Settings"))
                game->applySettings();

            if(CollapsingHeader("Display"))
            {
                PushItemWidth(128);
                InputInt("Window Width",&(settings.display.windowWidth),1);
                SameLine();
                InputInt("Height",&settings.display.windowHeight,1);
                PopItemWidth();

                Checkbox("Fullscreen",&settings.display.fullScreen);
                SliderFloat("Brightness",&settings.display.brightnessFactor,0.25,3);
                SliderInt("Refresh Rate",&settings.display.refreshRate,0,240);
            }
            if(CollapsingHeader("Gameplay"))
            {
                if(BeginCombo("Viewport Mode",GameplaySettings::VIEWPORT_MODE_NAMES[settings.gameplay.viewportMode]))
                {
                    for(char mode = 0;mode<GameplaySettings::VIEWPORT_MODE_COUNT;++mode)
                    {
                        if(Selectable(GameplaySettings::VIEWPORT_MODE_NAMES[mode],mode==settings.gameplay.viewportMode))
                            settings.gameplay.viewportMode = static_cast<GameplaySettings::ViewportMode>(mode);
                    }
                    EndCombo();
                }

                float horFov = settings.gameplay.horFov*TO_DEGREES;
                if(InputFloat("Horizontal FOV",&horFov,1))
                    settings.gameplay.horFov = horFov*TO_RADIANS;
            }

            End();
        }
        //endregion

        //Generate Render data and render using opengl implementation
        Render();
        ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
    }
};
REGISTERCLASS(ImGuiDebugGUI)