#pragma once

#include "../external/olcPixelGameEngine.h"
#include "../ufo_maths/ufo_maths.h"

using namespace ufoMaths;

typedef olc::Pixel Colour;

class Graphics{
public:
    olc::PixelGameEngine* graphics_engine = nullptr;

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
    const Colour WHITE = olc::WHITE;

    Graphics(olc::PixelGameEngine* _graphics_engine){
        graphics_engine = _graphics_engine;
    }

    static Graphics& Get(){
        static Graphics self(&Engine::Get().pixel_game_engine);
        return self;
    }

    //Blit to specific surface?
    void DrawString(Vector2f _position,const std::string& _str, Colour _colour, Vector2f _scale, Vector2f _rotation);
    void DrawRectangle(Vector2f _position, Vector2f _size, int _line_thickness);
    void DrawRectangleFilled(Vector2f _position, Vector2f _size);
    void DrawCircle(Vector2f _position, Vector2f _size, int _line_thickness);
    void DrawCircleFilled(Vector2f _position, Vector2f _size);
    void DrawLine(Vector2f _start, Vector2f _end, int _line_thickness, Colour _colour);
    void DrawDecal(const std::string& _key, Vector2f _drawing_position, Vector2f _centre, Vector2f _source_position, Vector2f _source_size, Vector2f _scale, float _rotation, Colour _tint);
    void DrawFrame(const std::string& _key, Vector2f _drawing_position, Vector2f _centre, Vector2f _frame_size, Vector2f _scale, int _index, float _rotation, Colour _tint);

};