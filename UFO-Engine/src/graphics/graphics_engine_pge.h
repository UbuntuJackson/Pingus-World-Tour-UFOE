#pragma once

#include "../ufo_maths/ufo_maths.h"
#include "graphics.h"
#include "graphics_engine.h"
#include "../ufo_engine/ufo_engine.h"

class GraphicsEnginePGE : public GraphicsEngine{
public:
    olc::PixelGameEngine* graphics_engine = nullptr;

    GraphicsEnginePGE(){
        graphics_engine = &(Engine::Get().pixel_game_engine);
    }
    void DrawString(Vector2f _position,const std::string& _str, const olc::Pixel& _colour, Vector2f _scale);
    //void DrawRectangle(Vector2f _position, Vector2f _size, int _line_thickness);
    void DrawRectangleFilled(Vector2f _position, Vector2f _size,  const Colour& _colour);
    void DrawRectangle(Vector2f _position, Vector2f _size, const Colour& _colour);
    //void DrawCircle(Vector2f _position, Vector2f _size, int _line_thickness);
    //void DrawCircleFilled(Vector2f _position, Vector2f _size);
    void DrawLine(Vector2f _start, Vector2f _end, const olc::Pixel& _colour);
    void DrawDecal(const std::string& _key, Vector2f _drawing_position, Vector2f _centre, Vector2f _source_position, Vector2f _source_size, Vector2f _scale, float _rotation, Colour _tint);
    void DrawFrame(const std::string& _key, Vector2f _drawing_position, Vector2f _centre, Vector2f _frame_size, Vector2f _scale, int _index, float _rotation, Colour _tint);
};