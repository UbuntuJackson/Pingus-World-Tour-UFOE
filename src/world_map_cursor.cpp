#include <actor.h>
#include <cmath>
#include <ufo_maths.h>
#include <ufo_engine.h>
#include <level.h>
#include "pingus_level.h"
#include <widget_sprite_reference.h>
#include <mouse.h>
#include "world_map_cursor.h"
#include "world_map_location.h"

void WorldMapCursor::OnLevelEnter(Level* _level){
    level = dynamic_cast<PingusLevel*>(_level); 
    level->is_menu = true;

    spr = AddChild<WidgetSpriteReference>(
        "pin",
        Vector2f(0.0f,0.0f),
        Vector2f(32.0f,32.0f),
        Vector2f(32.0f,32.0f),
        Vector2f(1.0f,1.0f),
        0.0f,0
    );

    SetZIndex(3);
    
}

void WorldMapCursor::OnUpdate(){
    local_position = Mouse::Get().GetPosition();
    if(local_position.x > 340.0f) spr->scale.x = -1.0f;
    else spr->scale.x = 1.0f;

    spr->local_position = Vector2f(spr->scale.x,1.0f) * 3.0f * std::sin(Engine::Get().GetTime()*1.5f);

    for(const auto& location : level->world_map_location_handles){
        if(ufoMaths::Distance2(level->GetActiveCamera()->TransformScreenToWorld(GetGlobalPosition()),location->GetGlobalPosition()) < 32.0f){
            location->selected = true;
            if(location->level_path != "" && Mouse::Get().GetLeftButton().is_pressed && location->unlocked){
                //Would be nice to have a way to error handle should the level path be faulty, instead of downright crashing.
                Engine::Get().GoToLevel(std::make_unique<PingusLevel>(), location->level_path);
            }
        }
        else{
            location->selected = false;
        }
    }
}