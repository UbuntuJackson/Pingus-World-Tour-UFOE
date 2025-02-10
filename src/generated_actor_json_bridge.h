#include <json.h>
#include <ufo_maths.h>
#include <console.h>
#include "../src/pingu.h"
#include "../src/goal.h"
#include "../src/pingus_main_menu.h"
void GeneratedActorJsonBridge(Level* _level, Json& _actor_json, std::string _actor_sheet){
    auto actors_tileset_data = _level->tilemap.GetTilesetData(_actor_sheet);
    int type_id = _actor_json.GetAsInt("gid") - actors_tileset_data.tileset_start_id+1;
    Console::Out(type_id);    switch(type_id){
        case 2:{
            auto instance = _level->NewActor<Pingu>(Vector2f((float)(_actor_json.GetAsInt("x")), (float)(_actor_json.GetAsInt("y"))));
            Console::Out("Pingu");
        }
        break;
        case 5:{
            auto instance = _level->NewActor<Goal>(Vector2f((float)(_actor_json.GetAsInt("x")), (float)(_actor_json.GetAsInt("y"))));
            instance->shape.size.x = _actor_json.GetAsInt("width");
            instance->shape.size.y = _actor_json.GetAsInt("height");
            Console::Out("Goal");
        }
        break;
        case 6:{
            auto instance = _level->NewActor<PingusMainMenu>(Vector2f((float)(_actor_json.GetAsInt("x")), (float)(_actor_json.GetAsInt("y"))));
            Console::Out("PingusMainMenu");
        }
        break;
    }
}
