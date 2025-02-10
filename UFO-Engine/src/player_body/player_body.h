#ifndef PLAYER_BODY_H
#define PLAYER_BODY_H
#include "../external/olcPixelGameEngine.h"
#include "../keyboard/single_keyboard.h"
#include <cmath>
#include "../console/console.h"
#include "../actor/actor.h"
#include "../collision/collision_body.h"
#include "../ufo_engine/ufo_engine.h"

class Level;

//template<typename tShape>
class PlayerBody : public CollisionBody{
public:
    PlayerBody(olc::vf2d _position);
    void OnLevelEnter(Level* _level);

    bool IsOnFloor();

    bool is_on_ground;
    bool was_on_ground;

    olc::Key right_button = olc::RIGHT;
    olc::Key left_button = olc::LEFT;
    olc::Key jump_button = olc::Z;
    float face_direction = 1.0f;

    bool jump_disabled = false;
    bool fall_disabled = false;

    void DisableJumpThisFrame();

    bool GetOnGround();
    

    float GROUND_ACCELERATION = 900.0f; //member variable
    float GROUND_FRICTION = 400.0f;
    float MAX_SPEED = 1000.0f;
    float JUMP_INITIAL_SPEED = -33000.0; //member variable
    float FALLING_ACCELERATION = 520.0f;
    float AIR_ACCELERATION = 200.0f;
    float AIR_FRICTION = 0.0f;
    void Acceleration(bool _left, bool _right, int _up = 0, int _down = 0);
    float GetHorizontalAcceleration();
    float GetHorizontalFriction();
    void Fall(float _falling_acceleration);
    virtual void Jump(float _speed);
    void Accelerate();
    void OnUpdate();
    void OnBeforeMove();
};

#endif