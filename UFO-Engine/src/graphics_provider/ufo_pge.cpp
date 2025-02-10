#include "ufo_pge.h"
#include "../ufo_engine/ufo_engine_main.h"

UFO_PixelGameEngine::UFO_PixelGameEngine()
{
    sAppName = "UFO Engine";
}

bool UFO_PixelGameEngine::OnUserCreate(){
    ufo_engine_main->scene_system.Start();
    return ufo_engine_main->OnMainReady();
}

bool UFO_PixelGameEngine::OnUserUpdate(float fElapsedTime){
    return ufo_engine_main->OnMainUpdate(fElapsedTime);
}

bool UFO_PixelGameEngine::OnUserCommand(std::string& _text){
    return ufo_engine_main->OnUserCommand(_text);
}