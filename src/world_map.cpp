#include <actor.h>
#include <ufo_maths.h>
#include <level.h>
#include "pingus_level.h"
#include <sprite_reference.h>
#include <ufo_engine.h>
#include <json_variant.h>
#include <console.h>
#include <widget_sprite_reference.h>
#include "world_map.h"
#include "world_map_location.h"

void WorldMap::OnLevelEnter(Level* _level){
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
        Vector2f(1.0f,1.0f),0.0f,0);

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

    bool level_was_selected = false;
    
    //WorldMapLocation* selected_level = nullptr;

    for(const auto& location : level->world_map_location_handles){
        if(location->selected && location->unlocked){
            level_was_selected = true;
            //preview_image->key = location->preview;
        }
    }

    backdrop_velocity += backdrop_acceleration * Engine::Get().GetDeltaTime();

    if(level_was_selected){
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