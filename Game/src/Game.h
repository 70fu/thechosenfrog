//
// Created by simon on 21.03.19.
//

#ifndef CGUE19_THECHOSENFROG_GAME_H
#define CGUE19_THECHOSENFROG_GAME_H

#include "assets/AssetManager.h"
#include "IEventManager.h"
#include "logger/imguial_log.h"
#include "IDebugGUI.h"
#include "components/MeshComponent.h"
#include "components/Components.h"
#include "components/TransformComponent.h"
#include "IGameRenderer.h"
#include "IGameUpdater.h"
#include "components/MaterialComponent.h"
#include "components/CameraComponent.h"
#include "scenes/IScene.h"
#include "components/CameraControllerComponent.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Physics.h"
#include "components/PhysicsComponent.h"
#include "components/CharControllerComponent.h"
#include "components/PlayerComponent.h"
#include "components/TextComponent.h"
#include "AppSettings.h"

class GLFWwindow;


class Game : public IObjectFactoryListener {
public:
	/**
	 * members every component store should have
	 */
	class ComponentStoreBase
	{
	public:
		/**
		 * Precondition: entity with given id contains component of type getType()
		 * Removes component that this store manages from given entity
		 * component bit of componentMask of given entity is set to 0 and last component of store is moved to position of the component of this entity
		 * @param entityId of entity to remove component from
		 */
		virtual void removeComp(EntityId entityId)=0;
		/**
		 * Just used for an assert check in the game constructor
		 * @return the type of components this component store manages
		 */
		virtual Components::Types getType() const=0;
	};

	/**
	 * stores and manages components of a given type in a contiguous array
	 * @tparam T, Type of component this store manages
	 * @tparam TYPE_ID, id of managed component type
	 */
    template<class T, Components::Types TYPE_ID>
    class ComponentStore : public ComponentStoreBase
    {
        friend class Game;
    private:
        T components[Components::MAX_SIZES[TYPE_ID]];
        T invalidComponent;//returned on invalid requests
        ComponentId numActive = 0;

        Game& game;
    public:
		explicit ComponentStore(Game& game):game(game) {}

		ComponentId getNumActive() const {return numActive;}

		/**
		 * Adds component to entity
		 * component bit of componentMask of given entity is set to 1
		 * Simply returns component if it is already attached
		 * @param entityId, id of entity to add component to
		 * @return returns a reference to the component attached to given entity to configure it
		 */
        T& addComp(EntityId entityId)
        {
            //TODO only check in debug mode
            if(numActive==Components::MAX_SIZES[TYPE_ID])
            {
                ImGuiAl::Log::getInstance().Warn("Could not create component of id %d, maximum reached, plz increase max in Components.h and recompile",TYPE_ID);
                return invalidComponent;
            }

            Entity& entity = game.entities[entityId];

            //check if component is already attached and return it
            if((entity.componentMask&Components::typeToMask(TYPE_ID))!=0)
			{
				return components[entity.components[TYPE_ID]];
			}

            //set index of component in entity object
            entity.components[TYPE_ID]=numActive;

            //set component bit to 1
            entity.componentMask|=Components::typeToMask(TYPE_ID);

            //return component at numActive position, and increase active components
            T& ret = components[numActive++];
            ret.entity = entityId;

            //notify event manager
            game.eventManager->componentAdded(game,entityId,TYPE_ID);

            return ret;
        }
        void removeComp(EntityId entityId) override
        {
            Entity& entity = game.entities[entityId];
            ComponentId pos = entity.components[TYPE_ID];

            //TODO only check in debug mode
            if((entity.componentMask&Components::typeToMask(TYPE_ID))==0)
            {
                ImGuiAl::Log::getInstance().Warn("Cannot remove component from entity with id %d, entity has no component of type %d",entity,TYPE_ID);
                return ;
            }

			//notify event manager
			game.eventManager->componentPreRemove(game,entityId,TYPE_ID);

            //set component bit to zero
            entity.componentMask&=~Components::typeToMask(TYPE_ID);

            //call cleanup function
            components[pos].cleanup(game);

            //fill hole
            if(numActive>0)
            {
                T& filler = components[--numActive];
                components[pos] = filler;//TODO think about moving component instead of copying
                game.entities[filler.entity].components[TYPE_ID]=pos;
            }
        }

        Components::Types getType() const override {return TYPE_ID;}

        //iterator methods iterate over all active components
        //iterators are invalidated on removal or add of components
        T* begin(){return &components[0];}
        T* end(){return &components[0]+numActive;}
        constexpr T* begin() const{return &components[0];}
        constexpr T* end() const{return &components[0]+numActive;}

        /**
         * Precondition: given entity has component of type TYPE_ID
         * Access component of given entity
         * @param entity
         * @return reference to component of given entity
         */
        T& operator[](EntityId entity)
        {
            //TODO think about checking whether given entity contains component and return invalidComponent if entity does not have a component of TYPE_ID
            return components[game.entities[entity].components[TYPE_ID]];
        }
        const T& operator[](EntityId entity) const
        {
            //TODO think about checking whether given entity contains component and return invalidComponent if entity does not have a component of TYPE_ID
            return components[game.entities[entity].components[TYPE_ID]];
        }
    };
private:
	//index=id, there may be holes of inactive/deleted entities
	Entity entities[MAX_ENTITIES]={};
	/**
	 * One past the last active entity in the entities array
	 */
	EntityId endActives = 0;
	/**
	 * Entities marked for deletion, get deleted after every update
	 */
	EntityId entitiesToDelete[MAX_ENTITIES];
	EntityId deleteCount = 0;
	/**
	 * Contains ids of entities that have been deleted and can be reused for future entities
	 * Managed as a min heap using methods from <iterator> header, since lower ids should be preferred to prevent holes in the entities array
	 */
	EntityId freeIds[MAX_ENTITIES];
	EntityId freeCount = 0;

