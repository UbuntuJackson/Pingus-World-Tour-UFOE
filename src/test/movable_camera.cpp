#include <actor.h>
#include <ufo_maths.h>
#include <sprite_reference.h>
#include <ufo_engine.h>
#include <single_keyboard.h>
#include <camera.h>
#include "movable_camera.h"

void MovableCamera::OnLevelEnter(Level* _level){
    AddChild<SpriteReference>(
        "spawner",
        Vector2f(0.0f, 0.0f),
        Vector2f(0.0f, 0.0f),
        Vector2f(32.0f, 32.0f),
        Vector2f(1.0f,1.0f),
        0.0f
    );
    SetZIndex(4);

    camera = AddChild<Camera>(Vector2f(0.0f, 0.0f));
}

void MovableCamera::OnUpdate(){
    //Move camera for x
    {
        int dir_x = int(SingleKeyboard::Get().GetKey(olc::D).is_held) - int(SingleKeyboard::Get().GetKey(olc::A).is_held);

        local_position.x += Engine::Get().GetDeltaTime() * dir_x * horisontal_speed;
        if(dir_x != 0){
            camera->local_position.x+=10.0f*dir_x*Engine::Get().GetDeltaTime();
        }
        else if(camera->local_position.x != 0.0f){
            Vector2f camera_position_before_offset = camera->local_position;
            camera->local_position.x += 0.9f * ufoMaths::Sign(0.0f-camera->local_position.x);

            if((0.0f-camera->local_position.x) * (0.0f-camera_position_before_offset.x) < 0){
                camera->local_position.x = 0.0f;
            }
        }
    }

    //Move camera for y
    {
        int dir_y = int(SingleKeyboard::Get().GetKey(olc::S).is_held) - int(SingleKeyboard::Get().GetKey(olc::W).is_held);

        local_position.y += Engine::Get().GetDeltaTime() * dir_y * horisontal_speed;
        if(dir_y != 0){
            camera->local_position.y+=10.0f*dir_y*Engine::Get().GetDeltaTime();
        }
        else if(camera->local_position.y != 0.0f){
            Vector2f camera_position_before_offset = camera->local_position;
            camera->local_position.y += 0.9f * ufoMaths::Sign(0.0f-camera->local_position.y);

            if((0.0f-camera->local_position.y) * (0.0f-camera_position_before_offset.y) < 0){
                camera->local_position.y = 0.0f;
            }
        }
    }
}