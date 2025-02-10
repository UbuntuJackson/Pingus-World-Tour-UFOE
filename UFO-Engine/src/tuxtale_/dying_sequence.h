#pragma once

#include "../level/actor.h"
#include "../ufo_maths/ufo_maths.h"
#include "../sprite_reference/sprite_reference.h"
#include "../ufo_clock/timer.h"

class Level;

class DyingSequence : public Actor{
public:
    int death_sprite_id = -1;
    Timer timer;
    Level* level = nullptr;
    Vector2f velocity;
    DyingSequence(Vector2f _local_position);
    
    void OnLevelEnter(Level *_level);

    void OnUpdate();


};