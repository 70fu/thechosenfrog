#ifndef CGUE19_THECHOSENFROG_MATERIALCOMPONENT_H
#define CGUE19_THECHOSENFROG_MATERIALCOMPONENT_H

#include "../assets/Assets.h"
#include "Component.h"
#include "../assets/Material.h"

class MaterialComponent : public Component
{
public:
    /**
     * override properties from material asset
     */
    MaterialData instanceProp;
    MaterialAsset* material;
};


#endif //CGUE19_THECHOSENFROG_MATERIALCOMPONENT_H
