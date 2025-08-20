#include <actor.h>
#include <ufo_maths.h>
#include <level.h>
#include "pingus_level.h"
#include <sprite_reference.h>
#include <ufo_engine.h>
#include <json_variant.h>
#include <console.h>
#include <widget_sprite_reference.h>
#include <button.h>
#include <nine_patch_theme.h>
#include <graphics.h>
#include <mouse.h>
#include "world_map.h"
#include "world_map_location.h"

void WorldMap::OnLevelEnter(Level* _level){
    Widget::OnLevelEnter(_level);
    level = dynamic_cast<PingusLevel*>(_level); 
    level->is_menu = true;

    JsonVariant& last_played_level_json = Engine::Get().GetActiveProfile()->save_file.Get("last_played_level");
    if(!last_played_level_json.IsNull()){
        last_played_level = last_played_level_json.AsString();
        
        JsonVariant& last_played_level_clear_data = Engine::Get().GetActiveProfile()->save_file.Get("cleared_levels").AsDictionary().Get(last_played_level);

        if(!last_played_level_clear_data.IsNull()) last_played_level_rank = last_played_level_clear_data.AsDictionary().Get("rank").AsInt();
    }
    Console::PrintLine("Last played level:",last_played_level);

    backdrop = level->NewActor<WidgetSpriteReference>("backdrop",
        Vector2f(340.0f,0.0f),
        Vector2f(0.0f,0.0f),
        Vector2f(680.0f,480.0f),
        Vector2f(1.0f,1.0f),0.0f);
    
    back_to_main_menu = AddChild<Button>(Vector2f(0.0f+16.0f,480.0f-64.0f), Vector2f(56.0f,56.0f-8.0f), "Back to main menu");
    back_to_main_menu->theme = std::make_unique<NinePatchTheme>("pwt_widget_theme_grey", 3,4,3,4);
    back_to_main_menu->hovered_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_light", 3,4,3,4);
    back_to_main_menu->held_theme = std::make_unique<NinePatchTheme>("pwt_theme_grey_dark", 3,4,3,4);

}

void WorldMap::OnStart(Level* _level){
    Console::PrintLine("WorldMap::OnStart");
    if(last_played_level_rank < 3){
        for(const auto& location : level->world_map_location_handles){
            if(location->level_path == last_played_level){
                JsonDictionary& unlocked_levels = Engine::Get().GetActiveProfile()->save_file.Get("unlocked_levels").AsDictionary();
                unlocked_levels.Set(location->unlocks_other_level, true);
                location->Unlock();
            }
        }
    }
}

void WorldMap::OnUpdate(){

    bool level_is_selected = false;
    float screen_width_half = 340.0f;

    if(back_to_main_menu->IsReleased()){
        Engine::Get().GoToLevel(std::make_unique<PingusLevel>(), "../res/map/title_screen/title_screen.json");
    }
    
    //WorldMapLocation* selected_level = nullptr;

    for(const auto& location : level->world_map_location_handles){
        if(location->selected && location->unlocked){
            
            level_is_selected = true;
            if(!level_was_selected){
                selected_to_left = Mouse::Get().GetPosition().x < screen_width_half;
            }

            //preview_image->key = location->preview;
        }
    }

    if(selected_to_left){
        backdrop_velocity += backdrop_acceleration * Engine::Get().GetDeltaTime();
        backdrop->scale.x = 1.0f;

        if(level_is_selected){
            if(backdrop->local_position.x > (0.0f+backdrop_velocity * Engine::Get().GetDeltaTime())) backdrop->local_position.x -= backdrop_velocity * Engine::Get().GetDeltaTime();
            else{
                backdrop->local_position.x = 0.0f;
                backdrop_velocity = 0.0f;
            }
        }
        else{
            if(backdrop->local_position.x < 340.0f-backdrop_velocity * Engine::Get().GetDeltaTime()) backdrop->local_position.x += backdrop_velocity * Engine::Get().GetDeltaTime();
            else{
                backdrop->local_position.x = 340.0f;
                backdrop_velocity = 0.0f;
            }
        }
    }
    else{

        
        backdrop->scale.x = -1.0f;
        backdrop_velocity += backdrop_acceleration * Engine::Get().GetDeltaTime();

        if(level_is_selected){
            if(backdrop->local_position.x < (340.0f+screen_width_half-backdrop_velocity * Engine::Get().GetDeltaTime())) backdrop->local_position.x += backdrop_velocity * Engine::Get().GetDeltaTime();
            else{
                backdrop->local_position.x = 340.0f+screen_width_half;
                backdrop_velocity = 0.0f+screen_width_half;
            }
        }
        else{
            if(backdrop->local_position.x > 0.0f+screen_width_half+backdrop_velocity * Engine::Get().GetDeltaTime()) backdrop->local_position.x -= backdrop_velocity * Engine::Get().GetDeltaTime();
            else{
                backdrop->local_position.x = 0.0f+screen_width_half;
                backdrop_velocity = 0.0f+screen_width_half;
            }
        }
    }

    level_was_selected = level_is_selected;
}

void WorldMap::OnWidgetDraw(){
    
}