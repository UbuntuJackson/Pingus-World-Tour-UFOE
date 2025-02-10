#pragma once
#include "../shapes/circle.h"
#include "../shapes/shape.h"
#include "../level/actor.h"
#include "../ufo_maths/ufo_maths.h"
#include "../ufo_clock/timer.h"

class Level;
class Json;
class TuxTaleLevel;
class SpriteReference;

class ControllableUFO : public Actor{
public:
    TuxTaleLevel* level = nullptr;
    SpriteReference* sprite_ref = nullptr;
    std::vector<int> sprite_references_with_time_limit;

    Vector2f velocity;
    Vector2f velocity_before_dash;
    float friction = 100.0f;
    float acceleration = 760.0f;
    float max_speed = 100.0f;
    bool vehicle_active = false;
    bool is_dashing = false;
    Timer dash_timer;
    float red_shift = 255.0f;

    ControllableUFO(Vector2f _local_position);
    void OnLevelEnter(Level* _level);
    void OnUpdate();
    static std::unique_ptr<ControllableUFO> Load(Json* _json);
    /*static void OnLoadFromEditor(ControllableUFO* _instance, Json* _json){
        _instance->local_position.x = (float)_json->GetAsInt("x");
        _instance->local_position.y = (float)_json->GetAsInt("y");
        _instance->friction = 300.0f;
    }*/

   void OnWidgetDraw();

};