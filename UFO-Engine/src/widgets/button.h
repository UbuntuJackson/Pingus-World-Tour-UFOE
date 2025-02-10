#ifndef BUTTON_H
#define BUTTON_H
#include <string>
#include <memory>
#include <functional>
#include "../external/olcPixelGameEngine.h"
#include "../shapes/rectangle.h"
#include "widget.h"
#include "../json/json.h"
#include "../level/level.h"

class Button : public Widget{
public:
    bool active;
    std::string on_clicked;
    std::string on_create;
    std::string text;
    olc::Pixel text_colour = olc::WHITE;
    olc::Pixel background_colour = olc::Pixel(0,150,160);
    std::function<void(Widget* _parent_widget, Button* _button)> on_pressed = [](Widget* _parent_widget, Button* _button){};
    std::function<void(Level*)> on_released = [](Level* _level){};
    std::function<void(Level*)> on_held = [](Level* _level){};
    bool refreshed = false;
    bool needs_to_refresh = false;
    Button(olc::vf2d _local_position, olc::vf2d _size, std::string _text = "", std::string on_create = "", std::string on_clicked = "");

    bool IsPressed();
    bool IsHeld();
    bool IsReleased();
    bool IsHovered();

    void OnWidgetDraw();

    static std::unique_ptr<Button> Load(Json* _json);
};

#endif