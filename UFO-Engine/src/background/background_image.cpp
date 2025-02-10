#include <string>
#include "../external/olcPixelGameEngine.h"
#include "background_image.h"
#include "../sprite_reference/sprite_reference.h"
#include "../camera/camera.h"
#include "../ufo_engine/ufo_engine.h"
#include "../drawing_system/drawing_system.h"
#include "../ufo_maths/ufo_maths.h"

BackgroundImage::BackgroundImage(std::string _key, olc::vf2d _position, olc::vf2d _offset, olc::vf2d _frame_size, olc::vf2d _scale, float _rotation, int _z_index, olc::vf2d _parallax, bool _temporary) :
    parallax{_parallax},
    SpriteReference(_key, _position, _offset, _frame_size, _scale, _rotation, _z_index, _temporary){
        Engine::Get().current_level->background_image_handles.push_back(this);
    }

void
BackgroundImage::OnDraw(Camera* _camera){
    olc::vf2d screen_size = Engine::Get().pixel_game_engine.GetWindowSizeInPixles();

    SpriteReference transformed_spr = SpriteReference(
        key,
        _camera->Transform(_camera->position*parallax),
        offset,
        frame_size,
        scale*_camera->scale,
        rotation,
        z_index,
        true
    );

    olc::vf2d scaled_frame_size = frame_size*_camera->scale;

    transformed_spr.position.x = ufoMaths::Wrap(transformed_spr.local_position.x, -scaled_frame_size.x, scaled_frame_size.x);
    transformed_spr.position.y = 0.0f;
    DrawingSystem::Draw(transformed_spr);
    
    transformed_spr.position.x += scaled_frame_size.x;
    transformed_spr.position.x = ufoMaths::Wrap(transformed_spr.position.x, -scaled_frame_size.x, scaled_frame_size.x);
    DrawingSystem::Draw(transformed_spr);
}
    
void
BackgroundImage::OnWidgetDraw(){}

void
BackgroundImage::OnBackgroundDraw(Camera* _camera){

    /*olc::vf2d screen_size = Engine::Get().pixel_game_engine.GetWindowSizeInPixles();

    SpriteReference transformed_spr = SpriteReference(
        key,
        _camera->Transform(_camera->position*parallax),
        offset,
        frame_size,
        scale*_camera->scale,
        rotation,
        z_index,
        true
    );

    olc::vf2d scaled_frame_size = frame_size*_camera->scale;

    transformed_spr.position.x = ufoMaths::Wrap(transformed_spr.local_position.x, -scaled_frame_size.x, scaled_frame_size.x);
    transformed_spr.position.y = 0.0f;
    DrawingSystem::Draw(transformed_spr);
    
    transformed_spr.position.x += scaled_frame_size.x;
    transformed_spr.position.x = ufoMaths::Wrap(transformed_spr.position.x, -scaled_frame_size.x, scaled_frame_size.x);
    DrawingSystem::Draw(transformed_spr);*/
}