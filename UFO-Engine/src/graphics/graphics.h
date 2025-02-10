#pragma once

#include "../external/olcPixelGameEngine.h"
#include "../ufo_maths/ufo_maths.h"

using namespace ufoMaths;

typedef olc::Pixel Colour;

class Graphics{
public:

    const Colour RED = olc::RED;
    const Colour DARK_RED = olc::DARK_RED;
    const Colour VERY_DARK_RED = olc::VERY_DARK_RED;
    const Colour BLUE = olc::BLUE;
    const Colour DARK_BLUE = olc::DARK_BLUE;
    const Colour VERY_DARK_BLUE = olc::VERY_DARK_BLUE;
    const Colour GREEN = olc::GREEN;
    const Colour DARK_GREEN = olc::DARK_GREEN;
    const Colour VERY_DARK_GREEN = olc::VERY_DARK_GREEN;
    const Colour YELLOW = olc::YELLOW;
    const Colour DARK_YELLOW = olc::DARK_YELLOW;
    const Colour VERY_DARK_YELLOW = olc::VERY_DARK_YELLOW;
    const Colour MAGENTA = olc::MAGENTA;
    const Colour DARK_MAGENTA = olc::DARK_MAGENTA;
    const Colour VERY_DARK_MAGENTA = olc::VERY_DARK_MAGENTA;
    const Colour CYAN = olc::CYAN;
    const Colour DARK_CYAN = olc::DARK_CYAN;
    const Colour VERY_DARK_CYAN = olc::VERY_DARK_CYAN;

    Graphics(){
        
    }

    //Blit to specific surface?
    void DrawString(Vector2f _position,const std::string& _str, Colour _colour, Vector2f _scale, Vector2f _rotation);
    void DrawRectangle(Vector2f _position, Vector2f _size, int _line_thickness);
    void DrawRectangleFilled(Vector2f _position, Vector2f _size);
    void DrawCircle(Vector2f _position, Vector2f _size, int _line_thickness);
    void DrawCircleFilled(Vector2f _position, Vector2f _size);
    void DrawLine(Vector2f _start, Vector2f _end, int _line_thickness, Colour _colour);

};