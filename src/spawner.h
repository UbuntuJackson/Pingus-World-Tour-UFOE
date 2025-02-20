#pragma once
#include <actor.h>
#include <level.h>
#include <timer.h>
#include <ufo_maths.h>
#include <sprite_reference.h>
#include "pingus_level.h"
#include "pingu.h"


class Spawner : public Actor{
public:
    PingusLevel* level = nullptr;

    /// @export;
    int number_of_pingus = 10;

    /// @export;
    float interval_milliseconds = 2000.0f;
    
    Timer spawn_timer;
    Spawner(Vector2f _local_position) : Actor(_local_position){

    }

    void OnLevelEnter(Level* _level){
        
        spawn_timer.Start(interval_milliseconds);
        level = dynamic_cast<PingusLevel*>(_level);
        
        AddChild<SpriteReference>(
            "spawner",
            Vector2f(-0.0f, -0.0f),
            Vector2f(0.0f, 0.0f),
            Vector2f(32.0f, 32.0f),
            Vector2f(1.0f,1.0f),
            0.0f
        );
    }

    void OnStart(Level* _level){
        level->total_number_of_pingus += number_of_pingus;
    }

    void OnUpdate(){
        if((spawn_timer.GetTimeLeft() <= 0.0f) && (number_of_pingus > 0)){
            level->NewActor<Pingu>(local_position);
            spawn_timer.Start(interval_milliseconds);
            number_of_pingus--;
        }
    }
};