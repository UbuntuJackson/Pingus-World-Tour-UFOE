#include "single_keyboard.h"
#include "../ufo_engine/ufo_engine.h"
#include "../ufo_input/ufo_input.h"
#include "../external/olcPixelGameEngine.h"

ButtonState SingleKeyboard::GetKey(olc::Key _button){
    return To_ufoButtonState(Engine::Get().pixel_game_engine.GetKey(_button));
}