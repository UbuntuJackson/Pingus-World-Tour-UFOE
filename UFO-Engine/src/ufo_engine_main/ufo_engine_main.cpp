#include "ufo_engine_main.h"
#include "../graphics_provider/graphics_provider.h"
#include "../ufo_clock/ufo_time.h"
#include "../keyboard/single_keyboard.h"
#include "../ufo_lang/ufo_lang.h"
UFO_EngineMain::UFO_EngineMain(int _w, int _h, int _pw, int _ph, bool _v_sync){
    Start(_w, _h, _pw, _ph, _v_sync);
}

bool
UFO_EngineMain::Start(int _w, int _h, int _pw, int _ph, bool _v_sync){
    if(GraphicsProvider::Get().Construct(_w,_h,_pw,_ph,false,_v_sync,true)){
        GraphicsProvider::Get().ufo_engine_main = this;
        GraphicsProvider::Get().Start();
        return false;
    }
    return true;
}

bool
UFO_EngineMain::OnMainReady(){
    return true;
}

bool
UFO_EngineMain::OnMainUpdate(float _delta_time){
    ufoTime::Get().delta_time = _delta_time;
    GraphicsProvider::Get().Clear(olc::GREY);
    GraphicsProvider::Get().SetPixelMode(olc::Pixel::NORMAL);
    scene_system.Update();
    if(SingleKeyboard::GetKey(olc::F1).is_pressed) GraphicsProvider::Get().ConsoleShow(olc::F1);
    return !scene_system.quit;
}

bool
UFO_EngineMain::OnUserCommand(std::string& _text){
    ufoLang::Execute(scene_system.GetActiveScene(), _text);

    return true;
}