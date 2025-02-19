#include <cmath>
#include <algorithm>
#include "../external/olcPixelGameEngine.h"
#include "camera.h"
#include "../console/console.h"
#include "../ufo_maths/ufo_maths.h"
#include "../shapes/circle.h"
#include "../shapes/rectangle.h"
#include "../sprite_reference/sprite_reference.h"
#include "../actor/actor.h"
#include "../ufo_engine/ufo_engine.h"
#include "../shapes/pill.h"
#include "../keyboard/single_keyboard.h"

Camera::Camera(olc::vf2d _position):
Actor(_position),
scale{1.0f},
original_local_position{_position}
{
    view = Bounds{0.0f,(float)Engine::Get().pixel_game_engine.GetWindowSizeInPixles().x, 0.0f, (float)Engine::Get().pixel_game_engine.GetWindowSizeInPixles().y};
    world = Bounds{0.0f,Engine::Get().current_level->level_size.x, 0.0f ,Engine::Get().current_level->level_size.y};
    clamp = true;
    Engine::Get().current_level->camera_handles.push_back(this);
}

void Camera::OnStart(Level* _level){
    Console::Out("Initial camera position:", local_position);
    //_level->ActiveCameraLookAround(true,true);
}

void
Camera::ClampLocalPosition(){
    if(clamp){ //for clamping x in this case, the same applies for y
        int scaled_width_half = (1.0f/scale)*view.GetWidthHalf(); //bigger scale -> distance on screen smaller -> less world-space needed to clamp.
        int scaled_height_half = (1.0f/scale)*view.GetHeightHalf();

        //Console::Out("position", position.x);
        //Console::Out("local position", local_position.x);

        /*if(position.x < world.x0 + scaled_width_half){
            local_position.x = world.x0 + scaled_width_half;
        }
        if(position.x > world.x1 - scaled_width_half){
            local_position.x = world.x0 - scaled_width_half;
        }

        if(position.y < world.y0 + scaled_height_half){
            local_position.y = world.y0 + scaled_height_half;
        }
        if(position.y > world.y1 - scaled_height_half){
            local_position.y = world.y1 - scaled_height_half;
        }*/
    }
}

void
Camera::EarlyUpdate(){
    
    local_position = original_local_position;
}

void
Camera::HandleUpdate(){ //Updates as a handle.
    /*if(clamp){ //for clamping x in this case, the same applies for y
        int scaled_width_half = (1.0f/scale)*view.GetWidthHalf(); //bigger scale -> distance on screen smaller -> less world-space needed to clamp.
        int scaled_height_half = (1.0f/scale)*view.GetHeightHalf();

        position.x = std::max(world.x0 + scaled_width_half, position.x); //world-wise this is the middle of the screen.
        position.x = std::min(world.x1 - scaled_width_half, position.x); //will subtract -view.GetWidthHalf() from here.
        position.y = std::max(world.y0 + scaled_height_half, position.y); //world-wise this is the middle of the screen.
        position.y = std::min(world.y1 - scaled_height_half, position.y); //will subtract -view.GetWidthHalf() from here.
    }*/

    if(clamp){
        int scaled_width_half = (1.0f/scale)*view.GetWidthHalf(); //bigger scale -> distance on screen smaller -> less world-space needed to clamp.
        int scaled_height_half = (1.0f/scale)*view.GetHeightHalf();

        //Console::Out(Engine::Get().current_level->camera_handles.size());

        if(GetGlobalPosition().x < world.x0 + scaled_width_half){
            local_position.x -= (GetGlobalPosition().x - (world.x0 + scaled_width_half));
            
        }
        else if(GetGlobalPosition().x > world.x1 - scaled_width_half){
            local_position.x -= (GetGlobalPosition().x - (world.x1 - scaled_width_half));
            
        }

        if(GetGlobalPosition().y < world.y0 + scaled_height_half){
            local_position.y -= (GetGlobalPosition().y - (world.y0 + scaled_height_half));
            
        }
        else if(GetGlobalPosition().y > world.y1 - scaled_height_half){
            local_position.y -= (GetGlobalPosition().y - (world.y1 - scaled_height_half));
            
        }
        
        if(SingleKeyboard::Get().GetKey(olc::C).is_pressed) local_position.x = 0.0f;
        //Engine::Get().Quit();

        //Console::Out("Local position set elsewhere", local_position);

        local_position_set_elsewhere = true;

    }
}

