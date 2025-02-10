#include <cmath>
#include "../external/olcPixelGameEngine.h"
#include "player_body.h"
#include "../console/console.h"
#include "../ufo_maths/ufo_maths.h"
#include "../ufo_engine/ufo_engine.h"
#include "../actor/actor.h"
#include "../camera/camera.h"
#include "../shapes/circle.h"
#include "../level/level.h"
#include "../collision/collision_body.h"

PlayerBody::PlayerBody(olc::vf2d _position) :
CollisionBody(_position){

    if(Engine::Get().vsync){
        GROUND_ACCELERATION = 0.29f; //member variable
        GROUND_FRICTION = 0.094f;
        MAX_SPEED = 2.0f;
        JUMP_INITIAL_SPEED = -5.0; //member variable
        FALLING_ACCELERATION = 0.2f;
        AIR_ACCELERATION = 0.06f;
        AIR_FRICTION = 0.000001f;
    }
    else{
        const float FACTOR = 3000.0f;

        GROUND_ACCELERATION = 0.29f*FACTOR; //member variable
        GROUND_FRICTION = 0.094f*FACTOR;
        MAX_SPEED = 2.0f*FACTOR;
        JUMP_INITIAL_SPEED = -5.0*FACTOR; //member variable
        FALLING_ACCELERATION = 0.2f*FACTOR;
        AIR_ACCELERATION = 0.06f*FACTOR;
        AIR_FRICTION = 0.000001f*FACTOR;
    }
}

void PlayerBody::OnLevelEnter(Level* _level){
    CollisionBody::OnLevelEnter(_level);
    auto camera = std::make_unique<Camera>(olc::vf2d(0.0f, 0.0f));
    camera->clamp = true;
    AddChild(std::move(camera));

    _level->player_body_handles.push_back(this);
}

bool PlayerBody::IsOnFloor(){
    auto collision_system = &(Engine::Get().current_level->collision_system);

    for(const auto& line : collision_system->lines){
        CollisionData result = ufoMaths::RayVsRay(Ray2(local_position+olc::vf2d(0.0f, 7.0f), local_position+olc::vf2d(0.0f, 10.0f)), line);
        if(result.is_hit) return true;
    }
    return false;
}

void PlayerBody::Acceleration(bool _left, bool _right, int _up, int _down){
    //if(collision_normal.y < 0.0f) is_on_ground = true;
    //else is_on_ground = false;

    float direction = int(_right) - int(_left);
    if(int(_right)-int(_left) != 0){
        face_direction = direction;
    }

    float acceleration;
    float ground_friction;
    
    if(!Engine::Get().vsync){
        acceleration = GetHorizontalAcceleration()*Engine::Get().GetDeltaTime() * direction;
        ground_friction = GetHorizontalFriction()*Engine::Get().GetDeltaTime();
    }
    else{
        acceleration = GetHorizontalAcceleration()* direction;
        ground_friction = GetHorizontalFriction();
    }

    was_on_ground = is_on_ground;
    is_on_ground = false;

    if(MAX_SPEED > std::abs(velocity.x)) velocity.x += acceleration;

    float hspeed_prefriction = velocity.x;
    velocity.x-=ground_friction*ufoMaths::Sign(velocity.x);

    if(velocity.x * hspeed_prefriction < 0.0f) velocity.x = 0.0f;

    Fall(FALLING_ACCELERATION);
    if(is_on_ground) Jump(JUMP_INITIAL_SPEED);
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
    
    /*if(is_on_ground){
        
    }
    {
        if(!Engine::Get().vsync) velocity.y += (_falling_acceleration * Engine::Get().GetDeltaTime());

    }*/

    if(!IsOnFloor()){
        velocity.y += _falling_acceleration;
        is_on_ground = false;
    }
    else{
        is_on_ground = true;
        //velocity.y = 0.0f;
    }

}

void PlayerBody::OnBeforeMove(){
    /*if(collision_normal.y < 0.0f){
        float dp = ufoMaths::DotProduct(FALLING_ACCELERATION*olc::vf2d(0.0f,1.0f),velocity);
        velocity-=dp*velocity;
        Console::Out(dp);
    }*/
}

void PlayerBody::Jump(float _speed){
    if(jump_disabled) return;
    if(SingleKeyboard::Get().GetKey(jump_button).is_pressed && (is_on_ground)){
        velocity.y = _speed;
    }
}

void PlayerBody::DisableJumpThisFrame(){
    jump_disabled = true;
}

bool PlayerBody::GetOnGround(){return false;}

void PlayerBody::Accelerate(){
    Acceleration(SingleKeyboard::Get().GetKey(left_button).is_held, SingleKeyboard::Get().GetKey(right_button).is_held);
}

void PlayerBody::OnUpdate(){
    CollisionBody::OnUpdate();
    //Console::Out(was_on_ground);
    //Fall(FALLING_ACCELERATION);
    //Jump(JUMP_INITIAL_SPEED);
    /*if(position.y >= 150.0f*8.0f){
        is_on_ground = true;
        position.y = 150.0f*8.0f;
        velocity.y = 0.0f;
    }*/
    //local_position += velocity * Engine::Get().GetDeltaTime();
}