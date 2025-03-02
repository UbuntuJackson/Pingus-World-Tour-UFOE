#pragma once

#include "../external/olcPixelGameEngine.h"
#include "../ufo_maths/ufo_maths.h"
#include "graphics_engine.h"

using namespace ufoMaths;

typedef olc::Pixel Colour;

class Graphics{
public:
    olc::PixelGameEngine* graphics_engine = nullptr;

    static const inline Colour RED = olc::RED;
    static const inline Colour DARK_RED = olc::DARK_RED;
    static const inline Colour VERY_DARK_RED = olc::VERY_DARK_RED;
    static const inline Colour BLUE = olc::BLUE;
    static const inline Colour DARK_BLUE = olc::DARK_BLUE;
    static const inline Colour VERY_DARK_BLUE = olc::VERY_DARK_BLUE;
    static const inline Colour GREEN = olc::GREEN;
    static const inline Colour DARK_GREEN = olc::DARK_GREEN;
    static const inline Colour VERY_DARK_GREEN = olc::VERY_DARK_GREEN;
    static const inline Colour YELLOW = olc::YELLOW;
    static const inline Colour DARK_YELLOW = olc::DARK_YELLOW;
    static const inline Colour VERY_DARK_YELLOW = olc::VERY_DARK_YELLOW;
    static const inline Colour MAGENTA = olc::MAGENTA;
    static const inline Colour DARK_MAGENTA = olc::DARK_MAGENTA;
    static const inline Colour VERY_DARK_MAGENTA = olc::VERY_DARK_MAGENTA;
    static const inline Colour CYAN = olc::CYAN;
    static const inline Colour DARK_CYAN = olc::DARK_CYAN;
    static const inline Colour VERY_DARK_CYAN = olc::VERY_DARK_CYAN;
    static const inline Colour WHITE = olc::WHITE;

    Graphics();

    std::unique_ptr<GraphicsEngine> v_graphics_engine;

    static Graphics& Get(){
        static Graphics self;
        return self;
    }

    //Blit to specific surface?
    void DrawString(Vector2f _position,const std::string& _str, Colour _colour, Vector2f _scale);
    //void DrawRectangle(Vector2f _position, Vector2f _size, int _line_thickness);
    void DrawRectangleFilled(Vector2f _position, Vector2f _size,  const Colour& _colour);
    void DrawRectangle(Vector2f _position, Vector2f _size, const Colour& _colour);
    //void DrawCircle(Vector2f _position, Vector2f _size, int _line_thickness);
    //void DrawCircleFilled(Vector2f _position, Vector2f _size);
    void DrawLine(Vector2f _start, Vector2f _end, Colour _colour);
    void DrawDecal(const std::string& _key, Vector2f _drawing_position, Vector2f _centre, Vector2f _source_position, Vector2f _source_size, Vector2f _scale, float _rotation, Colour _tint);
    void DrawFrame(const std::string& _key, Vector2f _drawing_position, Vector2f _centre, Vector2f _frame_size, Vector2f _scale, int _index, float _rotation, Colour _tint);

};