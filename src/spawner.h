#pragma once
#include <actor.h>
#include <level.h>
#include <timer.h>
#include <ufo_maths.h>
#include <sprite_reference.h>
#include "pingus_level.h"
#include "pingu.h"

/// @spawn;
class Spawner : public Actor{
public:
    PingusLevel* level = nullptr;

    /// @export;
    int number_of_pingus = 10;

    /// @export;
    float interval_milliseconds = 2000.0f;

    /// @export("face_right");
    int spawn_right_frequency = 1;

    /// @export("face_left");
    int spawn_left_frequency = 0;

    int released_pingus = 0;
    
    Timer spawn_timer;
    Spawner(Vector2f _local_position) : Actor(_local_position){

    }

    void OnLevelEnter(Level* _level){
        local_position.y = local_position.y -32.0f;
        
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
        assert((spawn_left_frequency > 0 || spawn_right_frequency > 0) && "Both spawn_frequency_left and spawn_frequency_right are 0.");
    }

    void OnUpdate(){
        if(level->fast_forward){
            spawn_timer.FastForward(level->number_of_updates_when_fast_forward);
        }

        if((spawn_timer.GetTimeLeft() <= 0.0f) && (number_of_pingus > 0)){
            Pingu* pingu = level->NewActor<Pingu>(local_position+Vector2f(10.0f,0.0f));

            if(released_pingus%(spawn_left_frequency+spawn_right_frequency) < spawn_right_frequency){
                pingu->face_direction = 1.0f;
            }
            else{
                pingu->face_direction = -1.0f;
            }

            spawn_timer.Start(interval_milliseconds);
            number_of_pingus--;
            released_pingus++;
        }
    }
};