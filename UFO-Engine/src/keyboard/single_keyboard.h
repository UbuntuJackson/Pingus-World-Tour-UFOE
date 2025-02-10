#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "../ufo_input/ufo_input.h"
#include "../external/olcPixelGameEngine.h"
class SingleKeyboard{
public:
    ButtonState GetKey(olc::Key _button);

    static SingleKeyboard& Get(){
        static SingleKeyboard single_keyboard;
        return single_keyboard;
    }
};

#endif