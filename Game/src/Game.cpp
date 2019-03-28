//
// Created by simon on 21.03.19.
//

#include <RuntimeObjectSystem.h>
#include "Game.h"
#include "StdioLogSystem.h"
#include "Constants.h"
#include "util/RuntimeCompileUtils.h"
#include "RuntimeClasses.h"

Game::Game():assetManager(),soloud() {

}

Game::~Game() {
    //TODO put code of cleanup here?? or call cleanup if it has not been called??
}

void Game::init()
{
    //init runtime compiled cpp classes
    compilerLogger = new StdioLogSystem();

    runtimeObjectSystem = new RuntimeObjectSystem();
    if(!runtimeObjectSystem->Initialise(compilerLogger, nullptr))
    {
        compilerLogger->LogError("Failed to init object system");
    }
    runtimeObjectSystem->GetObjectFactorySystem()->AddListener(this);

	// specify include directories
	FileSystemUtils::Path basePath = runtimeObjectSystem->FindFile((__FILE__));
	FileSystemUtils::Path GLFW_includePath = basePath.ParentPath().ParentPath().ParentPath() / "glfw" / "include";
	runtimeObjectSystem->AddIncludeDir(GLFW_includePath.c_str());


    //init audio module
    soloud.init();

    //init asset manager to load all assets
    assetManager.init(runtimeObjectSystem);

    //TODO construct runtime swappable members, e.g. EventManager
	eventManagerID = RuntimeCompileUtils::constructObject(runtimeObjectSystem, RuntimeClassNames::EVENT_MANAGER, &eventManager);
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

void Game::render() {

}

void Game::cleanup()
{
    assetManager.cleanup();
    soloud.deinit();

    if(runtimeObjectSystem!= nullptr) {
        runtimeObjectSystem->CleanObjectFiles();
        if(runtimeObjectSystem->GetObjectFactorySystem())
            runtimeObjectSystem->GetObjectFactorySystem()->RemoveListener(this);
    }

    delete runtimeObjectSystem;
    delete compilerLogger;
}

void Game::OnConstructorsAdded()
{
    //reload runtime swappable members e.g. EventManager
	RuntimeCompileUtils::updateObject(runtimeObjectSystem, eventManagerID, &eventManager);

}

IEventManager* Game::getEventManager() const
{
	return eventManager;
}
