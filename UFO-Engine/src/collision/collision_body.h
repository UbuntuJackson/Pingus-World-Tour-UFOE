#ifndef COLLISION_BODY_H
#define COLLISION_BODY_H
#include <memory>
#include "../external/olcPixelGameEngine.h"
#include "../ufo_engine/ufo_engine.h"
#include "../keyboard/single_keyboard.h"
#include "../ufo_input/ufo_input.h"
#include "../drawing_system/drawing_system.h"
#include "../camera/camera.h"
#include "../mouse/mouse.h"
#include "../level/level.h"
#include "../shapes/shape.h"
#include "../shapes/raw_shape_base.h"
#include "../shapes/collision_circle.h"
#include "../actor/actor.h"

class CollisionBody : public Actor{
public:
    olc::vf2d velocity = {0.0f, 0.0f};
    olc::vf2d acceleration = {1000.0f, 1000.0f};
    olc::vf2d collision_normal = {0.0f, 0.0f};
    //bool collision_happened_this_frame = false;
    RawShapeBase* shape = nullptr;
    int shape_id = -1;
    CollisionBody(olc::vf2d _position) : Actor(_position){
        if(Engine::Get().vsync) acceleration = {0.1f, 0.1f};
    }
    virtual void OnLevelEnter(Level* _level){
        SetShape(CollisionCircle(this,olc::vf2d(0.0f,0.0f), 16.0f),olc::vf2d(0.0f, 0.0f));

        _level->collision_body_handles.push_back(this);
    }

    template<typename tCollisionShape>
    void SetShape(tCollisionShape _shape, olc::vf2d _position){
        Console::Out("setshape",shape_id);
        
        if(shape != nullptr) Engine::Get().current_level->QueueForPurge(shape_id);

        std::unique_ptr<Shape<tCollisionShape>> u_shape = std::make_unique<Shape<tCollisionShape>>(_shape,_position);
        shape = &(u_shape->shape);
        shape_id = u_shape->id;
        AddChild(std::move(u_shape));
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
        /*if(Engine::Get().vsync) local_position += velocity; //* Engine::Get().GetDeltaTime();
        else{
            //Console::Out(collision_happened_this_frame);
            
            //if(collision_happened_this_frame) local_position += velocity/Engine::Get().GetDeltaTime();
            //else local_position += velocity;
            local_position += velocity;
            
        }*/
    }
    virtual void OnBeforeMove(){

    }
    void OnUpdate(){
        //shape->position = local_position;
        if(SingleKeyboard::Get().GetKey(olc::P).is_pressed) velocity = {0.0f, 0.0f};
        //if(Mouse::GetRightButton().is_pressed) local_position = Engine::Get().current_level->GetActiveCamera()->TransformScreenToWorld(Mouse::GetPosition());
    }
    void HandleUpdate(){
        Engine::Get().current_level->collision_system.UpdateCollisionBody(this);
    }

    void OnDebugDraw(Camera* _camera){
        shape->Draw(_camera);
        if(Engine::Get().vsync) DrawingSystem::Draw(Ray2(position, (position+velocity)), _camera, olc::BLACK);
        else DrawingSystem::Draw(Ray2(position, (position+velocity*Engine::Get().GetDeltaTime())), _camera, olc::BLACK);
    }
};

#endif