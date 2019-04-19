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

    //region components viewer
    //bool componentTabsOpen[Components::COMPONENT_COUNT]={true};
    ComponentId componentIndices[Components::COMPONENT_COUNT]={0};
    //endregion

    void drawMat(const glm::mat4x4& mat)
    {
        glm::mat4 transposed = glm::transpose(mat);
        ImGui::InputFloat4("",&transposed[0][0]);
        ImGui::InputFloat4("",&transposed[1][0]);
        ImGui::InputFloat4("",&transposed[2][0]);
        ImGui::InputFloat4("",&transposed[3][0]);
    }
public:

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
        if(isWindowEnabled(DebugWindowIds::LOGGER))
        {
            ImGui::Begin("Logger (1)");
            int action = ImGuiAl::Log::getInstance().Draw();
            ImGui::End();
        }
        //endregion
        //region component viewer
        if(isWindowEnabled(DebugWindowIds::COMPONENT_VIEWER))
        {
            ImGui::Begin("ComponentViewer");
            ImGui::BeginTabBar("Components",ImGuiTabBarFlags_None);
            //region transform components
            if(ImGui::BeginTabItem("Transforms"))
            {
                ImGui::Text("Active Components: %d",game->transformComps.getNumActive());

                ComponentId step = 1;
                ImGui::InputScalar("Chosen Component",ImGuiDataType_S16,&componentIndices[Components::TRANSFORM],&step);

                //clamp value
                componentIndices[Components::TRANSFORM] = std::clamp<ComponentId>(componentIndices[Components::TRANSFORM],0,game->transformComps.getNumActive());

                //render transform component
                if(game->transformComps.getNumActive()>0)
                {
                    TransformComponent& transform = game->transformComps[componentIndices[Components::TRANSFORM]];
                    glm::vec3 pos = transform.getTranslation();
                    if(ImGui::InputFloat3("Position: ",glm::value_ptr(pos)))
                        transform.setTranslation(pos);
                    glm::vec3 rot = transform.getRotation();
                    if(ImGui::InputFloat3("Rotation: ",glm::value_ptr(rot)))
                        transform.setRotation(rot);
                    glm::vec3 scale = transform.getScaling();
                    if(ImGui::InputFloat3("Scale: ",glm::value_ptr(scale)))
                        transform.setScaling(scale);

                    ImGui::Text("Local Matrix");
                    drawMat(transform.getTransformationMatrix());

                    ImGui::Text("Global Matrix");
                    drawMat(transform.getGlobalTransform());
                }

                ImGui::EndTabItem();
            }
            //endregion
            ImGui::EndTabBar();
            ImGui::End();
        }
        //endregion

        //Generate Render data and render using opengl implementation
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
};
REGISTERCLASS(ImGuiDebugGUI)