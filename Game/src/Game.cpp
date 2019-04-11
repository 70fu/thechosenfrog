//
// Created by simon on 21.03.19.
//

#include <RuntimeObjectSystem.h>
#include "Game.h"
#include "StdioLogSystem.h"
#include "Constants.h"
#include "util/RuntimeCompileUtils.h"
#include "RuntimeClasses.h"
#include "TransformComponent.h"

Game::Game():assetManager(),soloud() {

}

Game::~Game() {
    //TODO put code of cleanup here?? or call cleanup if it has not been called??
}

void Game::init(GLFWwindow* window)
{
    this->window=window;


	// creation of transformcomponent test
	TransformComponent t1({ 1,2,3 }, { 0,30,0 }, { 7,8,9 }); // rotation vector y x z


    //init logger
    ImGuiAl::Log::getInstance().Init(ImGuiAl::Log::kShowFilters,loggerActions);

    //init runtime compiled cpp classes
    runtimeObjectSystem = new RuntimeObjectSystem();
    if(!runtimeObjectSystem->Initialise(&ImGuiAl::Log::getInstance(), nullptr))
    {
        ImGuiAl::Log::getInstance().LogError("Failed to init object system");
    }
    runtimeObjectSystem->GetObjectFactorySystem()->AddListener(this);

	// specify include directories
	FileSystemUtils::Path basePath = runtimeObjectSystem->FindFile((__FILE__)).ParentPath().ParentPath().ParentPath();

	FileSystemUtils::Path GLFW_includePath = basePath / "glfw" / "include";
	runtimeObjectSystem->AddIncludeDir(GLFW_includePath.c_str());
    FileSystemUtils::Path soloudIncludePath = basePath / "soloud" / "include";
    runtimeObjectSystem->AddIncludeDir(soloudIncludePath.c_str());

    //specify library directories
    FileSystemUtils::Path soloudLibPath = basePath / "cmake-build-debug" / "soloud";
    runtimeObjectSystem->AddLibraryDir(soloudLibPath.c_str());

    //init audio module
    soloud.init();

    //init asset manager to load all assets
    assetManager.init(runtimeObjectSystem);

    //construct runtime swappable members, e.g. EventManager
	eventManagerID = RuntimeCompileUtils::constructObject(runtimeObjectSystem, RuntimeClassNames::EVENT_MANAGER, &eventManager);
    debugGuiID = RuntimeCompileUtils::constructObject(runtimeObjectSystem, RuntimeClassNames::IMGUI_DEBUG_GUI, &debugGui);
    //...

    //init debug gui
    debugGui->init(this);
}

bool Game::update(){

#ifndef NDEBUG
    //update RuntimeCompiledCpp file watcher to see if files need to be recompiled and do so in that case
    //check status of any compile
    if (runtimeObjectSystem->GetIsCompiledComplete()) {
        // load module when compile complete
        runtimeObjectSystem->LoadCompiledModule();
    }

    //if there are things compiling, do not update anything else
    if (runtimeObjectSystem->GetIsCompiling()) {
        return true;//TODO if we want to implement input recording & replay, the main method has to know that the game was not updated, because some things got recompiled, maybe it is possible to update the game even though RCCPP is compiling (i don't know)
    }
    runtimeObjectSystem->GetFileChangeNotifier()->Update(FIXED_DELTA);
#endif

    assetManager.update();

    //TODO update game world

    return true;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int width,height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    debugGui->render(this);

    //swap buffer after everything has rendered
    glfwSwapBuffers(window);
}

void Game::cleanup()
{
    debugGui->cleanup(this);

    assetManager.cleanup();
    soloud.deinit();

    if(runtimeObjectSystem!= nullptr) {
        runtimeObjectSystem->CleanObjectFiles();
        if(runtimeObjectSystem->GetObjectFactorySystem())
            runtimeObjectSystem->GetObjectFactorySystem()->RemoveListener(this);
    }

    delete runtimeObjectSystem;
}

void Game::OnConstructorsAdded()
{
    //reload runtime swappable members e.g. EventManager
	RuntimeCompileUtils::updateObject(runtimeObjectSystem, eventManagerID, &eventManager);
    RuntimeCompileUtils::updateObject(runtimeObjectSystem, debugGuiID, &debugGui);

}

IEventManager* Game::getEventManager() const
{
	return eventManager;
}

IDebugGUI *Game::getDebugGUI() const {
    return debugGui;
}

AssetManager &Game::getAssetManager() {
    return assetManager;
}

GLFWwindow *Game::getWindow() {
    return window;
}
