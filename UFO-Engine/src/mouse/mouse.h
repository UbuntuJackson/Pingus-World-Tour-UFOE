#ifndef MOUSE_H
#define MOUSE_H

#include "../external/olcPixelGameEngine.h"
#include "../ufo_maths/ufo_maths.h"
#include "../ufo_input/ufo_input.h"
#include "../ufo_engine/ufo_engine.h"

class Mouse{
public:
    olc::vf2d last_position;
    static olc::vf2d GetPosition();
    static ButtonState GetMiddleButton();
    static ButtonState GetLeftButton();
    static ButtonState GetRightButton();
    static int GetScrollDirection();
    void Update();
    
    static Mouse& Get();

    static olc::vf2d GetDeltaPosition();
};

#endif