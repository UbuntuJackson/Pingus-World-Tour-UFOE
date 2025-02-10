#include <string>
#include "pixel_game_engine.h"
#include "../ufo_engine/ufo_engine.h"

PixelGameEngine::PixelGameEngine()
{
    sAppName = "UFO Engine";
}

olc::vi2d PixelGameEngine::GetWindowSizeInPixles(){
    return GetWindowSize()/GetPixelSize();
}

bool PixelGameEngine::OnUserCreate(){
    return engine->OnReady();
}

bool PixelGameEngine::OnUserUpdate(float fElapsedTime){
    return engine->OnUpdate(fElapsedTime);
}

bool PixelGameEngine::OnUserCommand(std::string& _text){
    return engine->OnUserCommand(_text);
}