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
    local_position = level->GetActiveCamera()->TransformScreenToWorld(Mouse::GetPosition());
}

void MovingSolid::OnUpdate(){

    Vector2f delta_mouse = Mouse::Get().GetDeltaPosition()/level->GetActiveCamera()->scale;

    local_position.x += delta_mouse.x;

    switch(movement_mode){
        case MovementModes::LEFT_RIGHT : {
            if(local_position.x < max_left) velocity.x = 100.0f;
            else if(local_position.x > max_right) velocity.x = -100.0f;
        }
        break;
        case MovementModes::CIRCLE : {
            velocity.x = std::cos(angle) * 100.0f;
            velocity.y = std::sin(angle) * 100.0f;
            angle+=2.0f * Engine::Get().GetDeltaTime();
        }
        break;
        case MovementModes::STATIC : {

        }
        break;
    }
        
        //pingu->velocity = velocity;
        //pingu->PinguCollisionMovingSolid(velocity,this);
        //velocity.x += 0.0f * Engine::Get().GetDeltaTime();

        
    local_position.x += velocity.x * Engine::Get().GetDeltaTime();
    
    for(auto&& pingu : level->pingu_handles_all_pingus){
        if(pingu->IsOverlappingMovingSolid(Vector2f(pingu->local_position + Vector2f(0.0f,1.0f)), olc::WHITE, this)){
            pingu->local_position.x += velocity.x * Engine::Get().GetDeltaTime() + delta_mouse.x;
            
        }

        while(pingu->IsOverlappingMovingSolid(pingu->local_position, olc::WHITE, this)) pingu->local_position.x += ufoMaths::Sign(velocity.x+delta_mouse.x);
        
    }

    local_position.y += delta_mouse.y;

    local_position.y += velocity.y * Engine::Get().GetDeltaTime();

    for(auto&& pingu : level->pingu_handles_all_pingus){
        
        if(pingu->IsOverlappingMovingSolid(Vector2f(pingu->local_position + Vector2f(0.0f,1.0f)), olc::WHITE, this)){
            pingu->local_position.y += velocity.y * Engine::Get().GetDeltaTime() + delta_mouse.y;
        }

        while(pingu->IsOverlappingMovingSolid(pingu->local_position, olc::WHITE, this)) pingu->local_position.y += ufoMaths::Sign(velocity.y+delta_mouse.y);
    }

    Console::Print("\n");

}