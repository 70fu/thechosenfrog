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

class ImGuiDebugGUI : public TInterface<RuntimeClassIds ::IMGUI_DEBUG_GUI,IDebugGUI>
{
public:
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
        if(isWindowEnabled(DebugWindowIds::LOGGER))
        {
            ImGui::Begin("Logger (1)");
            int action = ImGuiAl::Log::getInstance().Draw();
            ImGui::End();
        }

        //Generate Render data and render using opengl implementation
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
};
REGISTERCLASS(ImGuiDebugGUI)