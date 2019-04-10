#include "Components.h"

Components::MaskBits Components::typeToMask(Components::Types componentType)
{
    return static_cast<MaskBits>(1<<componentType);
}
