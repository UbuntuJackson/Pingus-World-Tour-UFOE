#pragma once
#include <widget.h>
#include <mouse.h>
#include <graphics.h>
#include <ufo_maths.h>
#include "pingus_level.h"

class Button;
class WrapMenu;
class Level;
class PingusLevel;
class WrapMenu;
class PaintableSurface;
class ColourPicker;

/// @spawn;
class PingusWorldTourEditor : public Widget{
public:
    Button* b_save = nullptr;
    Button* b_load = nullptr;
    Button* b_import_layer = nullptr;

    //Colour and texture settings
    Button* b_upper_crust_colour = nullptr;
    Button* b_lower_crust_colour = nullptr;
    Button* b_mantle_texture = nullptr;

    WrapMenu* m_layers = nullptr;

    WrapMenu* m_colour_settings = nullptr;

    PingusLevel* level = nullptr;
    PaintableSurface* selected_layer = nullptr;

    ColourPicker* colour_picker = nullptr;

    float brush_radius = 20.0f;

    Colour SEMI_SOLID = Colour(255,0,0);
    Colour SOLID = Colour(255,255,255);
    Colour INDESTRUCTIBLE = Colour(0x40,0x40,0x40);

    Colour selected_solid_type = SOLID;

    WrapMenu* m_solid_type = nullptr;

    Button* b_solid = nullptr;
    Button* b_semi_solid = nullptr;
    Button* b_indestructible_solid = nullptr;

    enum SpawnModes{
        PINGU,
        SPAWNER,
        COIN
    };

    int spawn_mode = SpawnModes::PINGU;

    Button* b_spawn_pingu = nullptr;

    PingusWorldTourEditor(Vector2f _);

    void OnLevelEnter(Level* _level);

    void AddLayer(std::string _name, std::string _path);

    void OnWidgetHovered();
    void OnUpdate();

    void OnWidgetDraw();

};