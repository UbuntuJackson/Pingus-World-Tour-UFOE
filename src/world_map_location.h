#pragma once
#include <actor.h>
#include <ufo_maths.h>
#include <level.h>
#include "pingus_level.h"
#include <sprite_reference.h>

class WorldMapLocation : public Actor{
public:

    PingusLevel* level = nullptr;
    WorldMapLocation(Vector2f _) : Actor(Vector2f(0.0f,0.0f)){}

    void OnLevelEnter(Level* _level){
        level = dynamic_cast<PingusLevel*>(_level); 
        level->is_menu = true;

        AddChild<SpriteReference>(
            "world_map_pin.png",
            Vector2f(0.0f,0.0f),
            Vector2f(0.0f,0.0f),
            Vector2f(680.0f,480.0f),
            Vector2f(1.0f,1.0f),
            0.0f
        );
        
    }

};