#include "MaterialComponent.h"

void MaterialComponent::retrieveUniformLocations()
{
    instanceProp.retrieveLocations(*material->shader);
}
