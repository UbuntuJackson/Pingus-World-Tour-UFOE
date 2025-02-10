//This file is deprecated

#ifndef TCOLLISION_BODY_H
#define TCOLLISION_BODY_H

#include "../actor/actor.h"
#include "../external/olcPixelGameEngine.h"
#include "../ufo_engine/ufo_engine.h"
#include "../keyboard/single_keyboard.h"
#include "../ufo_input/ufo_input.h"
#include "../drawing_system/drawing_system.h"
#include "../camera/camera.h"
#include "../mouse/mouse.h"
#include "level.h"

template<typename tShape>
class TCollisionBody : public Actor{
public:
    olc::vf2d velocity = {0.0f, 0.0f};
    olc::vf2d acceleration = {5000.0f, 5000.0f};
    olc::vf2d collision_normal = {0.0f, 0.0f};
    tShape shape;
    TCollisionBody(olc::vf2d _position, tShape _shape) : Actor(_position), shape{_shape}{
        if(Engine::Get().vsync) acceleration = {0.1f, 0.1f};
    }
    virtual void OnLevelEnter(Level* _level){
        _level->collision_body_handles.push_back(this);
    }
    virtual olc::vf2d OnAccelerate(){return olc::vf2d(0.0f, 0.0f);}
    virtual void Accelerate(){
        olc::vf2d acceleration_this_frame = {0.0f, 0.0f};

        //int direction_x = (int)SingleKeyboard::Get().GetKey(olc::RIGHT).is_held - (int)SingleKeyboard::Get().GetKey(olc::LEFT).is_held;
        //int direction_y = (int)SingleKeyboard::Get().GetKey(olc::DOWN).is_held - (int)SingleKeyboard::Get().GetKey(olc::UP).is_held;
        acceleration_this_frame = OnAccelerate();

        if(SingleKeyboard::Get().GetKey(olc::B).is_pressed) velocity = velocity.norm()*8.0f;
        if(SingleKeyboard::Get().GetKey(olc::V).is_pressed) Console::Out(velocity);

        if(Engine::Get().vsync) velocity += acceleration_this_frame; //* Engine::Get().GetDeltaTime();
        else velocity += acceleration_this_frame * Engine::Get().GetDeltaTime();
    }
    virtual void Move(){
        if(Engine::Get().vsync) local_position += velocity; //* Engine::Get().GetDeltaTime();
        else local_position += velocity /** Engine::Get().GetDeltaTime()*/;
    }
    virtual void OnBeforeMove(){

    }
    void OnUpdate(){
        shape.position = local_position;
        if(SingleKeyboard::Get().GetKey(olc::P).is_pressed) velocity = {0.0f, 0.0f};
        //if(Mouse::GetRightButton().is_pressed) local_position = Engine::Get().current_level->GetActiveCamera()->TransformScreenToWorld(Mouse::GetPosition());
    }
    void HandleUpdate(){
        Engine::Get().current_level->tcollision_system.UpdateCollisionBody(this);
    }
    void OnDraw(Camera* _camera){
        if(!Engine::Get().all_shapes_visible) return;
        DrawingSystem::Draw(shape,_camera, olc::VERY_DARK_BLUE);
        if(Engine::Get().vsync) DrawingSystem::Draw(Ray2(position, (position+velocity)), _camera, olc::BLACK);
        else DrawingSystem::Draw(Ray2(position, (position+velocity*Engine::Get().GetDeltaTime())), _camera, olc::BLACK);
    }
};

#endif