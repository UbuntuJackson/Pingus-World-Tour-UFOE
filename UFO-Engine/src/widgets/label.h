#ifndef LABEL_H
#define LABEL_H

#include <string>
#include "../console/console.h"
#include "../ufo_engine/ufo_engine.h"
#include "../ufo_maths/ufo_maths.h"
#include "../widgets/widget.h"

using namespace ufoMaths;

class Label : public Widget{
public:
    std::string text;
    olc::Pixel colour;
    Vector2f scale;
    Label(Vector2f _local_position, Vector2f _size, std::string _text = "Hello World", olc::Pixel _colour = olc::WHITE, Vector2f _scale = {1.0f, 1.0f}) : Widget(_local_position, _size), text{_text}, colour{_colour}, scale{_scale}{

    }

    void OnWidgetDraw(){
        if(!visible) return;
        Engine::Get().pixel_game_engine.DrawStringDecal(GetGlobalPosition(), text, colour, scale);
    }
};

#endif