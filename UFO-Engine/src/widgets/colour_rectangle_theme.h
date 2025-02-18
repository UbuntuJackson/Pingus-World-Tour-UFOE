#pragma once

#include "theme.h"
#include "../graphics/graphics.h"

class ColourRectangleTheme : public Theme{
public:
    Colour colour = Graphics::CYAN;

    ColourRectangleTheme(Colour _colour);

    void OnDraw(Widget* _widget);
};