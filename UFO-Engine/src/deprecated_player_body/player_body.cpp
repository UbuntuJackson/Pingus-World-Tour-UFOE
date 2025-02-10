#include <cmath>
#include "../external/olcPixelGameEngine.h"
#include "player_body.h"
#include "../camera/camera.h"
#include "../ufo_clock/ufo_time.h"
#include "../console/console.h"
#include "../ufo_maths/ufo_maths.h"

PlayerBody::PlayerBody(olc::vf2d _position,Camera* _camera) :
position{_position},
camera{_camera},
velocity{olc::vf2d(0.0f, 0.0f)}{}

void PlayerBody::Acceleration(bool _left, bool _right, int _up, int _down){
    float direction = int(_right) - int(_left);
    float acceleration = GetHorizontalAcceleration()*ufoTime::GetDeltaTime() * direction;
    float ground_friction = GetHorizontalFriction()*ufoTime::GetDeltaTime();
    if(MAX_SPEED > std::abs(velocity.x)) velocity.x += acceleration;

    float hspeed_prefriction = velocity.x;
    velocity.x-=ground_friction*ufoMaths::Sign(velocity.x);

    if(velocity.x * hspeed_prefriction < 0.0f) velocity.x = 0.0f;

    was_on_ground = is_on_ground;
    is_on_ground = false;
}

float PlayerBody::GetHorizontalAcceleration(){
    if(is_on_ground) return GROUND_ACCELERATION;
    else return AIR_ACCELERATION;
}

float PlayerBody::GetHorizontalFriction(){
    if(is_on_ground) return GROUND_FRICTION;
    else return AIR_FRICTION;
}

void PlayerBody::Fall(float _falling_acceleration){
    if(!is_on_ground) velocity.y += (_falling_acceleration * ufoTime::GetDeltaTime());
}

void PlayerBody::Jump(float _speed){
    if(jump_disabled) return;
    if(SingleKeyboard::GetKey(jump_button).is_pressed && is_on_ground){
        velocity.y = _speed;
    }
}

void PlayerBody::DisableJumpThisFrame(){
    jump_disabled = true;
}

bool PlayerBody::GetOnGround(){}

void PlayerBody::Update(){
    OnUpdate();
    Acceleration(SingleKeyboard::GetKey(left_button).is_held, SingleKeyboard::GetKey(right_button).is_held);
    if(position.y >= 150.0f*8.0f){
        is_on_ground = true;
        position.y = 150.0f*8.0f;
        velocity.y = 0.0f;
    }
    Fall(FALLING_ACCELERATION);
    Jump(JUMP_INITIAL_SPEED);
    position += velocity * ufoTime::GetDeltaTime();
    camera->position = position;
}

void PlayerBody::OnUpdate(){
    
}

/*
void PlayerBody::OnSelect(){
    PlayerBodyEditor.open();
    if(File::Exists())
}
*/