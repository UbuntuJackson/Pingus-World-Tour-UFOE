#ifndef TOP_DOWN_BODY_H
#define TOP_DOWN_BODY_H
#include "../external/olcPixelGameEngine.h"
#include "../keyboard/single_keyboard.h"
#include <cmath>
#include "../console/console.h"
#include "../ufo_clock/ufo_time.h"

class TopDownBody
{
public:
    TopDownBody(olc::vf2d _position,Camera* _camera);
    olc::vf2d position;
    olc::vf2d velocity;
    Camera* camera;

    bool is_on_ground;
    bool was_on_ground;
    olc::Key right_button = olc::RIGHT;
    olc::Key left_button = olc::LEFT;
    olc::Key up_button = olc::UP;
    olc::Key down_button = olc::DOWN;
    olc::Key jump_button = olc::Z;

    bool jump_disabled;
    bool fall_disabled;

    void DisableJumpThisFrame();

    bool GetOnGround();
    
    float GetHorizontalAcceleration();
    float GetVerticalAcceleration();

    float HORIZONTAL_ACCELERATION = 8000.0f; //member variable
    float VERTICAL_ACCELERATION = 8000.0f; //member variable
    float HORIZONTAL_FRICTION = 6000.0f;
    float VERTICAL_FRICTION = 6000.0f;
    float HORIZONTAL_MAX_SPEED = 1000.0f;
    float VERTICAL_MAX_SPEED = 1000.0f;
    float JUMP_INITIAL_SPEED = -600.0; //member variable
    float FALLING_ACCELERATION = 1000.0f;

    void Acceleration(bool _left, bool _right, int _up, int _down);
    void Update();
    virtual void OnUpdate();
};

#endif