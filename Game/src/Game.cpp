#include <RuntimeObjectSystem.h>
#include "Game.h"
#include "StdioLogSystem.h"
#include "Constants.h"
#include "util/RuntimeCompileUtils.h"
#include "RuntimeClasses.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <PxScene.h>

Game::Game():assetManager(),soloud(),meshComps(*this),transformComps(*this),materialComps(*this),cameraComps(*this),cameraControllerComps(*this),physicsComps(*this),charControllerComps(*this),playerComps(*this),textComps(*this)
{
    //assert correctness of component store array
    //TODO only in debug mode?
    size_t componentStoreSize = Components::COMPONENT_COUNT;
    for(int i = 0;i<componentStoreSize;++i)
    {
        if(componentStores[i]==nullptr)
        {
            Log::getInstance().Error("There is a nullptr at position %d in the component stores array, maybe not all component stores are in the array. Change Game::componentStores",i,Components::Types::COMPONENT_COUNT);
            break;
        }

        Components::Types componentStoreType = componentStores[i]->getType();
        if(i!=componentStoreType)
            Log::getInstance().Error("Component store of type %d is at position %d in Game::componentStores, type and index should be the same",componentStoreType,i);
    }
}

Game::~Game() {
    //TODO put code of cleanup here?? or call cleanup if it has not been called??
}

void Game::init(GLFWwindow* window)
{
    this->window=window;

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
    FileSystemUtils::Path gladIncludePath = basePath / "Game" / "libs" / "glad" / "include";
    runtimeObjectSystem->AddIncludeDir(gladIncludePath.c_str());
    FileSystemUtils::Path glmIncludePath = basePath / "Game" / "libs" / "glm";
    runtimeObjectSystem->AddIncludeDir(glmIncludePath.c_str());
    FileSystemUtils::Path imguiIncludePath = basePath / "Game" / "libs" / "imgui";
    runtimeObjectSystem->AddIncludeDir(imguiIncludePath.c_str());

    //specify library directories
    FileSystemUtils::Path soloudLibPath = basePath / "cmake-build-debug" / "soloud";
    runtimeObjectSystem->AddLibraryDir(soloudLibPath.c_str());

    //init audio module
    soloud.init();

    //init physx module
    physics.init();

    //init asset manager to load all assets
    assetManager.init(runtimeObjectSystem);

    //construct runtime swappable members, e.g. EventManager
	eventManagerID = RuntimeCompileUtils::constructObject(runtimeObjectSystem, RuntimeClassNames::EVENT_MANAGER, &eventManager);
    debugGuiID = RuntimeCompileUtils::constructObject(runtimeObjectSystem, RuntimeClassNames::IMGUI_DEBUG_GUI, &debugGui);
    gameRendererId = RuntimeCompileUtils::constructObject(runtimeObjectSystem, RuntimeClassNames::GAME_RENDERER, &gameRenderer);
    gameUpdaterId = RuntimeCompileUtils::constructObject(runtimeObjectSystem, RuntimeClassNames::GAME_UPDATER, &gameUpdater);
    mainSceneId = RuntimeCompileUtils::constructObject(runtimeObjectSystem, RuntimeClassNames::MAIN_SCENE, &mainScene);
    //...

    //set physics simulation callback
    physics.getScene()->setSimulationEventCallback(eventManager);

    //init debug gui
    debugGui->init(this);

    //apply settings
    applySettings();

    //init scene
    mainScene->init(*this);
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

    //increase frame count
    ++frame;

    if(reloadSceneOnNextFrame)
    {
        //delete all entities
        for(EntityId i = MAX_ENTITIES-1;i>=0;--i)
            deleteEntity(i);
        deleteMarkedEntities();

        //load scene
        mainScene->init(*this);

        reloadSceneOnNextFrame = false;
    }

    assetManager.update();

    gameUpdater->update(*this);

    //remove entities marked for deletion
    deleteMarkedEntities();

    //reset mouse scroll delta
    mouseScrollDelta = glm::vec2(0,0);

    //reset mouse move delta
    lastMousePos = mousePos;

    //reset mouse enter event
    cursorEnteredWindowThisFrame = false;

    return true;
}

void Game::render()
{
    //obtain size of window
    int width,height;
    glfwGetFramebufferSize(window, &width, &height);

    //render game world
    gameRenderer->render(*this,width,height);

    //render debug gui
    debugGui->render(this);

    //swap buffer after everything has rendered
    glfwSwapBuffers(window);
}

void Game::cleanup()
{
    debugGui->cleanup(this);

    assetManager.cleanup();
    soloud.deinit();
    physics.cleanup();

    if(runtimeObjectSystem!= nullptr) {
        runtimeObjectSystem->CleanObjectFiles();
        if(runtimeObjectSystem->GetObjectFactorySystem())
            runtimeObjectSystem->GetObjectFactorySystem()->RemoveListener(this);
    }

    delete runtimeObjectSystem;
}

