//
// Created by simon on 07.03.19.
//

#include "ConsoleGame.h"
#include "StdioLogSystem.h"
#include "util/RuntimeCompileUtils.h"
#include <RuntimeObjectSystem.h>
#include <thread>
#include <chrono>
#include <iostream>

ConsoleGame::ConsoleGame() {

}

ConsoleGame::~ConsoleGame() {
    if(objectSystem!= nullptr) {
        objectSystem->CleanObjectFiles();
        if(objectSystem->GetObjectFactorySystem())
            objectSystem->GetObjectFactorySystem()->RemoveListener(this);
    }

    delete objectSystem;
    delete compilerLogger;
}

void ConsoleGame::Init() {
    compilerLogger = new StdioLogSystem();

    objectSystem = new RuntimeObjectSystem();
    if(!objectSystem->Initialise(compilerLogger, nullptr))
    {
        compilerLogger->LogError("Failed to init object system");
    }
    objectSystem->GetObjectFactorySystem()->AddListener(this);

    // construct first object
    //updateableId = RuntimeCompileUtils::constructObject(objectSystem,"ConsoleUpdate",updateable);
}

void ConsoleGame::Run() {
    while(true) {
        //check status of any compile
        if (objectSystem->GetIsCompiledComplete()) {
            // load module when compile complete
            objectSystem->LoadCompiledModule();
        }

        if (!objectSystem->GetIsCompiling()) {

            const float deltaTime = 1.0f;
            objectSystem->GetFileChangeNotifier()->Update(deltaTime);
            updateable->Update(deltaTime);
            std::this_thread::sleep_for(std::chrono::duration<float,std::milli>(deltaTime*1000));
        }
    }
}

void ConsoleGame::OnConstructorsAdded() {
    // This could have resulted in a change of object pointer, so release old and get new one.
    if( updateable )
    {
        //RuntimeCompileUtils::updateObject(objectSystem,updateableId,updateable);
    }
}
