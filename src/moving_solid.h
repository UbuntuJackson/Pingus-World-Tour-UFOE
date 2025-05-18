#pragma once
#include <actor.h>
#include <ufo_maths.h>
#include <functional>
#include <level_sprite_reference.h>

class PingusLevel;
class LevelSpriteReference;

///@spawn;
class MovingSolid : public Actor{
public:
    enum MovementModes{
        STATIC_MOUSE,
        LEFT_RIGHT,
        CIRCLE,
        CIRCLE_MOUSE
    };

    ///@export;
    int movement_mode = STATIC_MOUSE;

    /// @export;
    float max_travel_right = 0.0f;

    float max_left = 0.0f;
    float max_right = 0.0f;

    float angle = 0.0f;
    
    /// @export;
    float radius_x = 100.0f;
    /// @export;
    float radius_y = 100.0f;
    /// @export;
    float angle_increment = 2.0f;

    //Currently unused
    int moving_solid_id = 0;

    Vector2f centre_position;

    Vector2f velocity;
    PingusLevel* level = nullptr;
    LevelSpriteReference* spr = nullptr;

    MovingSolid(Vector2f _local_position);

    void OnLevelEnter(Level* _level);
    void OnStart(Level* _level);

    void OnUpdate();
};