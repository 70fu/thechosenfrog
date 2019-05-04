#ifndef CGUE19_THECHOSENFROG_PLAYERCOMPONENT_H
#define CGUE19_THECHOSENFROG_PLAYERCOMPONENT_H


#include "Component.h"

class PlayerComponent : public Component
{
public:
    //increase of jump strength if jump key/button is pressed per second
    static constexpr float JUMP_STRENGTH_INCREASE = 0.1;

    /* --------------------------------------------- */
    // Input
    /* --------------------------------------------- */
    enum PlayerInput : unsigned char
    {
        KEYBOARD_MOUSE,
        JOYSTICK
    };

    struct KeyboardInputs
    {
        //default values taken from macro values in glfw3.h
        int jumpKey=32;
        int forwardKey=87;
        int backKey=83;
        int rightKey=68;
        int leftKey=65;

        int tongueKey=-1;//not set
        int tongueMouseButton=0;
    };

    struct JoystickInputs
    {
        //TODO
    };

    union Input
    {
        KeyboardInputs keyboard={};
        JoystickInputs joystick;
    };

    PlayerInput activeInput=KEYBOARD_MOUSE;
    Input input;
};


#endif //CGUE19_THECHOSENFROG_PLAYERCOMPONENT_H
