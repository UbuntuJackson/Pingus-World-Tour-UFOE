#include "mouse.h"
#include "../external/olcPixelGameEngine.h"
#include "../ufo_maths/ufo_maths.h"
#include "../ufo_input/ufo_input.h"
#include "../ufo_engine/ufo_engine.h"

olc::vf2d Mouse::GetPosition(){
    return Engine::Get().pixel_game_engine.GetMousePos();
}
ButtonState Mouse::GetMiddleButton(){
    return To_ufoButtonState(Engine::Get().pixel_game_engine.GetMouse(2));
}
ButtonState Mouse::GetLeftButton(){
    return To_ufoButtonState(Engine::Get().pixel_game_engine.GetMouse(0));
}
ButtonState Mouse::GetRightButton(){
    return To_ufoButtonState(Engine::Get().pixel_game_engine.GetMouse(1));
}
int Mouse::GetScrollDirection(){
    return ufoMaths::Sign(Engine::Get().pixel_game_engine.GetMouseWheel());
}
void Mouse::Update(){
    Mouse::Get().last_position = Engine::Get().pixel_game_engine.GetMousePos();
}

Mouse& Mouse::Get(){
    static Mouse mouse;
    return mouse;
}

olc::vf2d Mouse::GetDeltaPosition(){
    return Engine::Get().pixel_game_engine.GetMousePos() - Mouse::Get().last_position;
}