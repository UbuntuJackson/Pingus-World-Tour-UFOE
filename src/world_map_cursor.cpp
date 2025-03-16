#include <actor.h>
#include <ufo_maths.h>
#include <level.h>
#include "pingus_level.h"
#include <sprite_reference.h>
#include <mouse.h>
#include "world_map_cursor.h"
#include "world_map_location.h"

void WorldMapCursor::OnLevelEnter(Level* _level){
    level = dynamic_cast<PingusLevel*>(_level); 
    level->is_menu = true;

    spr = AddChild<SpriteReference>(
        "pin",
        Vector2f(0.0f,0.0f),
        Vector2f(32.0f,32.0f),
        Vector2f(32.0f,32.0f),
        Vector2f(1.0f,1.0f),
        0.0f
    );

    SetZIndex(3);
    
}

void WorldMapCursor::OnUpdate(){
    local_position = level->GetActiveCamera()->TransformScreenToWorld(Mouse::Get().GetPosition());
    if(local_position.x > 340.0f) spr->scale.x = -1.0f;
    else spr->scale.x = 1.0f;

    for(const auto& location : level->world_map_location_handles){
        if(ufoMaths::Distance2(GetGlobalPosition(),location->GetGlobalPosition()) < 32.0f){
            location->selected = true;
            if(location->level_path != "" && Mouse::Get().GetLeftButton().is_pressed){
                //Would be nice to have a way to error handle should the level path be faulty, instead of downright crashing.
                Engine::Get().GoToLevel(std::make_unique<PingusLevel>(), location->level_path);
            }
        }
        else{
            location->selected = false;
        }
    }
}