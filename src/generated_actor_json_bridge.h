#include <json.h>
#include <ufo_maths.h>
#include <console.h>
#include "../src/honey_coin.h"
#include "../src/pingu.h"
#include "../src/spawner.h"
#include "../src/goal.h"
#include "../src/pingus_main_menu.h"
#include "../src/item_select_menu.h"
void GeneratedActorJsonBridge(Level* _level, Json& _actor_json, std::string _actor_sheet){
    auto actors_tileset_data = _level->tilemap.GetTilesetData(_actor_sheet);
    int type_id = _actor_json.GetAsInt("gid") - actors_tileset_data.tileset_start_id+1;
    switch(type_id){
        case 1:{
            auto instance = _level->NewActor<HoneyCoin>(Vector2f((float)(_actor_json.GetAsInt("x")), (float)(_actor_json.GetAsInt("y"))));
            Json property_json = _actor_json.GetObject("properties");
            if(!property_json.IsNull()){
                Json::ArrayForEach(Json(), property_json, [&](Json _json, Json _data){
                });
            }
        }
        break;
        case 2:{
            auto instance = _level->NewActor<Pingu>(Vector2f((float)(_actor_json.GetAsInt("x")), (float)(_actor_json.GetAsInt("y"))));
            Json property_json = _actor_json.GetObject("properties");
            if(!property_json.IsNull()){
                Json::ArrayForEach(Json(), property_json, [&](Json _json, Json _data){
                });
            }
        }
        break;
        case 4:{
            auto instance = _level->NewActor<Spawner>(Vector2f((float)(_actor_json.GetAsInt("x")), (float)(_actor_json.GetAsInt("y"))));
            Json property_json = _actor_json.GetObject("properties");
            if(!property_json.IsNull()){
                Json::ArrayForEach(Json(), property_json, [&](Json _json, Json _data){
                    if(_json.GetAsString("name") == "number_of_pingus"){
                        instance->number_of_pingus = _json.GetAsInt("value");
                    }
                });
            }
        }
        break;
        case 5:{
            auto instance = _level->NewActor<Goal>(Vector2f((float)(_actor_json.GetAsInt("x")), (float)(_actor_json.GetAsInt("y"))));
            instance->shape.size.x = _actor_json.GetAsInt("width");
            instance->shape.size.y = _actor_json.GetAsInt("height");
            Json property_json = _actor_json.GetObject("properties");
            if(!property_json.IsNull()){
                Json::ArrayForEach(Json(), property_json, [&](Json _json, Json _data){
                    if(_json.GetAsString("name") == "number_of_pingus"){
                        instance->number_of_pingus = _json.GetAsInt("value");
                    }
                });
            }
        }
        break;
        case 6:{
            auto instance = _level->NewActor<PingusMainMenu>(Vector2f((float)(_actor_json.GetAsInt("x")), (float)(_actor_json.GetAsInt("y"))));
            Json property_json = _actor_json.GetObject("properties");
            if(!property_json.IsNull()){
                Json::ArrayForEach(Json(), property_json, [&](Json _json, Json _data){
                });
            }
        }
        break;
        case 7:{
            auto instance = _level->NewActor<ItemSelectMenu>(Vector2f((float)(_actor_json.GetAsInt("x")), (float)(_actor_json.GetAsInt("y"))));
            Json property_json = _actor_json.GetObject("properties");
            if(!property_json.IsNull()){
                Json::ArrayForEach(Json(), property_json, [&](Json _json, Json _data){
                    if(_json.GetAsString("name") == "number_of_builders"){
                        instance->number_of_builders = _json.GetAsInt("value");
                    }
                    if(_json.GetAsString("name") == "number_of_bombers"){
                        instance->number_of_bombers = _json.GetAsInt("value");
                    }
                });
            }
        }
        break;
    }
}
