#include <string>
#include <vector>
#include <memory>
#include "../external/olcPixelGameEngine.h"
#include "../sprite_reference/sprite_reference.h"
#include "../level/level.h"
#include "../json/json.h"
#include "../ufo_maths/ufo_maths.h"
#include "../asset_manager/asset_manager.h"
#include "../bitmap_level/bitmap_collision_body.h"
#include "level_select_menu.h"
#include "../widgets/widget.h"
#include "../file/file_menu.h"
#include "../keyboard/single_keyboard.h"
#include "../widgets/sub_window.h"
#include "pingus_level.h"
#include "pingu.h"
#include "../mouse/mouse.h"
#include "level_select_menu.h"
#include "pingus_main_menu.h"

void PingusLevel::OnResourceLoad(){
    AssetManager::Get().LoadDecal("../res/pingus_world_tour/assets/pingu_hitbox.png","pill");
    AssetManager::Get().LoadDecal("../res/pingus_world_tour/assets/box_12_24.png","box_12_24");
    AssetManager::Get().LoadDecal("../res/pingus_world_tour/assets/walking animation-Sheet.png", "pingu_walk");
    AssetManager::Get().LoadDecal("../res/pingus_world_tour/assets/pingu_blowing_up-Sheet.png", "pingu_explode");
    Engine::Get().pixel_game_engine.LoadFontSprite("../res/pingus_world_tour/assets/pwt_font.png");
}

void PingusLevel::OnLoadJsonCustom(Json& level_json){
    
    Json j_layers = level_json.GetObject("layers");

    Json::ArrayForEach(Json(), j_layers, [this](Json& _j, const Json& _layers){
        if(_j.GetAsString("name") != "actors" && _j.GetAsString("name") != "goals" && _j.GetAsString("name") != "actor_layer"){

            AssetManager::Get().LoadDecal(_j.GetAsString("path"),_j.GetAsString("name"));

            level_decals[_j.GetAsString("name")] = AssetManager::Get().GetDecal(_j.GetAsString("name"));

            if(_j.GetAsString("name") == "solid") solid_decal = AssetManager::Get().GetDecal(_j.GetAsString("name"));

            auto decal = AssetManager::Get().GetDecal(_j.GetAsString("name"));

            auto u_sprite = std::make_unique<SpriteReference>(
                _j.GetAsString("name"),
                Vector2f(0.0f, 0.0f),
                Vector2f(0.0f, 0.0f),
                decal->sprite->Size(),
                Vector2f(1.0f, 1.0f),
                0.0f,
                0
            );

            if(_j.GetAsString("name") == "solid"){
                u_sprite->visible = false;
            }

            NewActor(std::move(u_sprite));

            custom_level_size = decal->sprite->Size();
            Console::Out("Decal size",decal->sprite->Size());
        }
        if(_j.GetAsString("name") == "actor_layer"){
            auto actor_array = _j.GetObject("actors");
            Json::ArrayForEach(Json(), actor_array, [this](Json& _j, const Json& _layers){
                if(_j.GetAsString("name") == "MainMenu"){
                    NewActor<PingusMainMenu>();
                }
            });
        }
    });

    //NewActor(std::make_unique<Pingu>(Vector2f(200.0f, 20.0f), this));

}

void PingusLevel::OnLoad(Json& _j){
    level_size = custom_level_size;
    GetActiveCamera()->world.x1 = custom_level_size.x;
    GetActiveCamera()->world.y1 = custom_level_size.y;
    GetActiveCamera()->clamp = true;
    Console::Out("Level size", level_size);

    tilemap_exists = false;
    
}

void PingusLevel::OnUpdate(){
    
    Level::OnUpdate();

    if(SingleKeyboard::Get().GetKey(olc::ESCAPE).is_pressed){
        /*auto u_sub_window = std::make_unique<SubWindow>(Vector2f(20.0f, 20.0f),Vector2f(400.0f, 400.0f));

        auto u_level_select_menu = std::make_unique<FileMenu>(Vector2f(20.0f, 20.0f),Vector2f(250.0f, 20.0f),"../res");

        u_level_select_menu->func_on_file_open = [](Widget* _w, FileMenuButton* _button, std::string _file_extension){
            if(_file_extension == "json"){
                auto pl = std::make_unique<PingusLevel>();
                pl->pingus_level_path = _button->path;
                Engine::Get().GoToLevel(std::move(pl),"../res/engine/default_level.json");
            }
            else{
                Console::Out("File is not .json");
            }

            if(_file_extension == "cpp"){
                Console::Out(File().Read(_button->path));

                _w->level->NewActor(std::make_unique<Label>(Vector2f(400.0f, 0.0f), Vector2f(400.0f, 400.0f), File().Read(_button->path)));
            }
        };

        //u_sub_window->AddChild((std::move(u_level_select_menu)));
        NewActor(std::move(u_level_select_menu));*/

        NewActor<PingusMainMenu>();
    }

    if(Mouse::Get().GetLeftButton().is_pressed){
        auto a = NewActor<Pingu>(GetActiveCamera()->TransformScreenToWorld(Mouse::Get().GetPosition()), this);

        Console::Out("Pingu count",pingu_count++);
    
    }
}