    AssetManager assetManager;
    const char* loggerActions[1] = {nullptr};//TODO find better place for this
    GLFWwindow* window;

    //audio module
    SoLoud::Soloud soloud;
    //physics module
    Physics physics;

    //runtime compiled cpp systems
    IRuntimeObjectSystem* runtimeObjectSystem;

	//swappable classes
	IEventManager* eventManager;
	ObjectId eventManagerID;

	IDebugGUI* debugGui;
	ObjectId debugGuiID;

	IGameRenderer* gameRenderer;
	ObjectId gameRendererId;

	IGameUpdater* gameUpdater;
	ObjectId gameUpdaterId;

	IScene* mainScene;
	ObjectId mainSceneId;

	bool reloadSceneOnNextFrame = false;

	/* --------------------------------------------- */
	// Input Helper
	/* --------------------------------------------- */
	glm::vec2 lastMousePos;
	glm::vec2 mousePos;
	glm::vec2 mouseScrollDelta;
	bool cursorEnteredWindowThisFrame = false;

	/**
	 * Deletes all entities marked for deletion
	 */
    inline void deleteMarkedEntities();

public:
    /* --------------------------------------------- */
    // Components
    /* --------------------------------------------- */
	ComponentStore<MeshComponent,Components::MESH> meshComps;
	ComponentStore<TransformComponent, Components::TRANSFORM> transformComps;
	ComponentStore<MaterialComponent, Components::MATERIAL> materialComps;
    ComponentStore<CameraComponent, Components::CAMERA> cameraComps;
    ComponentStore<CameraControllerComponent, Components::CAMERA_CONTROLLER> cameraControllerComps;
    ComponentStore<PhysicsComponent,Components::PHYSICS> physicsComps;
    ComponentStore<CharControllerComponent,Components::CHAR_CONTROLLER> charControllerComps;
    ComponentStore<PlayerComponent,Components::PLAYER> playerComps;
    ComponentStore<TextComponent,Components::TEXT> textComps;


    //Global Skybox, must never be nullptr
    CubeMapAsset* activeSkybox;

    AppSettings settings={};
private:
    /**
     * current frame, is incremented at the beginning on every update (thus -1 is immediatly incremented to 0 on the first update)
     */
    long long frame = -1;

    /**
     * This array is used to remove components from entities when an entity is deleted
     * a component store of every component type must be in this array.
     * Component store that manages components with type id T must be at index T
     */
    ComponentStoreBase* componentStores[Components::COMPONENT_COUNT]=
            {
                    &meshComps,
                    &transformComps,
                    &materialComps,
                    &cameraComps,
                    &cameraControllerComps,
                    &physicsComps,
                    &charControllerComps,
                    &playerComps,
                    &textComps
            };

public:
    Game();
    virtual ~Game();

    void init(GLFWwindow* window);
    /**
     * @return true if game has not finished
     */
    bool update();
    void render();
    void cleanup();

    /**
     * Reloads the scene at the beginning of the next frame
     */
    void reloadScene();

    //entity management methods
    /**
     * Creates a new entity
     * @return id of newly created entity
     */
	EntityId createEntity();
	/**
	 * @param entityId
	 * @return true if entity with given id is inactive/deleted or marked for deletion
	 */
	bool isDeleted(EntityId entityId);
	/**
	 * @param entityId
	 * @param componentMask
	 * @return true if entity with given id has all components described by given component mask, false otherwise
	 */
	bool hasComponents(EntityId entityId, Components::ComponentMask componentMask);
	/**
	 * Deletes entity with given id from the system, entity appears as deleted right after a call to this method, but is actually removed from the system after update of the game world
	 * @param entityId
	 */
	void deleteEntity(EntityId entityId);

    void OnConstructorsAdded();

	IEventManager* getEventManager() const;
	IDebugGUI* getDebugGUI() const;
	AssetManager& getAssetManager();
	GLFWwindow* getWindow();
	SoLoud::Soloud& getSoloud();
	Physics& getPhysics();


	/* --------------------------------------------- */
	// Input helper
	/* --------------------------------------------- */
	/**
	 * @return mouse position change since last update, returns zero vector if the cursor entered the window this update
	 */
	glm::vec2 getMouseMoveDelta() const;

	/**
	 * @return true if the cursor entered the window this frame
	 */
	bool cursorEnteredWindow() const;

	/**
	 * Called by the event manager
	 * @param enteredWindowThisUpdate
	 */
	void setCursorEnteredWindow(bool enteredWindowThisUpdate);

	/**
	 * @return Current mouse position
	 */
	const glm::vec2& getMousePos() const;

	/**
	 * Sets mouse position, is called by the event manager
	 * @param mousePos
	 */
	void setMousePos(const glm::vec2& mousePos);

	/**
	 * @return mouse scroll wheel change since last update
	 */
	const glm::vec2& getMouseScrollDelta() const;
	/**
	 * Called by the event manager
	 * @param delta
	 */
	void setMouseScrollDelta(const glm::vec2& delta);

	/**
	 * @return the current frame number = how often the game has been updated
	 */
	long long getFrame() const;

	/**
	 * apply the currently configured settings
	 */
	void applySettings();
};


#endif //CGUE19_THECHOSENFROG_GAME_H
