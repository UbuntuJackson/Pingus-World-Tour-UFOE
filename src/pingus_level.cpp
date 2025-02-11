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
#include <widget_sprite_reference.h>
//#include <sub_window.h>
#include "pingus_level.h"
#include "pingu.h"
#include <mouse.h>
#include <wrap_menu.h>
#include "level_select_menu.h"
#include "pingus_main_menu.h"
#include "generated_actor_json_bridge.h"

void PingusLevel::OnResourceLoad(){
    AssetManager::Get().LoadDecal("../res/assets/pingu_hitbox.png","pill");
    AssetManager::Get().LoadDecal("../res/assets/box_12_24.png","box_12_24");
    AssetManager::Get().LoadDecal("../res/assets/walking animation-Sheet.png", "pingu_walk");
    AssetManager::Get().LoadDecal("../res/assets/spawner.png", "spawner");
    AssetManager::Get().LoadDecal("../res/assets/pingu_fall.png", "pingu_fall");
    AssetManager::Get().LoadDecal("../res/assets/honey_coint-Sheet.png", "honey_coin");
    AssetManager::Get().LoadDecal("../res/assets/pingu_blowing_up-Sheet.png", "pingu_explode");
    AssetManager::Get().LoadDecal("../res/assets/honey_coin_hud-Sheet.png", "honey_coin_hud");
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

    rescued_pingus_label = NewActor<Label>(Vector2f(180.0f, 3.0f), Vector2f(100.0f, 20.0f), "Rescued Pingus");
    released_pingus_label = NewActor<Label>(Vector2f(160.0f, 20.0f), Vector2f(100.0f, 20.0f), "Released Pingus");
    honey_coin_hud = NewActor<WidgetSpriteReference>("honey_coin_hud",
        Vector2f(Engine::Get().pixel_game_engine.GetWindowSizeInPixles().x-33.0f, 40.0f),
        Vector2f(0.0f, 0.0f),
        Vector2f(32.0f, 32.0f),
        Vector2f(1.0f, 1.0f),
        0.0f,
        0
    );
    honey_coin_hud->current_frame_index = 1.0f;
    
}

void PingusLevel::OnUpdate(){
    
    Level::OnUpdate();

    rescued_pingus_label->text = "Rescued Pingus:" + std::to_string(rescued_pingus) + "/" + std::to_string(required_pingus);
    released_pingus_label->text = "Released Pingus:" + std::to_string(released_pingus) + "/" + std::to_string(total_number_of_pingus);

    if(SingleKeyboard::Get().GetKey(olc::ESCAPE).is_pressed){

        NewActor<PingusMainMenu>(Vector2f(100.0f,100.0f));
    }

    if(Mouse::Get().GetLeftButton().is_pressed){

        auto a = NewActor<Pingu>(GetActiveCamera()->TransformScreenToWorld(Mouse::Get().GetPosition()));

        //Console::Out("Pingu count",pingu_count++);
    
    }
}

void PingusLevel::OnHandlePurge(){
    PurgeHandles(honey_coin_handles);
}