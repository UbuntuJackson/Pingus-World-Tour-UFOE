#pragma once
#include <string>
#include <vector>
#include <memory>
#include <olcPixelGameEngine.h>
#include <level.h>
#include <json.h>
#include <ufo_maths.h>

class Widget;

using namespace ufoMaths;

class PingusLevel : public Level{
public:
    std::vector<olc::Decal*> level_decals_in_draw_order;
    olc::Decal* solid_decal;
    Vector2f custom_level_size;
    Widget* current_menu = nullptr;
    std::string pingus_level_path = "";
    int pingu_count = 0;

    void OnResourceLoad();

    void OnLoadActors(Json& _actor_json);

    void OnLoad(Json& _j);

    void OnUpdate();

};