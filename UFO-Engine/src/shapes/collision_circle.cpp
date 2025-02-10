#include "collision_circle.h"
#include "../ufo_engine/ufo_engine.h"
#include "../external/olcPixelGameEngine.h"
#include "../camera/camera.h"
#include "../collision/collision_body.h"
#include "../drawing_system/drawing_system.h"
#include "../console/console.h"

CollisionCircle::CollisionCircle(CollisionBody* _collision_body, olc::vf2d _position, float _radius) :
    collision_body{_collision_body},
    Circle(_position, _radius)
    {}

void CollisionCircle::Update(){

    /*if(Engine::Get().vsync){
        Engine::Get().current_level->collision_system.ResolveCircleVsLine(this, collision_body->velocity);
        collision_body->local_position = position;
        collision_body->local_position += collision_body->velocity;
    }
    else{
        collision_happened_this_frame = false;

        olc::vf2d velocity_delta_time = collision_body->velocity * Engine::Get().GetDeltaTime();
        
        Engine::Get().current_level->collision_system.ResolveCircleVsLine(this, velocity_delta_time);

        if(collision_body->collision_normal != olc::vf2d(0.0f, 0.0f)) collision_body->collision_normal = collision_body->collision_normal.norm();

        position+=collision_body->collision_normal * 0.1f;
        if(collision_body->collision_normal != olc::vf2d(0.0f, 0.0f)) Console::Out(collision_body->collision_normal);
        collision_body->local_position = position;

        if(collision_happened_this_frame){
            Console::Out("collision");

            collision_body->local_position += velocity_delta_time;
            //Console::Out(ufoMaths::Sign(ufoMaths::DotProduct(velocity_delta_time, collision_body->velocity)));
            collision_body->velocity = velocity_delta_time / Engine::Get().GetDeltaTime();
            //collision_happened_last_frame = true;

        }
        else{
            //Console::Out("Not collision");
            collision_body->local_position += velocity_delta_time;
        }

    }*/

    if(/*Engine::Get().vsync*/ false){
        std::vector<std::pair<float, Ray2>> hit_line_segments_empty;

        Engine::Get().current_level->collision_system.ResolveCircleVsLine202412(this, collision_body, collision_body->velocity,hit_line_segments_empty);
        collision_body->local_position = position;
        collision_body->local_position += collision_body->velocity;
    }
    else{
        std::vector<std::pair<float, Ray2>> hit_line_segments_empty;

        collision_happened_this_frame = false;

        olc::vf2d velocity_delta_time = collision_body->velocity * Engine::Get().GetDeltaTime();
        
        Engine::Get().current_level->collision_system.ResolveCircleVsLine202412(this,collision_body, velocity_delta_time,hit_line_segments_empty);

        if(collision_body->collision_normal != olc::vf2d(0.0f, 0.0f)) collision_body->collision_normal = collision_body->collision_normal.norm();

        if(hit_line_segments_empty.size() > 0){
            Console::Out("=== Collision History ===");
            for(const auto& line : hit_line_segments_empty){
                Console::Out(line.second.Start(),",", line.second.End());
            }
            Console::Out("Averaged collision normal: ", collision_body->collision_normal, '\n');
        }

        position+=collision_body->collision_normal*0.0001f;
        collision_body->local_position = position;

        if(collision_happened_this_frame){

            collision_body->local_position += velocity_delta_time;
            //Console::Out(ufoMaths::Sign(ufoMaths::DotProduct(velocity_delta_time, collision_body->velocity)));
            collision_body->velocity = velocity_delta_time / Engine::Get().GetDeltaTime();
            //collision_happened_last_frame = true;

        }
        else{
            //Console::Out("Not collision");
            collision_body->local_position += velocity_delta_time;
        }

    }
}

void CollisionCircle::Draw(Camera *_camera){
    DrawingSystem::Draw(*this,_camera, colour);
}