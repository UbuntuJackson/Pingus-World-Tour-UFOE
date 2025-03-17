#include <actor.h>
#include <ufo_maths.h>
#include <level.h>
#include "pingus_level.h"
#include <sprite_reference.h>
#include <ufo_engine.h>
#include <json_variant.h>
#include <console.h>
#include "world_map.h"
#include "world_map_location.h"

void WorldMap::OnLevelEnter(Level* _level){
    level = dynamic_cast<PingusLevel*>(_level); 
    level->is_menu = true;
    
    JsonVariant& last_played_level_json = Engine::Get().GetActiveProfile()->save_file.Get("last_played_level");
    if(!last_played_level_json.IsNull()) last_played_level = last_played_level_json.AsString();
    Console::PrintLine("Last played level:",last_played_level);

}

void WorldMap::OnStart(Level* _level){
    Console::PrintLine("WorldMap::OnStart");

    std::string level_to_unlock = "";
    for(const auto& location : level->world_map_location_handles){
        if(location->level_path == last_played_level){
            JsonDictionary& unlocked_levels = Engine::Get().GetActiveProfile()->save_file.Get("unlocked_levels").AsDictionary();
            unlocked_levels.Set(location->unlocks_other_level, true);
            level_to_unlock = location->unlocks_other_level;
            Console::PrintLine("Should unlock level:", level_to_unlock);
        }
    }
    for(const auto& location : level->world_map_location_handles){
        if(location->level_path == last_played_level){
            location->Unlock();
        }
    }
}