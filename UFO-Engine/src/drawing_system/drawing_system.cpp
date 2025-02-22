#include <algorithm>
#include "drawing_system.h"
#include "../camera/transformations.h"
#include "../console/console.h"
#include "../sprite_reference/sprite_reference.h"
#include "../asset_manager/get_rect_from_index.h"
#include "../ufo_engine/ufo_engine.h"
#include "../keyboard/single_keyboard.h"

/*void
DrawingSystem::Draw_ufoSprite(ufoSprite* _sprite){
    GraphicsProvider::Get().DrawPartialRotatedDecal(
        _sprite->position,
        AssetManager::Get().GetDecal(_sprite->key),
        _sprite->rotation,
        _sprite->offset,
        {0.0f, 0.0f},
        _sprite->frame_size,
        {_sprite->scale, _sprite->scale}
    );
}*/

void
DrawingSystem::Update(){
    
}

void DrawingSystem::DrawFilled(const ufo::Rectangle& _rectangle, const olc::Pixel& _colour){
    Engine::Get().pixel_game_engine.FillRectDecal(_rectangle.position, _rectangle.size, _colour);
}

void DrawingSystem::DrawFilled(const ufo::Rectangle& _rectangle, Camera* _camera, const olc::Pixel& _colour){
    DrawingSystem::DrawFilled(_camera->Transform(_rectangle), _colour);
}

void
DrawingSystem::Draw(const Circle& _circle, const olc::Pixel& _colour){
    Engine::Get().pixel_game_engine.DrawCircle(_circle.position, _circle.radius, _colour);
}

void
DrawingSystem::Draw(const Circle& _circle, Camera* _camera, const olc::Pixel& _colour){
    DrawingSystem::Draw(_camera->Transform(_circle), _colour);
}

void
DrawingSystem::Draw(const ufo::Rectangle& _rectangle, const olc::Pixel& _colour){
    Engine::Get().pixel_game_engine.DrawRectDecal(_rectangle.position, _rectangle.size, _colour);
}

void
DrawingSystem::Draw(const ufo::Rectangle& _rectangle, Camera* _camera, const olc::Pixel& _colour){
    DrawingSystem::Draw(_camera->Transform(_rectangle), _colour);
}

void
DrawingSystem::Draw(const Ray2& _ray, const olc::Pixel& _colour){
    Engine::Get().pixel_game_engine.DrawLineDecal(_ray.Start(), _ray.End(), _colour);
}

void
DrawingSystem::Draw(const Ray2& _ray, Camera* _camera, const olc::Pixel& _colour){
    DrawingSystem::Draw(_camera->Transform(_ray), _colour);
}

void DrawingSystem::Draw(const Pill& _pill, const olc::Pixel& _colour){
    DrawingSystem::Draw(Circle(_pill.position,_pill.radius), _colour);
    DrawingSystem::Draw(Circle(_pill.position+olc::vf2d(0.0f,_pill.height),_pill.radius), _colour);
    DrawingSystem::Draw(Ray2(_pill.position-olc::vf2d(_pill.radius,0.0f), _pill.position+olc::vf2d(0.0f,_pill.height)-olc::vf2d(_pill.radius,0.0f)), _colour);
    DrawingSystem::Draw(Ray2(_pill.position+olc::vf2d(_pill.radius,0.0f), _pill.position+olc::vf2d(0.0f,_pill.height)+olc::vf2d(_pill.radius,0.0f)), _colour);
}

void DrawingSystem::Draw(const Pill& _pill, Camera* _camera, const olc::Pixel& _colour){
    DrawingSystem::Draw(_camera->Transform(_pill), _colour);
}

void
DrawingSystem::Draw(const olc::vf2d& _point, const olc::Pixel& _colour){

}

void
DrawingSystem::Draw(const olc::vf2d& _point, Camera* _camera,  const olc::Pixel& _colour){

}

void
DrawingSystem::Draw(const SpriteReference& _sprite){
    if(!_sprite.visible) return;
    ufo::Rectangle sample_rectangle = GetFrameFromSpriteSheet(_sprite.key,_sprite.current_frame_index,_sprite.frame_size);
    Engine::Get().pixel_game_engine.DrawPartialRotatedDecal(
        _sprite.position,
        AssetManager::Get().GetDecal(_sprite.key),
        _sprite.rotation,
        _sprite.offset,
        sample_rectangle.position,
        sample_rectangle.size,
        _sprite.scale,
        _sprite.tint
    );
}

void
DrawingSystem::Draw(const SpriteReference& _sprite, Camera* _camera){
    if(!_sprite.visible) return;
    //Console::Out("input for the transform",_sprite.position);
    DrawingSystem::Draw(_camera->Transform(_sprite));
    //Console::Out(_camera->Transform(_sprite).position);
}

void DrawingSystem::DrawString(const olc::vf2d& _position,const std::string& _s, const olc::Pixel& _colour, const olc::vf2d& _scale){
    Engine::Get().pixel_game_engine.DrawStringDecal(_position, _s, _colour, _scale);
}