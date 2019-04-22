//
// Created by simon on 31.03.19.
//

#include <ISimpleSerializer.h>
#include "IDebugGUI.h"

void IDebugGUI::Serialize(ISimpleSerializer *pSerializer) {
    IObject::Serialize(pSerializer);

    SERIALIZE(enabled);
}

void IDebugGUI::enableWindow(unsigned int window)
{
    enabled|=window;
}

void IDebugGUI::disableWindow(unsigned int window)
{
    enabled &= ~window;
}

void IDebugGUI::toggleWindow(unsigned int window)
{
    enabled ^= window;
}

bool IDebugGUI::isWindowEnabled(unsigned int window) {
    return (enabled&window)!=0;
}

void IDebugGUI::Init(bool isFirstInit)
{
    IObject::Init(isFirstInit);
}

