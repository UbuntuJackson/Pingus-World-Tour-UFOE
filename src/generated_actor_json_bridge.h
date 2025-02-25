#include <json_variant.h>
#include <ufo_maths.h>
#include <console.h>
#include "../src/honey_coin.h"
#include "../src/pingu.h"
#include "../src/spawner.h"
#include "../src/goal.h"
#include "../src/pingus_main_menu.h"
#include "../src/item_select_menu.h"
void GeneratedActorJsonBridge(Level* _level, JsonDictionary& _actor_json, std::string _actor_sheet){
    auto actors_tileset_data = _level->tilemap.GetTilesetData(_actor_sheet);
    int type_id = _actor_json.Get("gid").AsInt() - actors_tileset_data.tileset_start_id+1;
    switch(type_id){
        case 1:{
            auto instance = _level->NewActor<HoneyCoin>(Vector2f(_actor_json.Get("x").AsFloat(), _actor_json.Get("y").AsFloat()));
            if(_actor_json.KeyExists("properties")){
                JsonArray& property_json = _actor_json.Get("properties").AsArray();
                for(auto&& _json : property_json.Iterable()){
                   auto property_dict = _json->AsDictionary();
                }
            }
        }
        break;
        case 2:{
            auto instance = _level->NewActor<Pingu>(Vector2f(_actor_json.Get("x").AsFloat(), _actor_json.Get("y").AsFloat()));
            if(_actor_json.KeyExists("properties")){
                JsonArray& property_json = _actor_json.Get("properties").AsArray();
                for(auto&& _json : property_json.Iterable()){
                   auto property_dict = _json->AsDictionary();
                }
            }
        }
        break;
        case 4:{
            auto instance = _level->NewActor<Spawner>(Vector2f(_actor_json.Get("x").AsFloat(), _actor_json.Get("y").AsFloat()));
            if(_actor_json.KeyExists("properties")){
                JsonArray& property_json = _actor_json.Get("properties").AsArray();
                for(auto&& _json : property_json.Iterable()){
                   auto property_dict = _json->AsDictionary();
                    if(property_dict.Get("name").AsString() == "number_of_pingus"){
                        instance->number_of_pingus = property_dict.Get("value").AsInt();
                    }
                    if(property_dict.Get("name").AsString() == "interval_milliseconds"){
                        instance->interval_milliseconds = property_dict.Get("value").AsFloat();
                    }
                }
            }
        }
        break;
        case 5:{
            auto instance = _level->NewActor<Goal>(Vector2f(_actor_json.Get("x").AsFloat(), _actor_json.Get("y").AsFloat()));
            instance->shape.size.x = _actor_json.Get("width").AsInt();
            instance->shape.size.y = _actor_json.Get("height").AsInt();
            if(_actor_json.KeyExists("properties")){
                JsonArray& property_json = _actor_json.Get("properties").AsArray();
                for(auto&& _json : property_json.Iterable()){
                   auto property_dict = _json->AsDictionary();
                    if(property_dict.Get("name").AsString() == "number_of_pingus"){
                        instance->number_of_pingus = property_dict.Get("value").AsInt();
                    }
                    if(property_dict.Get("name").AsString() == "maximum_rescuable_pingus"){
                        instance->maximum_rescuable_pingus = property_dict.Get("value").AsInt();
                    }
                }
            }
        }
        break;
        case 6:{
            auto instance = _level->NewActor<PingusMainMenu>(Vector2f(_actor_json.Get("x").AsFloat(), _actor_json.Get("y").AsFloat()));
            if(_actor_json.KeyExists("properties")){
                JsonArray& property_json = _actor_json.Get("properties").AsArray();
                for(auto&& _json : property_json.Iterable()){
                   auto property_dict = _json->AsDictionary();
                }
            }
        }
        break;
        case 7:{
            auto instance = _level->NewActor<ItemSelectMenu>(Vector2f(_actor_json.Get("x").AsFloat(), _actor_json.Get("y").AsFloat()));
            if(_actor_json.KeyExists("properties")){
                JsonArray& property_json = _actor_json.Get("properties").AsArray();
                for(auto&& _json : property_json.Iterable()){
                   auto property_dict = _json->AsDictionary();
                    if(property_dict.Get("name").AsString() == "number_of_bombers"){
                        instance->number_of_bombers = property_dict.Get("value").AsInt();
                    }
                    if(property_dict.Get("name").AsString() == "number_of_builders"){
                        instance->number_of_builders = property_dict.Get("value").AsInt();
                    }
                    if(property_dict.Get("name").AsString() == "number_of_parachutes"){
                        instance->number_of_parachutes = property_dict.Get("value").AsInt();
                    }
                    if(property_dict.Get("name").AsString() == "number_of_blockers"){
                        instance->number_of_blockers = property_dict.Get("value").AsInt();
                    }
                    if(property_dict.Get("name").AsString() == "number_of_climbers"){
                        instance->number_of_climbers = property_dict.Get("value").AsInt();
                    }
                }
            }
        }
        break;
    }
}
