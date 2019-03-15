//
// Created by simon on 14.03.19.
//

#ifndef CGUE19_THECHOSENFROG_UPDATEABLE_H
#define CGUE19_THECHOSENFROG_UPDATEABLE_H


#include <IObject.h>
#include "RuntimeTypeIds.h"

class Updateable : public TInterface<UPDATEABLE,IObject>{
public:
    virtual void Update(float delta)=0;
};

#endif //CGUE19_THECHOSENFROG_UPDATEABLE_H
