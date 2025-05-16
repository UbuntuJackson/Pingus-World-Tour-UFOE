#pragma once
#include <string>
#include <vector>
#include <memory>
#include <olcPixelGameEngine.h>
#include <unordered_map>
#include <level.h>
#include <json_variant.h>
#include <ufo_maths.h>

class Widget;
class Goal;
class WrapMenu;
class Label;
class WidgetSpriteReference;
class HoneyCoin;
class ItemSelectMenu;
class Button;
class WorldMapLocation;
class Pingu;
class PaintableSurface;
class MovingSolid;

class PingusLevel : public Level{
public:
    std::vector<Goal*> goals;
    std::vector<HoneyCoin*> honey_coin_handles;
    std::vector<WorldMapLocation*> world_map_location_handles;
    std::vector<olc::Decal*> level_decals_in_draw_order;
    std::vector<MovingSolid*> moving_solids_handles;
    olc::Decal* solid_decal;
    Vector2f custom_level_size;
    Widget* current_menu = nullptr;

    std::vector<Pingu*> anti_matter_pingus;
    std::vector<Pingu*> pingu_handles_all_pingus;
    std::unordered_map<std::string,PaintableSurface*> paintable_surface_handles;

    bool is_menu = false;
    bool level_finished = false;
    bool at_least_one_pingu_active = true;
    bool all_pingus_released = false;
    int number_of_updates_when_fast_forward = 3;
    bool fast_forward;
    Button* fast_forward_button = nullptr;
    Button* pause_button = nullptr;

    ItemSelectMenu* item_select_menu = nullptr;
    std::string pingus_level_path = "";
    int total_number_of_pingus = 0;
    int released_pingus = 0;
    int rescued_pingus = 0;
    int required_pingus = 0;
    int maximum_rescuable_pingus = 100;
    bool pingu_selected_this_frame = false;
    Button* end_level_button = nullptr;
    Label* rescued_pingus_label = nullptr;
    Label* released_pingus_label = nullptr;
    Label* max_pingus_label = nullptr;
    WidgetSpriteReference* honey_coin_hud = nullptr;

    void OnResourceLoad();

    void OnLoadActors(JsonDictionary& _actor_json);

    void OnLoad(JsonDictionary& _j);

    void OnCameraManipulation();
    
    void OnUpdate();

    void OnHandlePurge();

    void OnExit();

};