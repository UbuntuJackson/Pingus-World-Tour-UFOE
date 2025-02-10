#ifndef PLAYER_BODY_H
#define PLAYER_BODY_H
#include "../external/olcPixelGameEngine.h"
#include "../keyboard/single_keyboard.h"
#include <cmath>
#include "../console/console.h"
#include "../ufo_clock/ufo_time.h"

class PlayerBody //Add local position to this
{
public:
    PlayerBody(olc::vf2d _position,Camera* _camera);
    olc::vf2d position;
    olc::vf2d velocity;
    Camera* camera;

    bool is_on_ground;
    bool was_on_ground;
    olc::Key right_button = olc::RIGHT;
    olc::Key left_button = olc::LEFT;
    olc::Key jump_button = olc::Z;

    bool jump_disabled;
    bool fall_disabled;

    void DisableJumpThisFrame();

    bool GetOnGround();
    

    float GROUND_ACCELERATION = 8000.0f; //member variable
    float GROUND_FRICTION = 6000.0f;
    float MAX_SPEED = 1000.0f;
    float JUMP_INITIAL_SPEED = -900.0; //member variable
    float FALLING_ACCELERATION = 2000.0f;
    float AIR_ACCELERATION = 7500.0f;
    float AIR_FRICTION = 5500.0f;
    void Acceleration(bool _left, bool _right, int _up = 0, int _down = 0);
    float GetHorizontalAcceleration();
    float GetHorizontalFriction();
    void Fall(float _falling_acceleration);
    void Jump(float _speed);
    void Update();
    virtual void OnUpdate();
};

#endif