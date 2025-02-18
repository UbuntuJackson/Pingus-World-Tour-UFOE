#pragma once

#include <string>
#include "../ufo_maths/ufo_maths.h"
#include "../ufo_engine/ufo_engine.h"
#include "../external/olcPixelGameEngine.h"
#include "../graphics/graphics.h"

class Widget;

class Theme{
public:

    Theme() = default;

    virtual void OnDraw(Widget* _widget);
};