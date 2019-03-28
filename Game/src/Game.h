//
// Created by simon on 21.03.19.
//

#ifndef CGUE19_THECHOSENFROG_GAME_H
#define CGUE19_THECHOSENFROG_GAME_H


#include "assets/AssetManager.h"
#include "IEventManager.h"

class Game : public IObjectFactoryListener {
private:
    AssetManager assetManager;
	
    //audio module
    SoLoud::Soloud soloud;

    //runtime compiled cpp systems
    IRuntimeObjectSystem* runtimeObjectSystem;
    ICompilerLogger* compilerLogger;

	//swappable classes
	IEventManager* eventManager;
	ObjectId eventManagerID;


public:
    Game();
    virtual ~Game();

    void init();
    /**
     * @return true if game has not finished
     */
    bool update();
    void render();
    void cleanup();


    void OnConstructorsAdded();
	IEventManager* getEventManager() const;

};


#endif //CGUE19_THECHOSENFROG_GAME_H
