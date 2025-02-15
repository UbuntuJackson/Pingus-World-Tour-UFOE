#pragma once
#include <string>
#include <vector>
#include <memory>
#include <olcPixelGameEngine.h>
#include <level.h>
#include <json.h>
#include <ufo_maths.h>

class Widget;
class Goal;
class WrapMenu;
class Label;
class WidgetSpriteReference;
class HoneyCoin;
class ItemSelectMenu;

class PingusLevel : public Level{
public:
    std::vector<Goal*> goals;
    std::vector<HoneyCoin*> honey_coin_handles;
    std::vector<olc::Decal*> level_decals_in_draw_order;
    olc::Decal* solid_decal;
    Vector2f custom_level_size;
    Widget* current_menu = nullptr;

    bool is_menu = false;
    bool level_finished = false;

    ItemSelectMenu* item_select_menu = nullptr;
    std::string pingus_level_path = "";
    int total_number_of_pingus = 0;
    int released_pingus = 0;
    int rescued_pingus = 0;
    int required_pingus = 0;
    int maximum_rescuable_pingus = 100;
    Label* rescued_pingus_label = nullptr;
    Label* released_pingus_label = nullptr;
    WidgetSpriteReference* honey_coin_hud = nullptr;

    void OnResourceLoad();

    void OnLoadActors(Json& _actor_json);

    void OnLoad(Json& _j);

    void OnUpdate();

    void OnHandlePurge();

};