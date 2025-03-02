#include "../ufo_maths/ufo_maths.h"
#include "graphics.h"
#include "graphics_engine.h"
#include "graphics_engine_pge.h"

void GraphicsEnginePGE::DrawString(Vector2f _position,const std::string& _str, const olc::Pixel& _colour, Vector2f _scale){
    graphics_engine->DrawStringDecal(_position, _str, _colour, _scale);
}

void GraphicsEnginePGE::DrawLine(Vector2f _start, Vector2f _end, const olc::Pixel& _colour){
    graphics_engine->DrawLineDecal(_start, _end, _colour);
}

void
GraphicsEnginePGE::DrawDecal(const std::string& _key, Vector2f _drawing_position, Vector2f _centre, Vector2f _source_position, Vector2f _source_size, Vector2f _scale, float _rotation, Colour _tint){
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
GraphicsEnginePGE::DrawFrame(const std::string& _key, Vector2f _drawing_position, Vector2f _centre, Vector2f _frame_size, Vector2f _scale, int _index, float _rotation, Colour _tint){
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
GraphicsEnginePGE::DrawRectangleFilled(Vector2f _position, Vector2f _size, const Colour& _colour){
    graphics_engine->FillRectDecal(_position,_size,_colour);
}

void GraphicsEnginePGE::DrawRectangle(Vector2f _position, Vector2f _size, const Colour& _colour){
    graphics_engine->DrawRectDecal(_position,_size,_colour);
}