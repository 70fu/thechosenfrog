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
                //TODO log
            }
            else
                return pObj->GetObjectId();
        }
        else
        {
            //TODO log
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
            //TODO log
            delete pObj;
        }

        return previous!=*pointer;
    }
};

#endif //CGUE19_THECHOSENFROG_RUNTIMECOMPILEUTILS_H
