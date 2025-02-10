#include <string>
#include "../external/olcPixelGameEngine.h"
#include "spawn_point.h"
#include "../json/json.h"
#include "../ufo_engine/ufo_engine.h"
#include "level_trigger.h"

SpawnPoint::SpawnPoint(olc::vf2d _position, std::string _name) :
name{_name},
Actor(_position){
    Engine::Get().current_level->spawn_point_handles.push_back(this);
}

std::unique_ptr<SpawnPoint> SpawnPoint::Load(Json& _json){
    Console::Out("spawnpoint is loading");
    float x = (float)_json.GetAsInt("x");
    float y = (float)_json.GetAsInt("y");
    std::string name = _json.GetAsString("name");
    /*std::string l_from_path;
    Json properties = _json.GetObject("properties");
    Json::ArrayForEach(Json(), properties, [&](Json _property, Json _properties){
        
        if(_property.GetAsString("name") == "level_enterance"){
            int tiled_object_id = _property.GetAsInt("value");
            auto& trigger_handles = Engine::Get().current_level->level_trigger_handles;
            for(const auto& trigger : trigger_handles){
                if(trigger->tiled_object_id == tiled_object_id){
                    l_from_path = trigger->path;
                }
            }
        }
    });*/
    return std::make_unique<SpawnPoint>(olc::vf2d(x,y), name);
}