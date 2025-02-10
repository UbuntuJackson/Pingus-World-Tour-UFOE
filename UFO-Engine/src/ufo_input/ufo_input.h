#ifndef UFO_INPUT_H
#define UFO_INPUT_H
#include "../external/olcPixelGameEngine.h"
struct ButtonState{
    bool is_held;
    bool is_pressed;
    bool is_released;
};

ButtonState To_ufoButtonState(olc::HWButton _b);

#endif