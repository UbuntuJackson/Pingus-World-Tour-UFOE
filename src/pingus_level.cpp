#include <string>
#include <vector>
#include <memory>
#include <olcPixelGameEngine.h>
#include <sprite_reference.h>
#include <level.h>
#include <json.h>
#include <ufo_maths.h>
#include <asset_manager.h>
#include <bitmap_collision_body.h>
#include "level_select_menu.h"
#include <widget.h>
#include <file_menu.h>
#include <single_keyboard.h>
//#include <sub_window.h>
#include "pingus_level.h"
#include "pingu.h"
#include <mouse.h>
#include "level_select_menu.h"
#include "pingus_main_menu.h"
#include "generated_actor_json_bridge.h"

void PingusLevel::OnResourceLoad(){
    AssetManager::Get().LoadDecal("../res/assets/pingu_hitbox.png","pill");
    AssetManager::Get().LoadDecal("../res/assets/box_12_24.png","box_12_24");
    AssetManager::Get().LoadDecal("../res/assets/walking animation-Sheet.png", "pingu_walk");
    AssetManager::Get().LoadDecal("../res/assets/pingu_fall.png", "pingu_fall");
    AssetManager::Get().LoadDecal("../res/assets/pingu_blowing_up-Sheet.png", "pingu_explode");
    Engine::Get().pixel_game_engine.LoadFontSprite("../res/assets/pwt_font.png");
}

void PingusLevel::OnLoadActors(Json& _actor_json){
    Console::Out("PingusLevel OnLoadACtors");
    GeneratedActorJsonBridge(this, _actor_json, "pingus_actor_sheet");
}

void PingusLevel::OnLoad(Json& _j){
    custom_level_size = AssetManager::Get().GetDecal("solid.png")->sprite->Size();
    level_size = custom_level_size;
    Console::Out("Level Custom size =",level_size);
    GetActiveCamera()->world.x1 = custom_level_size.x;
    GetActiveCamera()->world.y1 = custom_level_size.y;
    GetActiveCamera()->clamp = true;
    Console::Out("Level size", level_size);
    
}

void PingusLevel::OnUpdate(){
    
    Level::OnUpdate();

    if(SingleKeyboard::Get().GetKey(olc::ESCAPE).is_pressed){

        NewActor<PingusMainMenu>(Vector2f(100.0f,100.0f));
    }

    if(Mouse::Get().GetLeftButton().is_pressed){

        auto a = NewActor<Pingu>(GetActiveCamera()->TransformScreenToWorld(Mouse::Get().GetPosition()));

        Console::Out("Pingu count",pingu_count++);
    
    }
}