#pragma once
#include <shape.h>
#include <rectangle.h>
#include <level.h>
#include <ufo_maths.h>
#include "pingus_level.h"

class Goal : public Shape<ufo::Rectangle>{
public:
    PingusLevel* level = nullptr;

    /// @export;
    int number_of_pingus = 0;

    /// @export;
    int maximum_rescuable_pingus = 100;
    Goal(Vector2f _local_position) : Shape<ufo::Rectangle>(ufo::Rectangle(Vector2f(0.0f, 0.0f),Vector2f(16.0f,16.0f)),_local_position){
        
    }

    void OnLevelEnter(Level* _level){
        local_position.y -= shape.size.y;
        level = dynamic_cast<PingusLevel*>(_level);
        level->goals.push_back(this);

    }

    void OnStart(Level* _level){
        level->required_pingus += number_of_pingus;
        level->maximum_rescuable_pingus = maximum_rescuable_pingus;
    }
    
};