olc::vf2d
Camera::Transform(const olc::vf2d& _position){ //position is supposed to be in the middle, everything else is drawn in the middle

    int screen_position_x = ((_position.x-GetGlobalPosition().x)*scale) + view.GetWidthHalf(); //subtract with unscaled view to centre
    int screen_position_y = ((_position.y-GetGlobalPosition().y)*scale) + view.GetHeightHalf(); //subtract with unscaled view to centre
    return olc::vf2d(screen_position_x, screen_position_y); //im not calculating screen_position_y here, just pretend I did this calculation for that too, haha
}

Rectangle Camera::Transform(const Rectangle& _rectangle){
    Rectangle transformed_rectangle{
    Transform(_rectangle.position),
    _rectangle.size * scale};
    return transformed_rectangle;
}
Circle Camera::Transform(const Circle& _circle){
    Circle transformed_circle{Transform(_circle.position), _circle.radius * scale};
    return transformed_circle;
}
Ray2 Camera::Transform(const Ray2& _ray){
    return Ray2(
        Transform(_ray.Start()),
        Transform(_ray.End())
    );
}

SpriteReference Camera::Transform(const SpriteReference& _sprite){
    olc::vf2d transformed_position = Transform(_sprite.position);
    SpriteReference transformed_sprite(
        _sprite.key,
        transformed_position,
        _sprite.offset,
        _sprite.frame_size,
        scale * _sprite.scale,
        _sprite.rotation,
        _sprite.z_index, true
    );
    transformed_sprite.position = transformed_position;
    transformed_sprite.current_frame_index = _sprite.current_frame_index;
    transformed_sprite.tint = _sprite.tint;
    return transformed_sprite;
}

Pill
Camera::Transform(const Pill& _pill){
    Pill transformed_pill = Pill(
        Transform(_pill.position),
        _pill.radius*scale,
        _pill.height*scale
    );
    return transformed_pill;
}

olc::vf2d
Camera::TransformScreenToWorld(const olc::vf2d& _screen_position){ //position is supposed to be in the middle, everything else is drawn in the middle
    
    /*if(clamp){ //for clamping x in this case, the same applies for y
        int scaled_width_half = (1.0f/scale)*world.GetWidthHalf(); //bigger scale -> distance on screen smaller -> less world-space needed to clamp.

        position.x = std::max(world.x0 + scaled_width_half, position.x - scaled_width_half); //world-wise this is the middle of the screen.
        position.x = std::min(world.x1 - scaled_width_half, position.x + scaled_width_half); //will subtract -view.GetWidthHalf() from here.
    }*/
    olc::vf2d world_position = {0.0f, 0.0f};
    world_position.x = (_screen_position.x - view.GetWidthHalf())/scale + GetGlobalPosition().x; //subtract with unscaled view to centre
    world_position.y = (_screen_position.y - view.GetHeightHalf())/scale + GetGlobalPosition().y; //subtract with unscaled view to centre
    return world_position; //im not calculating screen_position_y here, just pretend I did this calculation for that too, haha
}

bool
Camera::IsOnScreen(olc::vf2d _position, olc::vf2d _offset){
    return ufoMaths::RectangleVsPoint(GetOnScreenRectangleInWorld(_offset),_position);
}

Rectangle
Camera::GetOnScreenRectangleInWorld(olc::vf2d _offset){
    float x0 = (GetGlobalPosition().x-(view.GetWidthHalf())/scale-_offset.x*scale);
    float x1 = (GetGlobalPosition().x+(view.GetWidthHalf())/scale+_offset.x*scale);
    float y0 = (GetGlobalPosition().y-(view.GetHeightHalf())/scale-_offset.y*scale);
    float y1 = (GetGlobalPosition().y+(view.GetHeightHalf())/scale+_offset.y*scale);
    return Rectangle({x0, y0}, {x1-x0, y1-y0});
}