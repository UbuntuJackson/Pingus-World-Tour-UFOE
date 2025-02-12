#pragma once

#include <string>
#include "../ufo_maths/ufo_maths.h"
#include "../ufo_engine/ufo_engine.h"
#include "../external/olcPixelGameEngine.h"
#include "../graphics/graphics.h"

class Widget;

class Theme{
public:
    int right_edge = 0;
    int left_edge = 0;
    int top_edge = 0;
    int bottom_edge = 0;

    std::string key;
    Colour tint = Graphics::Get().WHITE;
    bool non_default_theme = false;

    Theme() = default;

    Theme(std::string _key, int _left_edge, int _right_edge, int _top_edge, int _bottom_edge);

    void OnDraw(Widget* _widget);
};