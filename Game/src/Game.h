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
		explicit ComponentStore(Game& game) ;

		/**
		 * Precondition: entity does not have component of the type this store manages
		 * Adds component to entity
		 * component bit of componentMask of given entity is set to 1
		 * @param entity, id of entity to add component to
		 * @return returns a reference to the component to configure it.
		 */
        T& addComp(EntityId entity);
        void removeComp(EntityId entity) override;

        Components::Types getType() const override;

        //iterator methods iterate over all active components
        //iterators are invalidated on removal or add of components
        auto begin();
        auto end();
        constexpr auto begin() const;
        constexpr auto end() const;

        /**
         * Precondition: given entity has component of type TYPE_ID
         * Access component of given entity
         * @param entity
         * @return reference to component of given entity
         */
        T& operator[](EntityId entity);
        const T& operator[](EntityId entity) const;
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



    /**
     * This array is used to remove components from entities when an entity is deleted
     * a component store of every component type must be in this array.
     * Component store that manages components with type id T must be at index T
     */
	ComponentStoreBase* componentStores[Components::COMPONENT_COUNT]=
			{
    			&meshComps,
				&transformComps,
				&materialComps
			};

    AssetManager assetManager;
    const char* loggerActions[1] = {nullptr};//TODO find better place for this
    GLFWwindow* window;

    //audio module
    SoLoud::Soloud soloud;

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

    /**
	 * Deletes all entities marked for deletion
	 */
    inline void deleteMarkedEntities();
public:
	//components
	ComponentStore<MeshComponent,Components::MESH> meshComps;
	ComponentStore<TransformComponent, Components::TRANSFORM> transformComps;
	ComponentStore<MaterialComponent, Components::MATERIAL> materialComps;

    Game();
    virtual ~Game();

    void init(GLFWwindow* window);
    /**
     * @return true if game has not finished
     */
    bool update();
    void render();
    void cleanup();

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
};


#endif //CGUE19_THECHOSENFROG_GAME_H
