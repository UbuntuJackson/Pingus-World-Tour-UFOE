#pragma once
#include <actor.h>
#include <ufo_maths.h>
#include <functional>
#include <sprite_reference.h>

class PingusLevel;
class SpriteReference;

///@spawn;
class MovingSolid : public Actor{
public:
    enum MovementModes{
        LEFT_RIGHT,
        CIRCLE,
        STATIC
    };

    int movement_mode = STATIC;
    float max_left = 0.0f;
    float max_right = 0.0f;
    float angle = 0.0f;

    Vector2f velocity;
    PingusLevel* level = nullptr;
    SpriteReference* spr = nullptr;

    MovingSolid(Vector2f _local_position);

    void OnLevelEnter(Level* _level);
    void OnStart(Level* _level);

    void OnUpdate();
};