#include <RuntimeObjectSystem.h>
#include "Game.h"
#include "StdioLogSystem.h"
#include "Constants.h"
#include "util/RuntimeCompileUtils.h"
#include "RuntimeClasses.h"

template<class T, Components::Types TYPE_ID>
Game::ComponentStore<T, TYPE_ID>::ComponentStore(Game &game) : game(game)
{

}

template<class T, Components::Types TYPE_ID>
T &Game::ComponentStore<T, TYPE_ID>::addComp(EntityId entityId)
{
    //TODO only check in debug mode
    if(numActive==Components::MAX_SIZES[TYPE_ID])
    {
        ImGuiAl::Log::getInstance().Warn("Could not create component of id %d, maximum reached, plz increase max in Components.h and recompile",TYPE_ID);
        return invalidComponent;
    }

    Entity& entity = game.entities[entityId];

    //set index of component in entity object
    entity.components[TYPE_ID]=numActive;

    //set component bit to 1
    entity.componentMask|=Components::typeToMask(TYPE_ID);

    //return component at numActive position, and increase active components
    T& ret = components[numActive++];
    ret.entityId = entity;
    return ret;
}

template<class T, Components::Types TYPE_ID>
void Game::ComponentStore<T, TYPE_ID>::removeComp(EntityId entityId)
{
    Entity& entity = game.entities[entityId];
    ComponentId pos = entity.components[TYPE_ID];

    //TODO only check in debug mode
    if((entity.componentMask&Components::typeToMask(TYPE_ID))==0)
    {
        ImGuiAl::Log::getInstance().Warn("Cannot remove component from entity with id %d, entity has no component of type %d",entity,TYPE_ID);
        return ;
    }

    //set component bit to zero
    entity.componentMask&=~Components::typeToMask(TYPE_ID);

    //fill hole
    components[pos] = components[--numActive];//TODO think about moving component instead of copying
}

template<class T, Components::Types TYPE_ID>
Components::Types Game::ComponentStore<T, TYPE_ID>::getType() const
{
    return TYPE_ID;
}

template<class T, Components::Types TYPE_ID>
auto Game::ComponentStore<T, TYPE_ID>::begin()
{
    return std::begin(components);
}

template<class T, Components::Types TYPE_ID>
auto Game::ComponentStore<T, TYPE_ID>::end()
{
    return begin()+numActive;
}

template<class T, Components::Types TYPE_ID>
constexpr auto Game::ComponentStore<T, TYPE_ID>::begin() const
{
    return std::cbegin(components);
}

template<class T, Components::Types TYPE_ID>
constexpr auto Game::ComponentStore<T, TYPE_ID>::end() const
{
    return begin()+numActive;
}

template<class T, Components::Types TYPE_ID>
T & Game::ComponentStore<T, TYPE_ID>::operator[](EntityId entityId)
{
    //TODO think about checking whether given entity contains component and return invalidComponent if entity does not have a component of TYPE_ID
    return components[game.entities[entityId].components[TYPE_ID]];
}

template<class T, Components::Types TYPE_ID>
const T & Game::ComponentStore<T, TYPE_ID>::operator[](EntityId entityId) const
{
    //TODO think about checking whether given entity contains component and return invalidComponent if entity does not have a component of TYPE_ID
    return components[game.entities[entityId].components[TYPE_ID]];
}


Game::Game():meshComps(*this),assetManager(),soloud(),transformComps(*this)
{
    //assert correctness of component store array
    //TODO only in debug mode?
    size_t componentStoreSize = std::size(componentStores);
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
    gameRendererId = RuntimeCompileUtils::constructObject(runtimeObjectSystem, RuntimeClassNames::GAME_RENDERER, &gameRenderer);
    gameUpdaterId = RuntimeCompileUtils::constructObject(runtimeObjectSystem, RuntimeClassNames::GAME_UPDATER, &gameUpdater);
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

    gameUpdater->update(*this);

    //remove entities marked for deletion
    deleteMarkedEntities();

    return true;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    int width,height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    //render game world
    gameRenderer->render(*this);

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

    if(runtimeObjectSystem!= nullptr) {
        runtimeObjectSystem->CleanObjectFiles();
        if(runtimeObjectSystem->GetObjectFactorySystem())
            runtimeObjectSystem->GetObjectFactorySystem()->RemoveListener(this);
    }

    delete runtimeObjectSystem;
}

EntityId Game::createEntity()
{
    //get free id
    EntityId id;
    if(freeCount>0)
    {
        std::pop_heap(std::begin(freeIds), std::begin(freeIds) + freeCount,std::greater<>());
        id = freeIds[--freeCount];//smallest id is now at the end
    }
    else
        id=endActives++;

    entities[id].deleted=false;

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

        //add id to free id list
        std::push_heap(std::begin(freeIds), std::begin(freeIds) + freeCount,std::greater<>());
        ++freeCount;

        //if deleted entity was the last (=on position endActive-1), change endActive accordingly
        if(id==endActives-1)
        {
            do
            {
                endActives--;
            }while(endActives>=0 && entities[endActives].deleted);
            ++endActives;//point one past the last active entity
        }
    }
}

void Game::OnConstructorsAdded()
{
    //reload runtime swappable members e.g. EventManager
	RuntimeCompileUtils::updateObject(runtimeObjectSystem, eventManagerID, &eventManager);
    RuntimeCompileUtils::updateObject(runtimeObjectSystem, debugGuiID, &debugGui);
    RuntimeCompileUtils::updateObject(runtimeObjectSystem, gameRendererId, &gameRenderer);
    RuntimeCompileUtils::updateObject(runtimeObjectSystem, gameUpdaterId, &gameUpdater);
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
