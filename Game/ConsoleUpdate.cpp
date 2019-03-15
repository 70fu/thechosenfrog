//
// Created by simon on 14.03.19.
//

#ifndef CGUE19_THECHOSENFROG_CONSOLEUPDATE_H
#define CGUE19_THECHOSENFROG_CONSOLEUPDATE_H


#include "Updateable.h"
#include <ObjectInterfacePerModule.h>
#include <iostream>
#include <ISimpleSerializer.h>

class ConsoleUpdate : public Updateable{
    std::string name="Fred";
public:
    void Serialize(ISimpleSerializer* pSerializer) override
    {
        Updateable::Serialize(pSerializer);
        SERIALIZE(name);
    }

    void Update(float delta) override
    {
        std::cout<<"Hello I'm "<<name<<std::endl;
    }
};
REGISTERCLASS(ConsoleUpdate);




#endif //CGUE19_THECHOSENFROG_CONSOLEUPDATE_H
