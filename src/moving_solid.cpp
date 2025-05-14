#include <actor.h>
#include <ufo_maths.h>
#include <functional>
#include <level_sprite_reference.h>
#include "pingus_level.h"
#include "pingu.h"
#include "moving_solid.h"

MovingSolid::MovingSolid(Vector2f _local_position) : Actor(_local_position){

}

void MovingSolid::OnLevelEnter(Level* _level){
    level = dynamic_cast<PingusLevel*>(_level);
}

void MovingSolid::OnUpdate(){

    for(auto&& pingu : level->pingu_handles_all_pingus){
        pingu->velocity = velocity;
        pingu->PinguCollisionMovingSolid(velocity,this);
    }

}