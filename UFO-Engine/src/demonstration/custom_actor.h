#pragma once

#include "../level/actor.h"
#include "../level/level.h"
#include "../console/console.h"
#include "../ufo_maths/ufo_maths.h"
#include "../mouse/mouse.h"
#include "../sprite_reference/sprite_reference.h"
#include "../level/ufo_engine.h"

class CustomActor : public Actor{
public:

    Level* level_handle = nullptr;

    CustomActor(Vector2f _local_position) : Actor(_local_position){

    }

    void OnLevelEnter(Level* _level){
        level_handle = _level;
    
        AddChild(std::make_unique<SpriteReference>(
            "shadow",
            Vector2f(0.0f, 0.0f),
            Vector2f(8.0f, 8.0f),
            Vector2f(16.0f, 16.0f),
            Vector2f(1.0f, 1.0f),
            0,
            0
        ));
    }

    void OnUpdate(){
        Vector2f mouse_position_world = level_handle->GetActiveCamera()->TransformScreenToWorld(Mouse::Get().GetPosition());

        Vector2f direction = mouse_position_world-local_position;

        local_position += direction.norm() * Engine::Get().GetDeltaTime()*100.0f;
    }

};