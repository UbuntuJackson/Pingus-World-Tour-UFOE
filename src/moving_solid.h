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
        STATIC_MOUSE,
        CIRCLE_MOUSE
    };

    int movement_mode = CIRCLE;

    /// @export;
    float max_travel_right = 0.0f;

    float max_left = 0.0f;
    float max_right = 0.0f;
    float angle = 0.0f;
    /// @export;
    float radius = 100.0f;
    /// @export;
    float angle_increment = 2.0f;

    Vector2f centre_position;

    Vector2f velocity;
    PingusLevel* level = nullptr;
    SpriteReference* spr = nullptr;

    MovingSolid(Vector2f _local_position);

    void OnLevelEnter(Level* _level);
    void OnStart(Level* _level);

    void OnUpdate();
};