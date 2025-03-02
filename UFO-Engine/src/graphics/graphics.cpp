#include <memory>
#include "graphics.h"
#include "../shapes/rectangle.h"
#include "graphics_engine_pge.h"

Graphics::Graphics(){
    v_graphics_engine = std::make_unique<GraphicsEnginePGE>();
}

void
Graphics::DrawString(Vector2f _position,const std::string& _str, Colour _colour, Vector2f _scale){
    v_graphics_engine->DrawString(_position,_str,_colour,_scale);
}

void Graphics::DrawLine(Vector2f _start, Vector2f _end, Colour _colour){
    v_graphics_engine->DrawLine(_start, _end, _colour);
}

void
Graphics::DrawDecal(const std::string& _key, Vector2f _drawing_position, Vector2f _centre, Vector2f _source_position, Vector2f _source_size, Vector2f _scale, float _rotation, Colour _tint){
    v_graphics_engine->DrawDecal(_key,_drawing_position, _centre, _source_position, _source_size, _scale, _rotation, _tint);
}

void
Graphics::DrawFrame(const std::string& _key, Vector2f _drawing_position, Vector2f _centre, Vector2f _frame_size, Vector2f _scale, int _index, float _rotation, Colour _tint){
    v_graphics_engine->DrawFrame(_key,_drawing_position,_centre,_frame_size,_scale,_index,_rotation,_tint);
}

void
Graphics::DrawRectangleFilled(Vector2f _position, Vector2f _size, const Colour& _colour){
    v_graphics_engine->DrawRectangleFilled(_position,_size,_colour);
}

void Graphics::DrawRectangle(Vector2f _position, Vector2f _size, const Colour& _colour){
    v_graphics_engine->DrawRectangle(_position,_size,_colour);
}