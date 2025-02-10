#include "ufo_input.h"

ButtonState To_ufoButtonState(olc::HWButton _b){
    return ButtonState{
        _b.bHeld,
        _b.bPressed,
        _b.bReleased
    };
}