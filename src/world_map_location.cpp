#include <actor.h>
#include <ufo_maths.h>
#include <level.h>
#include <sprite_reference.h>
#include <graphics.h>
#include <camera.h>
#include "world_map_location.h"
#include "pingus_level.h"

void WorldMapLocation::OnLevelEnter(Level* _level){
    AddChild<SpriteReference>(
        "red_x",
        Vector2f(0.0f,-32.0f),
        Vector2f(0.0f,0.0f),
        Vector2f(32.0f,32.0f),
        Vector2f(1.0f,1.0f),
        0.0f
    );
    dynamic_cast<PingusLevel*>(_level)->world_map_location_handles.push_back(this);
}

void WorldMapLocation::OnDraw(Camera* _camera){
    if(selected) Graphics::Get().DrawString(_camera->Transform(GetGlobalPosition()), name_of_location, Graphics::WHITE ,{1.0f,1.0f});
}