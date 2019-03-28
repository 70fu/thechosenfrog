//
// Created by simon on 21.03.19.
//

#ifndef CGUE19_THECHOSENFROG_GAME_H
#define CGUE19_THECHOSENFROG_GAME_H


#include "assets/AssetManager.h"

class Game : public IObjectFactoryListener {
private:
    AssetManager assetManager;

    //audio module
    SoLoud::Soloud soloud;

    //runtime compiled cpp systems
    IRuntimeObjectSystem* runtimeObjectSystem;
    ICompilerLogger* compilerLogger;
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
};


#endif //CGUE19_THECHOSENFROG_GAME_H
