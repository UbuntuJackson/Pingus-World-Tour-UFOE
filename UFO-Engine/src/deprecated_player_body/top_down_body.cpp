#include <cmath>
#include "../external/olcPixelGameEngine.h"
#include "top_down_body.h"
#include "../camera/camera.h"
#include "../ufo_clock/ufo_time.h"
#include "../console/console.h"
#include "../ufo_maths/ufo_maths.h"

TopDownBody::TopDownBody(olc::vf2d _position,Camera* _camera) :
position{_position},
camera{_camera},
velocity{olc::vf2d(0.0f, 0.0f)}{}

float
TopDownBody::GetHorizontalAcceleration(){
    return HORIZONTAL_ACCELERATION;
}

float
TopDownBody::GetVerticalAcceleration(){
    return VERTICAL_ACCELERATION;
}

void TopDownBody::Acceleration(bool _left, bool _right, int _up, int _down){
    float direction_horizontal = int(_right) - int(_left);
    float direction_vertical = int(_up) - int(_down);
    float horizontal_acceleration = GetHorizontalAcceleration()*ufoTime::GetDeltaTime() * direction_horizontal;
    float vertical_acceleration = GetVerticalAcceleration()*ufoTime::GetDeltaTime() * direction_vertical;

    float horizontal_friction = HORIZONTAL_FRICTION*ufoTime::GetDeltaTime();
    if(HORIZONTAL_MAX_SPEED > std::abs(velocity.x)) velocity.x += horizontal_acceleration;

    float vertical_friction = VERTICAL_FRICTION*ufoTime::GetDeltaTime();
    if(VERTICAL_MAX_SPEED > std::abs(velocity.y)) velocity.y += vertical_acceleration;

    float hspeed_prefriction = velocity.x;
    velocity.x-=horizontal_friction*ufoMaths::Sign(velocity.x);

    float vspeed_prefriction = velocity.y;
    velocity.y-=vertical_friction*ufoMaths::Sign(velocity.y);

    if(velocity.x * hspeed_prefriction < 0.0f) velocity.x = 0.0f;
    if(velocity.y * vspeed_prefriction < 0.0f) velocity.y = 0.0f;
}

void TopDownBody::Update(){
    OnUpdate();
    Acceleration(SingleKeyboard::GetKey(left_button).is_held, SingleKeyboard::GetKey(right_button).is_held,
                SingleKeyboard::GetKey(up_button).is_held, SingleKeyboard::GetKey(down_button).is_held);
    position += velocity * ufoTime::GetDeltaTime();
    camera->position = position;
}

void TopDownBody::OnUpdate(){
}