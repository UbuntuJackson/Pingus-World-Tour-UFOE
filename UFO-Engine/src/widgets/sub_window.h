#pragma once
#include "widget.h"
#include "button.h"
#include "../external/olcPixelGameEngine.h"

class SubWindow : public Widget{
public:
    SubWindow(Vector2f _local_position, Vector2f _size) : Widget(_local_position, _size){}

    void OnLevelEnter(Level *_level){
        Widget::OnLevelEnter(_level);

        auto u_button_x = std::make_unique<Button>(Vector2f(0.0f, 0.0f), Vector2f(16.0f, 32.0f));

        u_button_x->text = "x";
        u_button_x->background_colour = olc::MAGENTA;
        
        auto u_button_o = std::make_unique<Button>(Vector2f(16.0f, 0.0f), Vector2f(16.0f, 32.0f));
        
        u_button_o->text = "o";
        u_button_o->background_colour = olc::GREY;

        AddChild(std::move(u_button_x));
        AddChild(std::move(u_button_o));
    }
};