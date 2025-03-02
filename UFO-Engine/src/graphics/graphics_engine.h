#pragma once
#include "../external/olcPixelGameEngine.h"
#include "../ufo_maths/ufo_maths.h"

class GraphicsEngine{
public:

    virtual void DrawString(Vector2f _position,const std::string& _str, const olc::Pixel& _colour, Vector2f _scale) = 0;
    //virtual void DrawRectangle(Vector2f _position, Vector2f _size, int _line_thickness) = 0;
    virtual void DrawRectangleFilled(Vector2f _position, Vector2f _size,  const olc::Pixel& _colour) = 0;
    virtual void DrawRectangle(Vector2f _position, Vector2f _size, const olc::Pixel& _colour) = 0;
    //virtual void DrawCircle(Vector2f _position, Vector2f _size, int _line_thickness) = 0;
    //virtual void DrawCircleFilled(Vector2f _position, Vector2f _size) = 0;
    virtual void DrawLine(Vector2f _start, Vector2f _end, const olc::Pixel& _colour) = 0;
    virtual void DrawDecal(const std::string& _key, Vector2f _drawing_position, Vector2f _centre, Vector2f _source_position, Vector2f _source_size, Vector2f _scale, float _rotation, olc::Pixel _tint) = 0;
    virtual void DrawFrame(const std::string& _key, Vector2f _drawing_position, Vector2f _centre, Vector2f _frame_size, Vector2f _scale, int _index, float _rotation, olc::Pixel _tint) = 0;

};