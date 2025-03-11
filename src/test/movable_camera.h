#pragma once
#include <actor.h>
#include <ufo_maths.h>

class Level;
class Camera;

/// @spawn;
class MovableCamera : public Actor{
public:
    const float horisontal_speed = 200.0f;

    Camera* camera = nullptr;

    MovableCamera(Vector2f _local_position) : Actor(_local_position){

    }

    void OnLevelEnter(Level* _level);

    void OnUpdate();
};