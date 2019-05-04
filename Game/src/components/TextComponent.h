#ifndef CGUE19_THECHOSENFROG_TEXTCOMPONENT_H
#define CGUE19_THECHOSENFROG_TEXTCOMPONENT_H


#include <string>
#include "../assets/Assets.h"
#include "Component.h"
#include "../util/Color.h"

class TextComponent : public Component
{
public:
    std::string text;
    BitmapFontAsset* font;
    Color color={255,255,255,255};
    //negative values disables wrapping
    float wrapWidth = -1;
    float lineHeight=2;//space between lines
    bool inScreenspace = false;
};


#endif //CGUE19_THECHOSENFROG_TEXTCOMPONENT_H
