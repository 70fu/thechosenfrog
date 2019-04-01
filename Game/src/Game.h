//
// Created by simon on 21.03.19.
//

#ifndef CGUE19_THECHOSENFROG_GAME_H
#define CGUE19_THECHOSENFROG_GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "assets/AssetManager.h"
#include "IEventManager.h"
#include "log/imguial_log.h"
#include "IDebugGUI.h"

class Game : public IObjectFactoryListener {
private:
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


    void OnConstructorsAdded();

	IEventManager* getEventManager() const;
	IDebugGUI* getDebugGUI() const;
	AssetManager& getAssetManager();
	GLFWwindow* getWindow();

};


#endif //CGUE19_THECHOSENFROG_GAME_H
