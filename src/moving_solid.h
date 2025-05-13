#pragma once
#include <actor.h>
#include <ufo_maths.h>
#include <functional>
#include <level_sprite_reference.h>

class PingusLevel;

///@spawn;
class MovingSolid : public Actor{
public:
    Vector2f velocity;
    PingusLevel* level = nullptr;

    MovingSolid(Vector2f _local_position);

    void OnLevelEnter(Level* _level);

    void OnUpdate();
};