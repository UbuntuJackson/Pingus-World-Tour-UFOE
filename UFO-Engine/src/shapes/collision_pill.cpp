#include "collision_pill.h"
#include "pill.h"
#include "raw_shape_base.h"
#include "../external/olcPixelGameEngine.h"
#include "../drawing_system/drawing_system.h"
#include "../ufo_maths/ufo_maths.h"
#include "../collision/collision_body.h"
#include "../ufo_engine/ufo_engine.h"

CollisionPill::CollisionPill(olc::vf2d _position, float _radius, float _height, CollisionBody* _collision_body) :
    Pill(_position, _radius, _height),
    collision_body{_collision_body}{}

void CollisionPill::Update(){
    if(Engine::Get().vsync){
        Engine::Get().current_level->collision_system.ResolvePillVsLine(this, collision_body->velocity);
        collision_body->local_position += collision_body->velocity;
    }
    else{
        collision_happened_this_frame = false;

        olc::vf2d velocity_delta_time = collision_body->velocity * Engine::Get().GetDeltaTime();
        Engine::Get().current_level->collision_system.ResolvePillVsLine(this, velocity_delta_time);
        
        if(collision_happened_this_frame){
            //Console::Out(velocity_delta_time, "before");
            
            collision_body->local_position += velocity_delta_time;
            collision_body->velocity = velocity_delta_time/Engine::Get().GetDeltaTime();
            //Console::Out(velocity_delta_time, "after");
        }
        else{
            collision_body->local_position += velocity_delta_time;
        }

        //collision_body->velocity = velocity_delta_time;
        
        
    }

}