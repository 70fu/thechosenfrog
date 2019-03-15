//
// Created by simon on 07.03.19.
//

#ifndef CGUE19_THECHOSENFROG_CONSOLEGAME_H
#define CGUE19_THECHOSENFROG_CONSOLEGAME_H

#include <IObjectFactorySystem.h>
#include "Updateable.h"

//forward declarations
struct IRuntimeObjectSystem;

class ConsoleGame : public IObjectFactoryListener{
public:
    ConsoleGame();
    virtual ~ConsoleGame();

    void Init();
    void Run();

    virtual void OnConstructorsAdded();
private:
    IRuntimeObjectSystem* objectSystem;
    ICompilerLogger* compilerLogger;

    Updateable* updateable;
    ObjectId updateableId;
};


#endif //CGUE19_THECHOSENFROG_CONSOLEGAME_H
