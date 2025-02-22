#include "graphics.h"
#include "../shapes/rectangle.h"

void
Graphics::DrawDecal(const std::string& _key, Vector2f _drawing_position, Vector2f _centre, Vector2f _source_position, Vector2f _source_size, Vector2f _scale, float _rotation, Colour _tint){
    graphics_engine->DrawPartialRotatedDecal(
        _drawing_position,
        AssetManager::Get().GetDecal(_key),
        _rotation,
        _centre,
        _source_position,
        _source_size,
        _scale,
        _tint
    );
}

void
Graphics::DrawFrame(const std::string& _key, Vector2f _drawing_position, Vector2f _centre, Vector2f _frame_size, Vector2f _scale, int _index, float _rotation, Colour _tint){
    ufo::Rectangle sample_rectangle = GetFrameFromSpriteSheet(_key, _index, _frame_size);
    
    graphics_engine->DrawPartialRotatedDecal(
        _drawing_position,
        AssetManager::Get().GetDecal(_key),
        _rotation,
        _centre,
        sample_rectangle.position,
        sample_rectangle.size,
        _scale,
        _tint
    ); 
}

void
Graphics::DrawRectangleFilled(Vector2f _position, Vector2f _size, const Colour& _colour){
    graphics_engine->FillRectDecal(_position,_size,_colour);
}