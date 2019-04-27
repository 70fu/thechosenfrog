//
// Created by simon on 22.03.19.
//

#ifndef CGUE19_THECHOSENFROG_RUNTIMECOMPILEUTILS_H
#define CGUE19_THECHOSENFROG_RUNTIMECOMPILEUTILS_H

/**
 * This namespace contains utility functions for using RuntimeCompiledCpp
 */
#include <ObjectInterface.h>
#include <RuntimeObjectSystem.h>
#include <IObjectFactorySystem.h>
#include <IObject.h>

using namespace ImGuiAl;

namespace RuntimeCompileUtils
{
    template<class T>
    ObjectId constructObject(IRuntimeObjectSystem* objectSystem, const char* name, T** pointer)
    {
        // construct first object
        IObjectConstructor* pCtor = objectSystem->GetObjectFactorySystem()->GetConstructor(name);
        if( pCtor )
        {
            IObject* pObj = pCtor->Construct();
            pObj->GetInterface( pointer );
            if(pointer==nullptr)
            {
                delete pObj;//why do I delete this only in the error case?? But this is exactly how the ConsoleExample from the RuntimeCompiledCPP looks
                Log::getInstance().Error("Could not construct rccpp object %s",name);
            }
            else
            {
                //initialize
                pObj->Init(true);
                return pObj->GetObjectId();
            }
        }
        else
        {
            Log::getInstance().Error("rccpp object %s does not have a registered constructor",name);
        }

        //return invalid object id
        return ObjectId();
    }

    template<class T>
    bool updateObject(IRuntimeObjectSystem* objectSystem, const ObjectId& objectId, T** pointer)
    {
        T* previous = *pointer;
        IObject* pObj = objectSystem->GetObjectFactorySystem()->GetObject( objectId );
        pObj->GetInterface( pointer);
        if( *pointer== nullptr )
        {
            Log::getInstance().Error("Could not fetch new instance for rccpp object of type %s",typeid(T).name());
            delete pObj;
        }

        return previous!=*pointer;
    }
};

#endif //CGUE19_THECHOSENFROG_RUNTIMECOMPILEUTILS_H
