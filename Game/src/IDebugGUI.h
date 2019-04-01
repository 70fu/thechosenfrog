//
// Created by simon on 31.03.19.
//

#ifndef CGUE19_THECHOSENFROG_IDEBUGGUI_H
#define CGUE19_THECHOSENFROG_IDEBUGGUI_H

#include <IObject.h>
#include "DebugWindowIds.h"

class Game;

/**
 * Responsible for drawing all debug gui
 */
class IDebugGUI : public IObject
{
public:
    /**
     * Bitmask of enabled debug windows
     */
    unsigned int enabled=0;

    virtual void Serialize(ISimpleSerializer *pSerializer) override;

    //These four functions take a bitmask (see DebugWindowIds for possible values)
    void enableWindow(unsigned int window);
    void disableWindow(unsigned int window);
    void toggleWindow(unsigned int window);
    bool isWindowEnabled(unsigned int window);

    virtual void init(Game* game)=0;
    virtual void render(Game* game)=0;
    virtual void cleanup(Game* game)=0;
};

#endif //CGUE19_THECHOSENFROG_IDEBUGGUI_H
