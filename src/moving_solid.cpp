#include <actor.h>
#include <ufo_maths.h>
#include <functional>
#include <sprite_reference.h>
#include <ufo_maths.h>
#include <ufo_engine.h>
#include <mouse.h>
#include "pingus_level.h"
#include "pingu.h"
#include <console.h>
#include "moving_solid.h"

MovingSolid::MovingSolid(Vector2f _local_position) : Actor(_local_position){

}

void MovingSolid::OnLevelEnter(Level* _level){
    level = dynamic_cast<PingusLevel*>(_level);
    spr = AddChild<SpriteReference>(
        "moving_solid",
        Vector2f(0.0f,0.0f),
        Vector2f(0.0f,0.0f),
        Vector2f(64.0f, 64.0f),
        Vector2f(1.0f, 1.0f),
        0.0f
    );
    SetZIndex(0);
    level->moving_solids_handles.push_back(this);

    max_left = local_position.x;
    max_right = local_position.x + 100.0f;
    
}

void MovingSolid::OnStart(Level* _level){
    //local_position = level->GetActiveCamera()->TransformScreenToWorld(Mouse::GetPosition());
    centre_position = local_position;
}

void MovingSolid::OnUpdate(){

    Vector2f delta_mouse = Vector2f(0.0f,0.0f);

    switch(movement_mode){
        case MovementModes::LEFT_RIGHT : {
            if(local_position.x < max_left) velocity.x = 100.0f;
            else if(local_position.x > max_right) velocity.x = -100.0f;
        }
        break;
        case MovementModes::CIRCLE : {
            Vector2f new_position = centre_position + Vector2f(std::cos(angle) * radius_x, std::sin(angle) * radius_y);
            velocity = (new_position-local_position)/Engine::Get().GetDeltaTime();
            angle+=angle_increment * Engine::Get().GetDeltaTime();
        }
        break;
        case MovementModes::STATIC_MOUSE : {
            Vector2f new_position = level->GetActiveCamera()->TransformScreenToWorld(Mouse::GetPosition());
            delta_mouse = new_position - local_position;
        }
        break;
        case MovementModes::CIRCLE_MOUSE : {
            Vector2f new_position = level->GetActiveCamera()->TransformScreenToWorld(Mouse::GetPosition()) + Vector2f(std::cos(angle) * radius_x, std::sin(angle) * radius_y);
            velocity = (new_position-local_position)/Engine::Get().GetDeltaTime();
            angle+=angle_increment * Engine::Get().GetDeltaTime();
        }
        break;
    }

        //pingu->velocity = velocity;
        //pingu->PinguCollisionMovingSolid(velocity,this);
        //velocity.x += 0.0f * Engine::Get().GetDeltaTime();
    {
        float total_movement_x = delta_mouse.x + velocity.x * Engine::Get().GetDeltaTime();
        local_position.x += total_movement_x;
        
        for(auto&& pingu : level->pingu_handles_all_pingus){
            if(pingu->IsOverlappingMovingSolid(Vector2f(pingu->local_position + Vector2f(0.0f,1.0f)), olc::WHITE, this)
                && !pingu->IsOverlappingMovingSolid(pingu->local_position, olc::WHITE, this)
            ){
                pingu->local_position.x += total_movement_x;
                if(pingu->IsOverlappingMovingSolid(pingu->local_position, olc::WHITE, this)){
                    pingu->local_position.x -= ufoMaths::Sign(total_movement_x);
                }
            }

            if(total_movement_x == 0.0f) continue;

            while(pingu->IsOverlappingMovingSolid(pingu->local_position, olc::WHITE, this)) pingu->local_position.x += ufoMaths::Sign(total_movement_x);
            
            /*if(pingu->IsOverlappingSolid(pingu->local_position)){
                pingu->state = pingu->die_by_fall;
                pingu->is_in_special_state = true;
                Console::PrintLine("Pingu got squashed");
            }*/
        }
    }

    {
        float total_movement_y = delta_mouse.y + velocity.y * Engine::Get().GetDeltaTime();

        local_position.y += total_movement_y;

        for(auto&& pingu : level->pingu_handles_all_pingus){
            
            if(pingu->IsOverlappingMovingSolid(Vector2f(pingu->local_position + Vector2f(0.0f,1.0f)), olc::WHITE, this)
                && !pingu->IsOverlappingMovingSolid(pingu->local_position, olc::WHITE, this)
            ){
                pingu->local_position.y += total_movement_y;
                if(pingu->IsOverlappingMovingSolid(pingu->local_position, olc::WHITE, this)){
                    pingu->local_position.y -= ufoMaths::Sign(total_movement_y);
                }
            
            }
            if(total_movement_y == 0.0f) continue;

            while(pingu->IsOverlappingMovingSolid(pingu->local_position, olc::WHITE, this)) pingu->local_position.y += ufoMaths::Sign(total_movement_y);
        }
    }

    //Console::Print("\n");

}