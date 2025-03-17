#include <actor.h>
#include <ufo_maths.h>
#include <level.h>
#include <sprite_reference.h>
#include <graphics.h>
#include <camera.h>
#include <json_variant.h>
#include <ufo_engine.h>
#include "world_map_location.h"
#include "pingus_level.h"

void WorldMapLocation::OnLevelEnter(Level* _level){
    spr = AddChild<SpriteReference>(
        "red_x",
        Vector2f(0.0f,-32.0f),
        Vector2f(0.0f,0.0f),
        Vector2f(32.0f,32.0f),
        Vector2f(1.0f,1.0f),
        0.0f
    );
    level = dynamic_cast<PingusLevel*>(_level);
    level->world_map_location_handles.push_back(this);
}

void WorldMapLocation::OnStart(Level* _level){
    JsonDictionary& unlocked_levels = Engine::Get().GetActiveProfile()->save_file.Get("unlocked_levels").AsDictionary();
    if(!unlocked_levels.Get(level_path).IsNull()) unlocked = true;
    
    Console::PrintLine("Location",_level->path,unlocked);

    spr->visible = unlocked;
}

void WorldMapLocation::OnDraw(Camera* _camera){
    if(selected && unlocked) Graphics::Get().DrawString(_camera->Transform(GetGlobalPosition()), name_of_location, Graphics::WHITE ,{1.0f,1.0f});
}

void WorldMapLocation::Unlock(){
    spr->visible = true;
    unlocked = true;
}