void Game::reloadScene()
{
    reloadSceneOnNextFrame = true;
}

EntityId Game::createEntity()
{
    //get free id
    EntityId id;
    if(freeCount>0)
    {
        std::pop_heap(std::begin(freeIds), std::begin(freeIds) + freeCount,std::greater<>());
        id = freeIds[--freeCount];//smallest id is now at the end

        //check if ids in heap are bigger than biggest active entity id
        if(id>=endActives)
        {
            //clear heap
            freeCount=0;
            //simply take next id
            id=endActives++;
        }
    }
    else
        id=endActives++;

    entities[id].deleted=false;

    //notify event manager
    eventManager->entityAdded(*this,id);

    return id;
}

bool Game::isDeleted(EntityId entityId)
{
    return entities[entityId].deleted;
}

bool Game::hasComponents(EntityId entity, Components::ComponentMask componentMask)
{
    return (entities[entity].componentMask&componentMask)==componentMask;
}

void Game::deleteEntity(EntityId entityId)
{
    Entity& entity = entities[entityId];

    if(!entity.deleted)
    {
        //mark for deletion
        entitiesToDelete[deleteCount++] = entityId;

        //mark entity as deleted
        entity.deleted = true;

        //notify event manager
        eventManager->entityPreRemove(*this,entityId);
    }
}

void Game::deleteMarkedEntities()
{
    for(int i = 0;i<deleteCount;++i)
    {
        EntityId id = entitiesToDelete[i];
        Entity& entity = entities[id];

        //iterate over every component type and remove from respective component store if entity has component of given type
        Components::ComponentMask mask = 1;
        for(int j = 0;j<Components::Types::COMPONENT_COUNT;++j,mask<<=1)
        {
            if((entity.componentMask&mask)!=0)
                componentStores[j]->removeComp(id);
        }

        //reset component mask
        entity.componentMask=0;

        //check if deleted entity was the last (=on position endActive-1)
        if(id==endActives-1)
        {
            //change endActive accordingly
            do
            {
                endActives--;
            }while(endActives>=0 && entities[endActives].deleted);
            ++endActives;//point one past the last active entity
        }
        else if(id<endActives)
        {
            //add id to free id list
            freeIds[freeCount++]=id;
            std::push_heap(std::begin(freeIds), std::begin(freeIds) + freeCount,std::greater<>());
        }
    }

    //everything deleted
    deleteCount = 0;
}

void Game::OnConstructorsAdded()
{
    //reload runtime swappable members e.g. EventManager
	if(RuntimeCompileUtils::updateObject(runtimeObjectSystem, eventManagerID, &eventManager))
    {
        //set physics simulation callback
        physics.getScene()->setSimulationEventCallback(eventManager);
    }
    RuntimeCompileUtils::updateObject(runtimeObjectSystem, debugGuiID, &debugGui);
    RuntimeCompileUtils::updateObject(runtimeObjectSystem, gameRendererId, &gameRenderer);
    RuntimeCompileUtils::updateObject(runtimeObjectSystem, gameUpdaterId, &gameUpdater);
    RuntimeCompileUtils::updateObject(runtimeObjectSystem, mainSceneId, &mainScene);
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

SoLoud::Soloud &Game::getSoloud()
{
    return soloud;
}

Physics &Game::getPhysics()
{
    return physics;
}

glm::vec2 Game::getMouseMoveDelta() const
{
    if(cursorEnteredWindowThisFrame)
        return {0,0};

    return mousePos-lastMousePos;
}

bool Game::cursorEnteredWindow() const
{
    return cursorEnteredWindowThisFrame;
}

void Game::setCursorEnteredWindow(bool enteredWindowThisUpdate)
{
    cursorEnteredWindowThisFrame = enteredWindowThisUpdate;
}

const glm::vec2 &Game::getMousePos() const
{
    return mousePos;
}

void Game::setMousePos(const glm::vec2 &newMousePos)
{
    mousePos = newMousePos;
}

const glm::vec2& Game::getMouseScrollDelta() const
{
    return mouseScrollDelta;
}

void Game::setMouseScrollDelta(const glm::vec2 &delta)
{
    mouseScrollDelta = delta;
}

long long Game::getFrame() const
{
    return frame;
}

void Game::applySettings()
{
    glfwSetWindowMonitor(window,settings.display.fullScreen?glfwGetPrimaryMonitor():nullptr,0,0,settings.display.windowWidth,settings.display.windowHeight,settings.display.refreshRate);

    eventManager->settingsChanged(*this);
}
