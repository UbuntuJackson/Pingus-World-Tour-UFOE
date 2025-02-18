#pragma once

#include "theme.h"

class NinePatchTheme : public Theme{
public:
    int right_edge = 0;
    int left_edge = 0;
    int top_edge = 0;
    int bottom_edge = 0;

    std::string key;
    Colour tint = Graphics::Get().WHITE;

    NinePatchTheme(std::string _key, int _left_edge, int _right_edge, int _top_edge, int _bottom_edge);

    void OnDraw(Widget* _widget);
};